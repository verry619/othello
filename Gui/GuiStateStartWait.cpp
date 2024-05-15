#include "GuiStateStartWait.h"

void GuiStateStartWait::GameStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	DISC* penBoard = pcCom->FetchCurrentBoard();
	UpdateBoard(penBoard);
	DrawBoard(pcMainWnd);
	delete penBoard;

	s_currentTurn = DISC::BLACK;
}