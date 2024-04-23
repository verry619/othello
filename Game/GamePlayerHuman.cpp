#include "GamePlayerHuman.h"

GamePlayerHuman::GamePlayerHuman(DISC enDiscCol, FuncPutDisc funcPutDisc)
	:GamePlayer(enDiscCol, funcPutDisc)
{

}

void GamePlayerHuman::PlayNextTurn(const BOARD_INFO penBoardInfo)
{
	GamePlayer::PlayNextTurn(penBoardInfo);
}

void GamePlayerHuman::ListenHumanInput(DISC_MOVE enDiscMove)
{
	GamePlayer::CallbackToClient(enDiscMove);
}