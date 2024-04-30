#include "GameCom.h"
#include "CmnCom.h"
#include "CmnLog.h"

GameCom::GameCom(CallbackFuncs funcs)
	:m_pcUiListenerBlack(NULL),
	m_pcUiListenerWhite(NULL),
	m_callbacks(funcs)
{
	if (!CmnCom::Initialize(std::bind(&GameCom::RcvMsg, this, std::placeholders::_1, std::placeholders::_2)))
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "INITIALIZE ERROR!");
		return;
	}
	if (!CmnCom::StartCom(SOCKET_ROLE::SERVER))
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "START COM ERROR!");
		return;
	}
}

void GameCom::SetUiListener(GamePlayerHuman* pcHuman)
{
	if (NULL == pcHuman)
	{
		m_pcUiListenerBlack = NULL;
		m_pcUiListenerWhite = NULL;
		return;
	}

	if (DISC::BLACK == pcHuman->GetDiscCol())
	{
		m_pcUiListenerBlack = pcHuman;
	}
	else if (DISC::WHITE == pcHuman->GetDiscCol())
	{
		m_pcUiListenerWhite = pcHuman;
	}
	else
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START, "PARAM ERROR!");
		return;
	}

}

void GameCom::SendMsg(OTHELLO_PROCESS_ID enDst, OTHELLO_MSG enMsg)
{
	char* pcBuf = NULL;
	if (OTHELLO_PROCESS_ID::GUI == enDst)
	{
		pcBuf = (char*)malloc(sizeof(OTHELLO_MSG));
		if (NULL != pcBuf)
		{
			CmnCom::ConvMsgToCbuf(&enMsg, pcBuf);
			CmnCom::SendMsg(pcBuf, sizeof(OTHELLO_MSG));
			free(pcBuf);
		}
	}
}

void GameCom::RcvMsg(const char* pcBuf, unsigned int unBufLen)
{
	OTHELLO_MSG enMsg;
	CmnCom::ConvCbufToMsg(pcBuf, &enMsg);

	switch (enMsg.enId)
	{
	case OTHELLO_MSG_ID::GAME_START:
		if (NULL == m_callbacks.funcGameStart)
		{
			WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START, "CALLBACK SET ERROR!");
			return;
		}

		BOARD_SIZE enBoardSize;
		enBoardSize.ucRow = static_cast<unsigned char>(enMsg.p1);
		enBoardSize.ucCol = static_cast<unsigned char>(enMsg.p2);
		m_callbacks.funcGameStart(enBoardSize, static_cast<GAME_SETTING>(enMsg.p3));

		break;

	case OTHELLO_MSG_ID::GAME_QUIT:
		if (NULL == m_callbacks.funcGameQuit)
		{
			WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_QUIT, "CALLBACK SET ERROR!");
			return;
		}

		m_callbacks.funcGameQuit();

		break;


	case OTHELLO_MSG_ID::PUT_DISC:
		DISC_MOVE enDiscMove;
		enDiscMove.enColor = static_cast<DISC>(enMsg.p1);
		enDiscMove.enPos.ucRow = static_cast<unsigned char>(enMsg.p2);
		enDiscMove.enPos.ucCol = static_cast<unsigned char>(enMsg.p3);

		if (DISC::BLACK == enDiscMove.enColor)
		{
			if (NULL == m_pcUiListenerBlack)
			{
				WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC, "CALLBACK SET ERROR!");
				return;
			}
			m_pcUiListenerBlack->ListenHumanInput(enDiscMove);
		}
		else if (DISC::WHITE == enDiscMove.enColor)
		{
			if (NULL == m_pcUiListenerWhite)
			{
				WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC, "CALLBACK SET ERROR!");
				return;
			}
			m_pcUiListenerWhite->ListenHumanInput(enDiscMove);
		}
		else
		{
			WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PUT_DISC, "PARAM ERROR!");
		}

		break;

	default:
		break;
	}
}

void GameCom::UpdateBoard(BOARD_INFO enBoardInfo)
{
	CmnCom::ShmData* penShm;
	penShm = (CmnCom::ShmData*)malloc(sizeof(CmnCom::ShmData));

	if (NULL == penShm)
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "SHM ERROR!");
		return;
	}

	CmnCom::ReadShm(penShm);
	memcpy(penShm->enBoard, enBoardInfo.penDiscs,
		sizeof(DISC) * enBoardInfo.enSize.ucRow * enBoardInfo.enSize.ucCol);
	CmnCom::WriteShm(penShm);

	free(penShm);
}