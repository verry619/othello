// GuiDummy.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

#include <chrono>
#include <thread>

#include "Cmn.h"
#include "CmnCom.h"

#define BOARD_ROW_LEN 6
#define BOARD_COL_LEN 6

static void PrintBoard(DISC* punBoard, int unRowMax, int unColMax);
static void RcvMsg(void);

int main()
{
	std::cout << "Hello World!\n";

	CmnCom* pcComCom;
	pcComCom = new CmnCom();

	pcComCom->Initialize(std::bind(RcvMsg));
	pcComCom->StartCom(EN_SOCKET_ROLE::CLIENT);

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::GAME_START;
	msg.p1 = BOARD_ROW_LEN;
	msg.p2 = BOARD_COL_LEN;
	msg.p3 = static_cast<unsigned int>(GAME_SETTING::HUMAN_HUMAN);
	msg.p4 = 0;

	std::cout << "pause:\n";
	std::cin >> msg.p4;

	char cBuf[64];
	CmnConvMsgToCbuf(&msg, cBuf);
	pcComCom->SendMsg(cBuf, sizeof(cBuf));

	std::cout << "pause:\n";
	std::cin >> msg.p4;

	CmnCom::ShmData penShm;

	pcComCom->ReadShm(&penShm);
	PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);

	msg.enId = OTHELLO_MSG_ID::PUT_DISC;

	while (1)
	{
		unsigned int cRow, cCol;

		std::cout << "BLACK:\nROW:";
		std::cin >> cRow;
		std::cout << "COL:";
		std::cin >> cCol;

		msg.p1 = static_cast<unsigned int>(DISC::BLACK);;
		msg.p2 = cRow;
		msg.p3 = cCol;

		CmnConvMsgToCbuf(&msg, cBuf);
		pcComCom->SendMsg(cBuf, sizeof(cBuf));

		std::cout << "ReadBoard:\n";
		std::cin >> msg.p4;

		pcComCom->ReadShm(&penShm);
		PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);

		std::cout << "WHITE:\nROW:";
		std::cin >> cRow;
		std::cout << "COL:";
		std::cin >> cCol;

		msg.p1 = static_cast<unsigned int>(DISC::WHITE);;
		msg.p2 = cRow;
		msg.p3 = cCol;

		CmnConvMsgToCbuf(&msg, cBuf);
		pcComCom->SendMsg(cBuf, sizeof(cBuf));

		std::cout << "ReadBoard:\n";
		std::cin >> msg.p4;

		pcComCom->ReadShm(&penShm);
		PrintBoard(penShm.enBoard, BOARD_ROW_LEN, BOARD_COL_LEN);
	}
}

static void PrintBoard(DISC *punBoard, int unRowMax, int unColMax)
{
	std::cout << "###################\n";
	std::cout << "  ";
	for (int col = 0;col < unColMax;col++)
	{
		std::cout << col << " ";
	}
	std::cout << " <-COL\n";

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
	std::cout << "^\n|\nROW\n";
	std::cout << "###################\n";
}

static void RcvMsg(void)
{
	std::cout << "GuiDummy RcvMsg()\n";
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
