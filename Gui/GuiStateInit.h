#pragma once
#include "GuiState.h"
class GuiStateInit :
	public GuiState
{
public:
	GuiStateInit(GuiBoardVV*& vv) :GuiState(vv) {};
	void ComStartComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
};