#include "GamePlayerCpu.h"
#include "GameRule.h"

#include <cstdlib>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <random>


GamePlayerCpu::GamePlayerCpu(DISC enDiscCol, FuncPutDisc funcPutDisc)
	:GamePlayer(enDiscCol, funcPutDisc)
{
}

GamePlayerCpu::~GamePlayerCpu(void)
{
	GamePlayer::~GamePlayer();
}

void GamePlayerCpu::PlayMyTurn(BOARD_INFO enBoardInfo)
{
	std::vector<unsigned char> v(enBoardInfo.enSize.ucRow * enBoardInfo.enSize.ucCol);
	std::iota(v.begin(), v.end(), 0);

	unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(std::begin(v), std::end(v), std::default_random_engine(seed));

	DISC_MOVE enDiscMove = { GamePlayer::GetDiscCol(),{0,0} };

	for (const auto& idx : v)
	{
		enDiscMove.enPos.ucRow = idx / enBoardInfo.enSize.ucCol;
		enDiscMove.enPos.ucCol = idx % enBoardInfo.enSize.ucCol;

		if (GameRule::CheckFlip(enDiscMove, enBoardInfo))
		{
			break;
		}
	}

	/* TODO 置くマスがないパターンを考慮していない */
	GamePlayer::CallbackToClient(enDiscMove);
}

void GamePlayerCpu::PlayNextTurn(BOARD_INFO enBoardInfo)
{
	GamePlayer::PlayNextTurn(enBoardInfo);
}