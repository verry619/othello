#include "GameCtrl.h"
#include "GameRule.h"
#include "GamePlayerHuman.h"
#include "GamePlayerCpu.h"

#include "CmnLog.h"

GameCtrl::GameCtrl(void)
	:m_enState(GAME_CTRL_STATE::IDLE),
	m_BoardInfo({ {0,0},nullptr }),
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
	unsigned int unBoardSize = static_cast<unsigned int>(m_BoardInfo.enSize.ucRow) * static_cast<unsigned int>(m_BoardInfo.enSize.ucCol);
	m_BoardInfo.penDiscs = static_cast<DISC*>(malloc(sizeof(DISC) * unBoardSize));
	if (nullptr == m_BoardInfo.penDiscs)
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START, "BOARD INIT ERROR!");
		return;
	}

	GameRule::InitializeBoard(m_BoardInfo);
	m_pcCom->UpdateBoard(m_BoardInfo);
	InitializePlayerSetting();

	CmnLog::getInstance().CreateNewDevLogFile();

	m_pcPlayerBlack->PlayNextTurn(m_BoardInfo);

	SendMsgToGui(OTHELLO_MSG_ID::GAME_START, 0, 0, 0, 0);

	m_enState = GAME_CTRL_STATE::IDLE;
}

void GameCtrl::InitializePlayerSetting(void)
{
	switch (m_enSetting)
	{
	case GAME_SETTING::HUMAN_HUMAN:
		m_pcPlayerBlack = new GamePlayerHuman(DISC::BLACK,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));
		m_pcCom->SetUiListener(dynamic_cast<GamePlayerHuman*>(m_pcPlayerBlack));

		m_pcPlayerWhite = new GamePlayerHuman(DISC::WHITE,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));
		m_pcCom->SetUiListener(dynamic_cast<GamePlayerHuman*>(m_pcPlayerWhite));

		break;

	case GAME_SETTING::HUMAN_CPU:
		m_pcPlayerBlack = new GamePlayerHuman(DISC::BLACK,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));
		m_pcCom->SetUiListener(dynamic_cast<GamePlayerHuman*>(m_pcPlayerBlack));

		m_pcPlayerWhite = new GamePlayerCpu(DISC::WHITE,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));

		break;

	case GAME_SETTING::CPU_HUMAN:
		m_pcPlayerBlack = new GamePlayerCpu(DISC::BLACK,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));

		m_pcPlayerWhite = new GamePlayerHuman(DISC::WHITE,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));
		m_pcCom->SetUiListener(dynamic_cast<GamePlayerHuman*>(m_pcPlayerWhite));

		break;

	case GAME_SETTING::CPU_CPU:
		m_pcPlayerBlack = new GamePlayerCpu(DISC::BLACK,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));

		m_pcPlayerWhite = new GamePlayerCpu(DISC::WHITE,
			std::bind(&GameCtrl::PutDisc, this, std::placeholders::_1));

		break;

	default:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START, "GAME SETTING ERROR!");
		break;
	}
}

void GameCtrl::QuitGame_Internal(void)
{
	m_pcCom->SetUiListener(NULL);

	if (nullptr != m_pcPlayerBlack)
	{
		delete m_pcPlayerBlack;
	}

	if (nullptr != m_pcPlayerWhite)
	{
		delete m_pcPlayerWhite;
	}

	free(m_BoardInfo.penDiscs);

	SendMsgToGui(OTHELLO_MSG_ID::GAME_QUIT, 0, 0, 0, 0);

	m_enState = GAME_CTRL_STATE::IDLE;
}

void GameCtrl::PutDisc_Internal(DISC enDiscCol, unsigned char ucRow, unsigned char ucCol)
{
	DISC_MOVE enDiscMove = { enDiscCol,{ucRow,ucCol} };

	if (!GameRule::FlipDiscs(enDiscMove, m_BoardInfo))
	{
		/* Invalid position to put disc */
		SendMsgToGui(OTHELLO_MSG_ID::PUT_DISC, O_FAILURE, 0, 0, 0);

		ContinuePlayerTurn(enDiscCol, m_BoardInfo);

		m_enState = GAME_CTRL_STATE::IDLE;
		return;
	}

	m_pcCom->UpdateBoard(m_BoardInfo);

	SendMsgToGui(OTHELLO_MSG_ID::PUT_DISC, O_SUCCESS, 0, 0, 0);

	CmnLog::getInstance().WriteGameLog(enDiscMove, m_BoardInfo);

	DecideNextTurn(enDiscCol, m_BoardInfo);

	m_enState = GAME_CTRL_STATE::IDLE;
	return;
}

void GameCtrl::ContinuePlayerTurn(DISC enDiscCol, BOARD_INFO enBoard)
{
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
}

void GameCtrl::DecideNextTurn(DISC enDiscCol, BOARD_INFO enBoard)
{
	if (DISC::BLACK == enDiscCol)
	{
		if (GameRule::CanMoveStone(DISC::WHITE, enBoard))
		{
			m_pcPlayerWhite->PlayNextTurn(enBoard);
		}
		else
		{
			if (GameRule::CanMoveStone(DISC::BLACK, enBoard))
			{
				SendMsgToGui(OTHELLO_MSG_ID::PASS_TURN, 0, 0, 0, 0);

				m_pcPlayerBlack->PlayNextTurn(enBoard);
			}
			else
			{
				SendMsgToGui(OTHELLO_MSG_ID::GAME_END, 0, 0, 0, 0);
			}
		}

	}
	else if (DISC::WHITE == enDiscCol)
	{
		if (GameRule::CanMoveStone(DISC::BLACK, enBoard))
		{
			m_pcPlayerBlack->PlayNextTurn(enBoard);
		}
		else
		{
			if (GameRule::CanMoveStone(DISC::WHITE, enBoard))
			{
				SendMsgToGui(OTHELLO_MSG_ID::PASS_TURN, 0, 0, 0, 0);

				m_pcPlayerWhite->PlayNextTurn(enBoard);
			}
			else
			{
				SendMsgToGui(OTHELLO_MSG_ID::GAME_END, 0, 0, 0, 0);
			}
		}
	}
	else
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC, "PARAM ERROR!");
	}
}

void GameCtrl::SendMsgToGui(OTHELLO_MSG_ID enId, unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4)
{
	OTHELLO_MSG msg = { enId,p1, p2, p3, p4 };
	m_pcCom->SendMsg(OTHELLO_PROCESS_ID::GUI, msg);
}

void GameCtrl::StartGame(BOARD_SIZE enBoardSize, GAME_SETTING enSetting)
{
	if (GAME_CTRL_STATE::IDLE != m_enState)
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START, "GAME CTRL STATE BUSY!");
		return;
	}

	m_BoardInfo.enSize.ucRow = enBoardSize.ucRow;
	m_BoardInfo.enSize.ucCol = enBoardSize.ucCol;
	m_enSetting = enSetting;

	PostThreadMessage(m_unThreadId,
		static_cast<DWORD>(OTHELLO_MSG_ID::GAME_START),
		0,
		0);

	m_enState = GAME_CTRL_STATE::WAITING;
}

void GameCtrl::QuitGame(void)
{
	if (GAME_CTRL_STATE::IDLE != m_enState)
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_QUIT, "GAME CTRL STATE BUSY!");
		return;
	}

	PostThreadMessage(m_unThreadId,
		static_cast<DWORD>(OTHELLO_MSG_ID::GAME_QUIT),
		0,
		0);

	m_enState = GAME_CTRL_STATE::WAITING;
}

void GameCtrl::PutDisc(DISC_MOVE enDiscMove)
{
	if (GAME_CTRL_STATE::IDLE != m_enState)
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC, "GAME CTRL STATE BUSY!");
		return;
	}

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