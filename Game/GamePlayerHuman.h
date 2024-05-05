#pragma once

#include "GamePlayer.h"

class GamePlayerHuman :
	public GamePlayer
{
public:
	GamePlayerHuman(DISC enDiscCol, FuncPutDisc funcPutDisc);
	~GamePlayerHuman(void);
	void PlayNextTurn(BOARD_INFO enBoardInfo) override;

	void ListenHumanInput(DISC_MOVE enDiscMove);
};

