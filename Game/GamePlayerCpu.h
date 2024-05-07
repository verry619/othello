#pragma once

#include "GamePlayer.h"

class GamePlayerCpu :
    public GamePlayer
{
private:
    void PlayMyTurn(const BoardInfo* pcBoardInfo) override;
public:
    GamePlayerCpu(DISC enDiscCol, FuncPutDisc funcPutDisc);
    ~GamePlayerCpu(void);
    void PlayNextTurn(const BoardInfo* pcBoardInfo) override;
};

