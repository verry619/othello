#pragma once
#include "GuiState.h"
class GuiStateGameResult :
    public GuiState
{
public:
	GuiStateGameResult(std::vector<std::vector<DISC>>& vv) :GuiState(vv) {};
};

