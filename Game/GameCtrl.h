#pragma once

#include "Cmn.h"
#include "GameCom.h"
#include "GamePlayer.h"

enum class GAME_CTRL_STATE {
    IDLE,
    WAITING
};

class GameCtrl
{

private:
    GAME_CTRL_STATE m_enState;
    BOARD_INFO m_BoardInfo;
    GAME_SETTING m_enSetting;
    GameCom* m_pcCom;
    GamePlayer* m_pcPlayerBlack;
    GamePlayer* m_pcPlayerWhite;
    unsigned int m_unThreadId;

    static unsigned int __stdcall executeLauncher(void* args);
    void ThreadProc(void);
    void OnRcvMsg(OTHELLO_MSG_ID msg, WORD param1, DWORD param2);
    void StartGame_Internal(void);
    void InitializePlayerSetting(void);
    void QuitGame_Internal(void);
    void PutDisc_Internal(DISC enDiscCol, unsigned char ucRow, unsigned char ucCol);
    void ContinuePlayerTurn(DISC enDiscCol, BOARD_INFO enBoard);
    void DecideNextTurn(DISC enDiscCol, BOARD_INFO enBoard);
    void SendMsgToGui(OTHELLO_MSG_ID enId, unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4);

public:
    GameCtrl(void);
    void StartGame(BOARD_SIZE enBoardSize, GAME_SETTING enSetting);
    void QuitGame(void);
    void PutDisc(DISC_MOVE enDiscMove);
};