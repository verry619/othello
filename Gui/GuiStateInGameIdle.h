#pragma once
#include "GuiState.h"
class GuiStateInGameIdle :
	public GuiState
{
public:
	GuiStateInGameIdle(GuiBoardVV*& vv) :GuiState(vv) {};
	void PutDisc(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC_POS enDiscPos) override;
	void PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes, DISC enDiscCol) override;
	void GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
	void PassTurn(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, DISC enDiscCol) override;
	void GameEnd(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
};