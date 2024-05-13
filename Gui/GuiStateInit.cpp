#include "GuiStateInit.h"

#include <iostream>

void GuiStateInit::ComStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	std::cout << "GuiStateInit::ComStartComp" << std::endl;

}

void GuiStateInit::GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	std::cout << "GuiStateInit::GameStart" << std::endl;

	DISC* penBoard = pcCom->FetchCurrentBoard();
	UpdateBoard(penBoard);
	DrawBoard(pcMainWnd);
	delete penBoard;
}