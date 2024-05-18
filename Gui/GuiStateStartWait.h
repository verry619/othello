#pragma once
#include "GuiState.h"
class GuiStateStartWait :
	public GuiState
{
public:
	GuiStateStartWait(GuiBoardVV*& vv) :GuiState(vv) {};
	void GameStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)override;
};

