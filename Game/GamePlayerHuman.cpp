#include "GamePlayerHuman.h"

GamePlayerHuman::GamePlayerHuman(DISC enDiscCol, FuncPutDisc funcPutDisc)
	:GamePlayer(enDiscCol, funcPutDisc)
{

}

void GamePlayerHuman::PlayNextTurn(BOARD_INFO enBoardInfo)
{
	GamePlayer::PlayNextTurn(enBoardInfo);
}

void GamePlayerHuman::ListenHumanInput(DISC_MOVE enDiscMove)
{
	GamePlayer::CallbackToClient(enDiscMove);
}