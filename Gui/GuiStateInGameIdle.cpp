#include "GuiStateInGameIdle.h"

#define BOARD_ROW_LEN 6
#define BOARD_COL_LEN 6

void GuiStateInGameIdle::PutDisc(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC_POS enDiscPos)
{
	DISC* penBoard = pcCom->FetchCurrentBoard();
	penBoard[enDiscPos.ucRow * BOARD_COL_LEN + enDiscPos.ucCol] = DISC::WHITE;
	UpdateBoard(penBoard);
	DrawBoard(pcMainWnd);
	delete penBoard;

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::PUT_DISC;
	msg.p1 = static_cast<unsigned short>(DISC::WHITE); /* <TBD> COLOR must be variant */
	msg.p2 = enDiscPos.ucRow;
	msg.p3 = enDiscPos.ucCol;

	pcCom->SendMsg(OTHELLO_PROCESS_ID::GAME, msg);
}

void GuiStateInGameIdle::PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes)
{
	PutDiscComp_InGame(pcCom, pcMainWnd, bRes);
}

void GuiStateInGameIdle::GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GameStart_Sub(pcCom, pcMainWnd);
}