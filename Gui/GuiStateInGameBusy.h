#pragma once
#include "GuiState.h"
class GuiStateInGameBusy :
    public GuiState
{
public:
    GuiStateInGameBusy(std::vector<std::vector<DISC>>& vv) :GuiState(vv) {};
    void PutDiscComp(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd) override;
};

