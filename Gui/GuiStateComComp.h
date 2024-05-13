#pragma once
#include "GuiState.h"
class GuiStateComComp :
	public GuiState
{
public:
	GuiStateComComp(std::vector<std::vector<DISC>>& vv) :GuiState(vv) {};
	void ComStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
	void GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
};

