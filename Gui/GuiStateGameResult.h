#pragma once
#include "GuiState.h"
class GuiStateGameResult :
    public GuiState
{
public:
	GuiStateGameResult(GuiBoardVV*& vv) :GuiState(vv) {};
	void GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
};

