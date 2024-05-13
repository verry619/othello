#pragma once
#include "GuiState.h"
class GuiStateStartWait :
	public GuiState
{
public:
	GuiStateStartWait(std::vector<std::vector<DISC>>& vv) :GuiState(vv) {};
	void GameStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)override;
};

