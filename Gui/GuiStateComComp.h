#pragma once
#include "GuiState.h"
class GuiStateComComp :
	public GuiState
{
public:
	GuiStateComComp(GuiBoardVV*& vv) :GuiState(vv) {};
	void ComStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
	void GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
};

