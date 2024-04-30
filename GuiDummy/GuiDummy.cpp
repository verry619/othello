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
static bool bBlackWaiting = true;
static bool bWhiteWaiting = true;
static bool bGameQuitWaiting = true;
static bool bValidDisc = false;

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

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::GAME_START;
	msg.p1 = BOARD_ROW_LEN;
	msg.p2 = BOARD_COL_LEN;
	msg.p3 = static_cast<unsigned int>(GAME_SETTING::HUMAN_HUMAN);
	msg.p4 = 0;

	char cBuf[sizeof(OTHELLO_MSG)];
	CmnCom::ConvMsgToCbuf(&msg, cBuf);
	pcComCom->SendMsg(cBuf, sizeof(cBuf));

	while (bGameStartWaiting)
	{
		NOP_FUNCTION;
	}

	CmnCom::ShmData penShm;

	pcComCom->ReadShm(&penShm);
	PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);

	while (1)
	{
		unsigned int cRow, cCol;

		do
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

			bBlackWaiting = true;
			bValidDisc = false;

			pcComCom->SendMsg(cBuf, sizeof(cBuf));

			while (bBlackWaiting)
			{
				NOP_FUNCTION;
			}

			pcComCom->ReadShm(&penShm);
			PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);

			if (bValidDisc)
			{
				break;
			}

		} while (1);

		do
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

			bBlackWaiting = true;
			bValidDisc = false;

			pcComCom->SendMsg(cBuf, sizeof(cBuf));

			while (bBlackWaiting)
			{
				NOP_FUNCTION;
			}

			pcComCom->ReadShm(&penShm);
			PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);

			if (bValidDisc)
			{
				break;
			}

		} while (1);

		msg.enId = OTHELLO_MSG_ID::GAME_QUIT;
		msg.p1 = 0;
		msg.p2 = 0;
		msg.p3 = 0;

		CmnCom::ConvMsgToCbuf(&msg, cBuf);
		pcComCom->SendMsg(cBuf, sizeof(cBuf));

		while (bGameQuitWaiting)
		{
			NOP_FUNCTION;
		}

		bGameQuitWaiting = true;

		OTHELLO_MSG msg;
		msg.enId = OTHELLO_MSG_ID::GAME_START;
		msg.p1 = BOARD_ROW_LEN;
		msg.p2 = BOARD_COL_LEN;
		msg.p3 = static_cast<unsigned int>(GAME_SETTING::HUMAN_HUMAN);
		msg.p4 = 0;

		char cBuf[sizeof(OTHELLO_MSG)];
		CmnCom::ConvMsgToCbuf(&msg, cBuf);
		pcComCom->SendMsg(cBuf, sizeof(cBuf));

		while (bGameStartWaiting)
		{
			NOP_FUNCTION;
		}

		CmnCom::ShmData penShm;

		pcComCom->ReadShm(&penShm);
		PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);
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
		bGameQuitWaiting = false;
		break;
	case OTHELLO_MSG_ID::PUT_DISC:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC);
		bBlackWaiting = false;
		if (O_SUCCESS == enMsg.p1)
		{
			bValidDisc = true;
		}
		else
		{
			bValidDisc = false;
		}
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
