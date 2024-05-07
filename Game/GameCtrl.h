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
	BoardInfo* m_pcBoardInfo;
	GAME_SETTING m_enSetting;
	GameCom* m_pcCom;
	GamePlayer* m_pcPlayerBlack;
	GamePlayer* m_pcPlayerWhite;
	unsigned int m_unThreadId;

	static unsigned int __stdcall executeLauncher(void* args);
	void ThreadProc(void);
	void OnRcvMsg(OTHELLO_MSG_ID msg, WORD param1, DWORD param2);
	void StartGame_Internal(GAME_SETTING enSetting, unsigned char ucRow, unsigned char ucCol);
	void InitializePlayerSetting(void);
	void QuitGame_Internal(void);
	void PreviousGameFinalize(void);
	void PutDisc_Internal(DISC enDiscCol, unsigned char ucRow, unsigned char ucCol);
	void ContinuePlayerTurn(DISC enDiscCol, const BoardInfo* enBoard);
	void DecideNextTurn(DISC enDiscCol, const BoardInfo* enBoard);
	void SendMsgToGui(OTHELLO_MSG_ID enId, unsigned short p1, unsigned short p2, unsigned short p3, unsigned short p4);

public:
	GameCtrl(void);
	void StartGame(BOARD_SIZE enBoardSize, GAME_SETTING enSetting);
	void QuitGame(void);
	void PutDisc(DISC_MOVE enDiscMove);
};