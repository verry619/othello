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

void GamePlayerCpu::PlayMyTurn(const BoardInfo* pcBoardInfo)
{
	std::vector<DISC_POS> v;
	DISC_POS enPosTmp = pcBoardInfo->GetFirstPos();

	do
	{
		v.push_back(enPosTmp);
		enPosTmp = pcBoardInfo->GetNextPos(enPosTmp);
	} while (
		(enPosTmp.ucRow != pcBoardInfo->GetFirstPos().ucRow) ||
		(enPosTmp.ucCol != pcBoardInfo->GetFirstPos().ucCol)
		);

	unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(std::begin(v), std::end(v), std::default_random_engine(seed));

	DISC_MOVE enDiscMove = { GamePlayer::GetDiscCol(),{0,0} };

	for (const auto& pos : v)
	{
		enDiscMove.enPos = pos;
		if (GameRule::CheckFlip(enDiscMove, pcBoardInfo))
		{
			break;
		}
	}

	/* TODO 置くマスがないパターンを考慮していない */
	GamePlayer::CallbackToClient(enDiscMove);
}

void GamePlayerCpu::PlayNextTurn(const BoardInfo* pcBoardInfo)
{
	GamePlayer::PlayNextTurn(pcBoardInfo);
}