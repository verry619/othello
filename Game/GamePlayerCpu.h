#pragma once

#include "GamePlayer.h"

class GamePlayerCpu :
    public GamePlayer
{
private:
    void PlayMyTurn(BOARD_INFO enBoardInfo) override;
public:
    GamePlayerCpu(DISC enDiscCol, FuncPutDisc funcPutDisc);
    ~GamePlayerCpu(void);
    void PlayNextTurn(BOARD_INFO enBoardInfo) override;
};

