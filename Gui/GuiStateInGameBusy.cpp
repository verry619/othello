#include "GuiStateInGameBusy.h"

void GuiStateInGameBusy::PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	DISC* penBoard = pcCom->FetchCurrentBoard();
	UpdateBoard(penBoard);
	DrawBoard(pcMainWnd);
	delete penBoard;
}