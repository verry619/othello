#pragma once

#include "GamePlayer.h"

class GamePlayerHuman :
	public GamePlayer
{
public:
	GamePlayerHuman(DISC enDiscCol, FuncPutDisc funcPutDisc);
	void PlayNextTurn(BOARD_INFO enBoardInfo) override;

	void ListenHumanInput(DISC_MOVE enDiscMove);
};

