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
	const BoardInfo* m_pcBoardInfo;
	bool bThreadKill = false;
	bool bThreadKillComp = false;

	static unsigned int __stdcall executeLauncher(void* args);
	void ThreadProc(void);
	virtual void PlayMyTurn(const BoardInfo* pcBoardInfo);

public:
	GamePlayer(DISC enDiscCol, FuncPutDisc funcPutDisc);
	~GamePlayer(void);
	virtual void PlayNextTurn(const BoardInfo* pcBoardInfo);
	DISC GetDiscCol(void);
};

