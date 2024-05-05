// GuiDummy.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

#include <chrono>
#include <thread>

#include "Cmn.h"
#include "CmnCom.h"

#include "CmnLog.h"

#define BOARD_ROW_LEN 6
#define BOARD_COL_LEN 6

static void PrintBoard(DISC* punBoard, int unRowMax, int unColMax);
static void RcvMsg(const char* pcBuf, unsigned int unBufLen);

static bool bComStartWaiting = true;
static bool bGameStartWaiting = true;
static bool bPutDiscWaiting = true;
static bool bGameQuitWaiting = true;
static bool bValidDisc = false;
static bool bGameEnd = false;

int main()
{
	std::cout << "Hello World!\n";

	CmnCom* pcComCom;
	pcComCom = new CmnCom();

	pcComCom->Initialize(std::bind(RcvMsg, std::placeholders::_1, std::placeholders::_2));
	pcComCom->StartCom(SOCKET_ROLE::CLIENT);

	while (bComStartWaiting)
	{
		NOP_FUNCTION;
	}

	GAME_SETTING enGameSetting = GAME_SETTING::CPU_CPU;

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::GAME_START;
	msg.p1 = BOARD_ROW_LEN;
	msg.p2 = BOARD_COL_LEN;
	msg.p3 = static_cast<unsigned int>(enGameSetting);
	msg.p4 = 0;

	char cBuf[sizeof(OTHELLO_MSG)];
	CmnCom::ConvMsgToCbuf(&msg, cBuf);
	pcComCom->SendMsg(cBuf, sizeof(cBuf));

	while (bGameStartWaiting)
	{
		NOP_FUNCTION;
	}
	bGameStartWaiting = true;

	CmnCom::ShmData penShm;

	pcComCom->ReadShm(&penShm);
	PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);

	while (1)
	{
		unsigned int cRow, cCol;

		do
		{
			if ((GAME_SETTING::HUMAN_HUMAN == enGameSetting) ||
				(GAME_SETTING::HUMAN_CPU == enGameSetting))
			{
				std::cout << "BLACK:\nROW:";
				std::cin >> cRow;
				std::cout << "COL:";
				std::cin >> cCol;

				msg.enId = OTHELLO_MSG_ID::PUT_DISC;
				msg.p1 = static_cast<unsigned int>(DISC::BLACK);
				msg.p2 = cRow;
				msg.p3 = cCol;

				CmnCom::ConvMsgToCbuf(&msg, cBuf);

				pcComCom->SendMsg(cBuf, sizeof(cBuf));
			}

			while (bPutDiscWaiting)
			{
				NOP_FUNCTION;
			}
			if (bGameEnd)
			{
				break;
			}
			bPutDiscWaiting = true;

			pcComCom->ReadShm(&penShm);
			PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);

			if (bValidDisc)
			{
				bValidDisc = false;
				break;
			}
			bValidDisc = false;

		} while (1);

		do
		{
			if ((GAME_SETTING::HUMAN_HUMAN == enGameSetting) ||
				(GAME_SETTING::CPU_HUMAN == enGameSetting))
			{
				std::cout << "WHITE:\nROW:";
				std::cin >> cRow;
				std::cout << "COL:";
				std::cin >> cCol;

				msg.enId = OTHELLO_MSG_ID::PUT_DISC;
				msg.p1 = static_cast<unsigned int>(DISC::WHITE);
				msg.p2 = cRow;
				msg.p3 = cCol;

				CmnCom::ConvMsgToCbuf(&msg, cBuf);

				pcComCom->SendMsg(cBuf, sizeof(cBuf));
			}

			while (bPutDiscWaiting)
			{
				NOP_FUNCTION;
			}
			if (bGameEnd)
			{
				break;
			}
			bPutDiscWaiting = true;

			pcComCom->ReadShm(&penShm);
			PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);

			if (bValidDisc)
			{
				bValidDisc = false;
				break;
			}
			bValidDisc = false;

		} while (1);

		if (bGameEnd)
		{
			BOARD_INFO enBoardInfo;
			pcComCom->ReadShm(&penShm);
			enBoardInfo.enSize = penShm.enBoardSize;
			enBoardInfo.penDiscs = penShm.enBoard;

			unsigned int unCountBlack = 0;
			unsigned int unCountWhite = 0;

			DISC enWinner = CmnCountDiscs(enBoardInfo, unCountBlack, unCountWhite);

			OTHELLO_LOG_PARAM p = { 0,0,0,0 };
			if (DISC::BLACK == enWinner)
			{
				p.p1 = static_cast<unsigned int>(GAME_RESULT::BLACK_WIN);
			}
			else if (DISC::WHITE == enWinner)
			{
				p.p1 = static_cast<unsigned int>(GAME_RESULT::WHITE_WIN);
			}
			else
			{
				p.p1 = static_cast<unsigned int>(GAME_RESULT::DRAW);
			}
			WRITE_DEV_LOG(OTHELLO_LOG_ID::GAME_RESULT, p);

			bGameEnd = false;

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			msg.enId = OTHELLO_MSG_ID::GAME_START;
			msg.p1 = BOARD_ROW_LEN;
			msg.p2 = BOARD_COL_LEN;
			msg.p3 = static_cast<unsigned int>(enGameSetting);
			msg.p4 = 0;

			char cBuf[sizeof(OTHELLO_MSG)];
			CmnCom::ConvMsgToCbuf(&msg, cBuf);
			pcComCom->SendMsg(cBuf, sizeof(cBuf));

			while (bGameStartWaiting)
			{
				NOP_FUNCTION;
			}
			bGameStartWaiting = true;

			CmnCom::ShmData penShm;

			pcComCom->ReadShm(&penShm);
			PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);
		}
	}
}

static void PrintBoard(DISC* punBoard, int unRowMax, int unColMax)
{
	std::cout << "###################\n";
	std::cout << "  ";
	for (int col = 0;col < unColMax;col++)
	{
		std::cout << col << " ";
	}
	std::cout << " <COL\n";

	std::cout << "  ";
	for (int col = 0;col < unColMax;col++)
	{
		std::cout << "--";
	}
	std::cout << "\n";

	for (int row = 0;row < unRowMax;row++)
	{
		std::cout << row << "|";
		for (int col = 0;col < unColMax;col++)
		{
			std::cout << static_cast<unsigned int>(punBoard[row * unColMax + col]) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "^\nROW\n";
	std::cout << "###################\n";
}

static void RcvMsg(const char* pcBuf, unsigned int unBufLen)
{
	OTHELLO_MSG enMsg;
	CmnCom::ConvCbufToMsg(pcBuf, &enMsg);

	switch (enMsg.enId)
	{
	case OTHELLO_MSG_ID::COM_START:
		if (SOCKET_ROLE::SERVER == static_cast<SOCKET_ROLE>(enMsg.p1))
		{
			//WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE,"COM START");
			bComStartWaiting = false;
		}
		break;
	case OTHELLO_MSG_ID::GAME_START:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START);
		bGameStartWaiting = false;
		break;
	case OTHELLO_MSG_ID::GAME_QUIT:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_QUIT);
		{
			OTHELLO_LOG_PARAM p = { static_cast<unsigned int>(GAME_RESULT::NO_CONTEST),0,0,0 };
			WRITE_DEV_LOG(OTHELLO_LOG_ID::GAME_RESULT, p);
		}
		
		bGameQuitWaiting = false;
		break;
	case OTHELLO_MSG_ID::GAME_END:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_END);
		bPutDiscWaiting = false;
		bGameEnd = true;
		break;
	case OTHELLO_MSG_ID::PUT_DISC:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC);
		bPutDiscWaiting = false;
		if (O_SUCCESS == enMsg.p1)
		{
			bValidDisc = true;
		}
		else
		{
			bValidDisc = false;
		}
		break;
	case OTHELLO_MSG_ID::PASS_TURN:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PASS_TURN);
		bPutDiscWaiting = false;
		bValidDisc = true;
		break;
	default:
		break;
	}
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
