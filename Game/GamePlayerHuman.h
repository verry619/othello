#pragma once

#include "GamePlayer.h"

class GamePlayerHuman :
    public GamePlayer
{
public:
    GamePlayerHuman(DISC enDiscCol, FuncPutDisc funcPutDisc);
    void PlayNextTurn(const BOARD_INFO penBoardInfo) override;

    void ListenHumanInput(DISC_MOVE enDiscMove);
};

