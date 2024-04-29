#include "GameCtrl.h"
#include "GameRule.h"

#include "CmnLog.h"

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
	(void)_beginthreadex(NULL, 0, &GameCtrl::executeLauncher, this, 0, &m_unThreadId);
}

unsigned int __stdcall GameCtrl::executeLauncher(void* args)
{
	reinterpret_cast<GameCtrl*>(args)->ThreadProc();
	return 0;
}

void GameCtrl::ThreadProc(void)
{
	CallbackFuncs* funcs;
	funcs = new CallbackFuncs();
	funcs->funcGameStart = std::bind(&GameCtrl::StartGame, this, std::placeholders::_1, std::placeholders::_2);
	funcs->funcGameQuit = std::bind(&GameCtrl::QuitGame, this);
	funcs->funcPutDisc = std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1);

	m_pcCom = new GameCom(*funcs);

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
		QuitGame_Internal();
		break;
	case OTHELLO_MSG_ID::PUT_DISC:
		PutDisc_Internal(
			static_cast<DISC>(param1),
			static_cast<unsigned char>((param2 >> 8) & 0x00ff),
			static_cast<unsigned char>(param2 & 0x00ff)
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
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START, "BOARD INIT ERROR!");
		return;
	}

	BOARD_INFO enBoard;
	enBoard.penDiscs = m_penBoard;
	enBoard.enSize.ucRow = m_unRowNum;
	enBoard.enSize.ucCol = m_unColNum;

	GameRule::InitializeBoard(enBoard);
	m_pcCom->UpdateBoard(enBoard);

	if ((GAME_SETTING::HUMAN_HUMAN == m_enSetting) || (GAME_SETTING::HUMAN_CPU == m_enSetting))
	{
		m_pcPlayerBlack = new GamePlayerHuman(DISC::BLACK,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));
		m_pcCom->SetUiListener(dynamic_cast<GamePlayerHuman*>(m_pcPlayerBlack));
	}

	if ((GAME_SETTING::HUMAN_HUMAN == m_enSetting) || (GAME_SETTING::CPU_HUMAN == m_enSetting))
	{
		m_pcPlayerWhite = new GamePlayerHuman(DISC::WHITE,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));
		m_pcCom->SetUiListener(dynamic_cast<GamePlayerHuman*>(m_pcPlayerWhite));
	}

	m_pcPlayerBlack->PlayNextTurn(enBoard);

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::GAME_START;

	m_pcCom->SendMsg(OTHELLO_PROCESS_ID::GUI, msg);

	m_enState = GAME_CTRL_STATE::IDLE;
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

	if (GameRule::FlipDiscs(enDiscMove, enBoard))
	{
		m_pcCom->UpdateBoard(enBoard);

		if (DISC::BLACK == enDiscCol)
		{
			m_pcPlayerWhite->PlayNextTurn(enBoard);
		}
		else if (DISC::WHITE == enDiscCol)
		{
			m_pcPlayerBlack->PlayNextTurn(enBoard);
		}
		else
		{
			WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC, "PARAM ERROR!");
		}

		OTHELLO_MSG msg;
		msg.enId = OTHELLO_MSG_ID::PUT_DISC;
		msg.p1 = O_SUCCESS;
		m_pcCom->SendMsg(OTHELLO_PROCESS_ID::GUI, msg);
	}
	else
	{
		/* Invalid position to put disc */
		if (DISC::BLACK == enDiscCol)
		{
			m_pcPlayerBlack->PlayNextTurn(enBoard);
		}
		else if (DISC::WHITE == enDiscCol)
		{
			m_pcPlayerWhite->PlayNextTurn(enBoard);
		}
		else
		{
			WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC, "PARAM ERROR!");
		}

		OTHELLO_MSG msg;
		msg.enId = OTHELLO_MSG_ID::PUT_DISC;
		msg.p1 = O_FAILURE;
		m_pcCom->SendMsg(OTHELLO_PROCESS_ID::GUI, msg);
	}

	CmnLog::getInstance().WriteGameLog(enDiscMove, enBoard);

	m_enState = GAME_CTRL_STATE::IDLE;
}

void GameCtrl::StartGame(BOARD_SIZE enBoardSize, GAME_SETTING enSetting)
{
	if (GAME_CTRL_STATE::IDLE != m_enState)
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START, "GAME CTRL STATE BUSY!");
		return;
	}

	m_unRowNum = enBoardSize.ucRow;
	m_unColNum = enBoardSize.ucCol;
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

void GameCtrl::PutDisc(DISC_MOVE enDiscMove)
{
	WPARAM wParam = static_cast<WPARAM>(enDiscMove.enColor);
	LPARAM lParam =
		((static_cast<LPARAM>(enDiscMove.enPos.ucRow) << 8) & 0xff00) |
		(static_cast<LPARAM>(enDiscMove.enPos.ucCol) & 0x00ff);

	PostThreadMessage(m_unThreadId,
		static_cast<DWORD>(OTHELLO_MSG_ID::PUT_DISC),
		wParam,
		lParam);
	m_enState = GAME_CTRL_STATE::WAITING;
}