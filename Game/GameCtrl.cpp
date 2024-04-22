#include "GameCtrl.h"
#include "GameRule.h"

GameCtrl::GameCtrl(void)
	:m_enState(GAME_CTRL_STATE::IDLE),
	m_unRowNum(0),
	m_unColNum(0),
	m_penBoard(nullptr),
	m_enSetting(GAME_SETTING::HUMAN_HUMAN),
	m_pcCom(nullptr),
	m_pcPlayerBlack(nullptr),
	m_pcPlayerWhite(nullptr),
	m_unThreadId(0)
{
	CallbackFuncs* funcs;
	funcs = new CallbackFuncs();
	funcs->funcGameStart = std::bind(&GameCtrl::StartGame, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	funcs->funcGameQuit = std::bind(&GameCtrl::QuitGame, this);
	funcs->funcGameEnd = NULL;
	funcs->funcPassTurn = NULL;
	funcs->funcPutDisc = std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	m_pcCom = new GameCom(*funcs);

	(void)_beginthreadex(NULL, 0, &GameCtrl::executeLauncher, this, 0, &m_unThreadId);
}

unsigned int __stdcall GameCtrl::executeLauncher(void* args)
{
	reinterpret_cast<GameCtrl*>(args)->ThreadProc();
	return 0;
}

void GameCtrl::ThreadProc(void)
{
	MSG msg;

	while (1)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			OnRcvMsg(
				static_cast<OTHELLO_MSG_ID>(msg.message),
				static_cast<WORD>(msg.wParam),
				static_cast<DWORD>(msg.lParam));
		}
	}

	_endthreadex(0);
	return;
}

void GameCtrl::OnRcvMsg(OTHELLO_MSG_ID msg, WORD param1, DWORD param2)
{
	switch (msg)
	{
	case OTHELLO_MSG_ID::GAME_START:
		StartGame_Internal();
		break;
	case OTHELLO_MSG_ID::GAME_QUIT:
		break;
	case OTHELLO_MSG_ID::GAME_END:
		break;
	case OTHELLO_MSG_ID::PASS_TURN:
		break;
	case OTHELLO_MSG_ID::PUT_DISC:
		PutDisc_Internal(
			static_cast<DISC>(param1),
			static_cast<char>((param2 >> 8) & 0x00ff),
			static_cast<char>(param2 & 0x00ff)
		);
		break;
	default:
		break;
	}
}

void GameCtrl::StartGame_Internal(void)
{
	unsigned int unBoardSize = static_cast<unsigned int>(m_unRowNum) * static_cast<unsigned int>(m_unColNum);
	m_penBoard = static_cast<DISC*>(malloc(sizeof(DISC) * unBoardSize));
	if (NULL == m_penBoard)
	{
		/* エラーログ残す */
		return;
	}

	BOARD_INFO enBoard;
	enBoard.penDiscs = m_penBoard;
	enBoard.enSize.ucRow = m_unRowNum;
	enBoard.enSize.ucCol = m_unColNum;

	GameRule::InitializeBoard(enBoard);
	m_pcCom->UpdateBoard(m_penBoard, unBoardSize);

	if ((GAME_SETTING::HUMAN_HUMAN == m_enSetting) || (GAME_SETTING::HUMAN_CPU == m_enSetting))
	{
		m_pcPlayerBlack = new GamePlayerHuman(DISC::BLACK);
	}
	else if ((GAME_SETTING::HUMAN_HUMAN == m_enSetting) || (GAME_SETTING::CPU_HUMAN == m_enSetting))
	{
		m_pcPlayerWhite = new GamePlayerHuman(DISC::WHITE);
	}

	m_enState = GAME_CTRL_STATE::IDLE;


	/* TBD for unit test */
#if 0
	BOARD_INFO enBoard_test;
	DISC enDiscs_test[6][6] = {
		{DISC::NONE, DISC::WHITE, DISC::WHITE, DISC::WHITE, DISC::BLACK, DISC::NONE},
		{DISC::WHITE, DISC::WHITE, DISC::WHITE, DISC::WHITE, DISC::NONE, DISC::NONE},
		{DISC::WHITE, DISC::WHITE, DISC::WHITE, DISC::WHITE, DISC::NONE, DISC::NONE},
		{DISC::NONE, DISC::WHITE, DISC::WHITE, DISC::BLACK, DISC::BLACK, DISC::NONE},
		{DISC::NONE, DISC::WHITE, DISC::WHITE, DISC::WHITE, DISC::NONE, DISC::NONE},
		{DISC::NONE, DISC::BLACK, DISC::WHITE, DISC::WHITE, DISC::NONE, DISC::NONE}
	};
	DISC_MOVE enDiscMove_test;

	enBoard_test.enSize = { 6,6 };
	enBoard_test.penDiscs = &enDiscs_test[0][0];

	enDiscMove_test.enColor = DISC::BLACK;
	enDiscMove_test.enPos = { 0,0 };
	GameRule::FlipDiscs(enDiscMove_test, enBoard_test);
#endif

}

void GameCtrl::QuitGame_Internal(void)
{
	free(m_penBoard);
}

void GameCtrl::PutDisc_Internal(DISC enDiscCol, unsigned char ucRow, unsigned char ucCol)
{
	DISC_MOVE enDiscMove;
	BOARD_INFO enBoard;

	enDiscMove.enColor = enDiscCol;
	enDiscMove.enPos.ucRow = ucRow;
	enDiscMove.enPos.ucCol = ucCol;

	enBoard.penDiscs = m_penBoard;
	enBoard.enSize.ucRow = m_unRowNum;
	enBoard.enSize.ucCol = m_unColNum;

	GameRule::FlipDiscs(enDiscMove, enBoard);
	m_pcCom->UpdateBoard(m_penBoard,
		sizeof(DISC)*static_cast<unsigned int>(m_unRowNum) * static_cast<unsigned int>(m_unColNum));
}

void GameCtrl::StartGame(unsigned int unRowNum, unsigned int unColNum, GAME_SETTING enSetting)
{
	if (GAME_CTRL_STATE::IDLE != m_enState)
	{
		/* エラーログ残す */
		return;
	}

	m_unRowNum = unRowNum;
	m_unColNum = unColNum;
	m_enSetting = enSetting;

	PostThreadMessage(m_unThreadId,
		static_cast<DWORD>(OTHELLO_MSG_ID::GAME_START),
		0,
		0);

	m_enState = GAME_CTRL_STATE::WAITING;
}

void GameCtrl::QuitGame(void)
{
	PostThreadMessage(m_unThreadId,
		static_cast<DWORD>(OTHELLO_MSG_ID::GAME_QUIT),
		0,
		0);
}

void GameCtrl::PutDisc(DISC enDiscCol, unsigned char ucRow, unsigned char ucCol)
{
	WPARAM wParam = static_cast<WPARAM>(enDiscCol);
	LPARAM lParam =
		((static_cast<LPARAM>(ucRow) << 8) & 0xff00) |
		(static_cast<LPARAM>(ucCol) & 0x00ff);

	PostThreadMessage(m_unThreadId,
		static_cast<DWORD>(OTHELLO_MSG_ID::PUT_DISC),
		wParam,
		lParam);
}