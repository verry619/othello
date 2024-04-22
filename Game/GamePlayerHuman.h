#pragma once
#include "GamePlayer.h"
class GamePlayerHuman :
    public GamePlayer
{
public:
    GamePlayerHuman(DISC enDiscCol);
    void PlayNextTurn(const DISC* penBoard) override;

    void ListenHumanInput(DISC enDiscCol, char cRow, char cCol);
};

