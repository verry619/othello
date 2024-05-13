#pragma once
#include "GuiState.h"
class GuiStateInGameIdle :
	public GuiState
{
public:
	GuiStateInGameIdle(std::vector<std::vector<DISC>>& vv) :GuiState(vv) {};
	void PutDisc(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC_POS enDiscPos) override;
};

