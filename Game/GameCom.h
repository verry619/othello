#pragma once

#include "Cmn.h"
#include "CmnCom.h"
#include "GamePlayerHuman.h"

class CallbackFuncs
{
    using FuncGameStart = std::function<void(unsigned int unRowNum, unsigned int unColNum, GAME_SETTING enSetting)>;
    using FuncGameQuit = std::function<void(void)>;
    using FuncPutDisc = std::function<void(DISC enDiscCol, char cRow, char cCol)>;
public:
    FuncGameStart funcGameStart;
    FuncGameQuit funcGameQuit;
    void* funcGameEnd;
    void* funcPassTurn;
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
    void SendMsg(OTHELLO_THREAD_ID enDst, OTHELLO_MSG enMsg);
    void RcvMsg(char* pcBuf, unsigned int unBufLen);
    void UpdateBoard(DISC* m_penBoard, unsigned int unSize);
};

