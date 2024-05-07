#pragma once

#include "Cmn.h"
#include "CmnCom.h"
#include "GamePlayerHuman.h"

class CallbackFuncs
{
    using FuncGameStart = std::function<void(BOARD_SIZE enBoardSize, GAME_SETTING enSetting)>;
    using FuncGameQuit = std::function<void(void)>;
    using FuncPutDisc = std::function<void(DISC_MOVE enDiscMove)>;
public:
    FuncGameStart funcGameStart;
    FuncGameQuit funcGameQuit;
    FuncPutDisc funcPutDisc;
};

class GameCom
{
private:
    GamePlayerHuman* m_pcUiListenerBlack;
    GamePlayerHuman* m_pcUiListenerWhite;
    CallbackFuncs m_callbacks;

public:
    GameCom(CallbackFuncs funcs);
    void SetUiListener(GamePlayerHuman* pcHuman);
    void SendMsg(OTHELLO_PROCESS_ID enDst, OTHELLO_MSG enMsg);
    void RcvMsg(const char* pcBuf, unsigned int unBufLen);
    void UpdateBoard(const BoardInfo* const penBoardInfo);
};

