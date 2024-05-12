#pragma once
#include "GuiState.h"
class GuiStateInit :
	public GuiState
{
public:
	void ComStartComp(void) override;
	void GameStart(void) override;
};