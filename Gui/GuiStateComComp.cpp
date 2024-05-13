#include "GuiStateComComp.h"

#include <iostream>

#define BOARD_ROW_LEN 6
#define BOARD_COL_LEN 6

void GuiStateComComp::ComStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	std::cout << "GuiStateComComp::ComStartComp" << std::endl;
}

void GuiStateComComp::GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	std::cout << "GuiStateComComp::GameStart" << std::endl;

	GAME_SETTING enGameSetting = GAME_SETTING::CPU_HUMAN;

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::GAME_START;
	msg.p1 = BOARD_ROW_LEN;
	msg.p2 = BOARD_COL_LEN;
	msg.p3 = static_cast<unsigned short>(enGameSetting);
	msg.p4 = 0;

	pcCom->UpdateBoardSize(BOARD_ROW_LEN, BOARD_COL_LEN);
	pcCom->SendMsg(OTHELLO_PROCESS_ID::GAME, msg);
}