#pragma once
#include "GuiState.h"
class GuiStateInGameBusy :
    public GuiState
{
public:
    GuiStateInGameBusy(GuiBoardVV*& vv) :GuiState(vv) {};
    void PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes, DISC enDiscCol) override;
    void GameStart(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
};