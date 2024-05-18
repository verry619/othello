#include "GuiStateInGameIdle.h"

void GuiStateInGameIdle::PutDisc(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC_POS enDiscPos)
{
	DISC* penBoard = pcCom->FetchCurrentBoard();
	penBoard[enDiscPos.ucRow * m_ucGameCol + enDiscPos.ucCol] = s_currentTurn;
	UpdateBoard(penBoard);
	DrawBoard(pcMainWnd);
	delete penBoard;

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::PUT_DISC;
	msg.p1 = static_cast<unsigned short>(s_currentTurn);
	msg.p2 = enDiscPos.ucRow;
	msg.p3 = enDiscPos.ucCol;

	pcCom->SendMsg(OTHELLO_PROCESS_ID::GAME, msg);
}

void GuiStateInGameIdle::PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes, DISC enDiscCol)
{
	ProceedNextTurn(pcCom, pcMainWnd, bRes, enDiscCol);
}

void GuiStateInGameIdle::GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GameStart_Sub(pcCom, pcMainWnd);
}

void GuiStateInGameIdle::PassTurn(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC enDiscCol)
{
	ProceedNextTurn(pcCom, pcMainWnd, true, enDiscCol);
}

void GuiStateInGameIdle::GameEnd(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GameEnd_Sub(pcCom, pcMainWnd);
}