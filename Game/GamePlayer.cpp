#include "GamePlayer.h"

#include <chrono>
#include <thread>

GamePlayer::GamePlayer(DISC enDiscCol, FuncPutDisc funcPutDisc)
	:m_enMyDiscCol(enDiscCol),
	m_enState(GAME_PLAYER_STATE::WAITING_TURN),
	m_unThreadId(0),
	m_funcPutDisc(funcPutDisc),
	m_pcBoardInfo(nullptr)
{

	(void)_beginthreadex(NULL, 0, &GamePlayer::executeLauncher, this, 0, &m_unThreadId);
}

GamePlayer::~GamePlayer(void)
{
	bThreadKill = true;
}

unsigned int __stdcall GamePlayer::executeLauncher(void* args)
{
	reinterpret_cast<GamePlayer*>(args)->ThreadProc();
	return 0;
}

void GamePlayer::CallbackToClient(DISC_MOVE enDiscMove)
{
	if (GAME_PLAYER_STATE::MY_TURN == m_enState)
	{
		m_enState = GAME_PLAYER_STATE::WAITING_TURN;
		m_funcPutDisc(enDiscMove);
	}
}

void GamePlayer::PlayMyTurn(const BoardInfo* enBoardInfo)
{

}

void GamePlayer::ThreadProc(void)
{
	while (!bThreadKill)
	{
		if (GAME_PLAYER_STATE::MY_TURN == m_enState)
		{
			if (nullptr == m_pcBoardInfo)
			{
				break;
			}
			PlayMyTurn(m_pcBoardInfo);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	_endthreadex(0);

	return;
}

void GamePlayer::PlayNextTurn(const BoardInfo* pcBoardInfo)
{
	if (GAME_PLAYER_STATE::WAITING_TURN == m_enState)
	{
		m_pcBoardInfo = pcBoardInfo;
		m_enState = GAME_PLAYER_STATE::MY_TURN;
	}
}

DISC GamePlayer::GetDiscCol(void)
{
	return m_enMyDiscCol;
}
