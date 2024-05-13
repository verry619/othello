#pragma once
#include "GuiState.h"
class GuiStateInit :
	public GuiState
{
public:
	GuiStateInit(std::vector<std::vector<DISC>>& vv) :GuiState(vv) {};
	void ComStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
	void GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
};