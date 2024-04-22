#pragma once

#include "Cmn.h"
#include <windows.h> 

class GamePlayer
{
    enum class GAME_PLAYER_STATE {
        MY_TURN,
        WAITING_TURN
    };

private:
    DISC m_enMyDiscCol;
    GAME_PLAYER_STATE m_enState;
    unsigned int m_unThreadId;

    static unsigned int __stdcall executeLauncher(void* args);
    void ThreadProc(void);
    void OnRcvMsg(OTHELLO_MSG_ID msg, WORD param1, DWORD param2);

public:
    GamePlayer(DISC enDiscCol);
    virtual void PlayNextTurn(const DISC* penBoard);
    DISC GetDiscCol(void);
};

