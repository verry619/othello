#include "GamePlayerHuman.h"

GamePlayerHuman::GamePlayerHuman(DISC enDiscCol, FuncPutDisc funcPutDisc)
	:GamePlayer(enDiscCol, funcPutDisc)
{

}

GamePlayerHuman::~GamePlayerHuman(void)
{
	GamePlayer::~GamePlayer();
}

void GamePlayerHuman::PlayNextTurn(const BoardInfo* pcBoardInfo)
{
	GamePlayer::PlayNextTurn(pcBoardInfo);
}

void GamePlayerHuman::ListenHumanInput(DISC_MOVE enDiscMove)
{
	GamePlayer::CallbackToClient(enDiscMove);
}