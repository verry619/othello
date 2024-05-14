#include "GuiStateComComp.h"

#include <iostream>

void GuiStateComComp::ComStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	std::cout << "GuiStateComComp::ComStartComp" << std::endl;
}

void GuiStateComComp::GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GameStart_Sub(pcCom, pcMainWnd);
}