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

    static unsigned int __stdcall executeLauncher(void* args);
    void ThreadProc(void);
    void OnRcvMsg(OTHELLO_MSG_ID msg, WORD param1, DWORD param2);

public:
    GamePlayer(DISC enDiscCol, FuncPutDisc funcPutDisc);
    virtual void PlayNextTurn(const BOARD_INFO penBoardInfo);
    DISC GetDiscCol(void);
};

