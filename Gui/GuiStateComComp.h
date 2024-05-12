#pragma once
#include "GuiState.h"
class GuiStateComComp :
    public GuiState
{
public:
	void ComStartComp(void) override;
	void GameStart(void) override;
};

