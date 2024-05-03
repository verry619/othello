#pragma once

#include "Cmn.h"

#include <windows.h> 
#include <functional>

class GamePlayer
{
protected:
	using FuncPutDisc = std::function<void(DISC_MOVE enDiscMove)>;
	void CallbackToClient(DISC_MOVE enDiscMove);

private:
	enum class GAME_PLAYER_STATE {
		MY_TURN,
		WAITING_TURN
	};

	DISC m_enMyDiscCol;
	GAME_PLAYER_STATE m_enState;
	unsigned int m_unThreadId;
	FuncPutDisc m_funcPutDisc;
	BOARD_INFO m_enBoardInfo;

	static unsigned int __stdcall executeLauncher(void* args);
	void ThreadProc(void);
	virtual void PlayMyTurn(BOARD_INFO enBoardInfo);

public:
	GamePlayer(DISC enDiscCol, FuncPutDisc funcPutDisc);
	virtual void PlayNextTurn(BOARD_INFO enBoardInfo);
	DISC GetDiscCol(void);
};

