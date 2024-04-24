#include "GameCom.h"
#include "CmnCom.h"

GameCom::GameCom(CallbackFuncs funcs)
	:m_pcUiListenerBlack(NULL),
	m_pcUiListenerWhite(NULL),
	m_callbacks(funcs)
{
	if (!CmnCom::Initialize(std::bind(&GameCom::RcvMsg, this, std::placeholders::_1, std::placeholders::_2)))
	{
		/* エラーログを残す */
		return;
	}
	if (!CmnCom::StartCom(SOCKET_ROLE::SERVER))
	{
		/* エラーログを残す */
		return;
	}
}

void GameCom::SetUiListener(GamePlayerHuman* pcHuman)
{
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
		/* エラーログを残す */
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
		if (NULL != m_callbacks.funcGameStart)
		{
			BOARD_SIZE enBoardSize;
			enBoardSize.ucRow = static_cast<unsigned char>(enMsg.p1);
			enBoardSize.ucCol = static_cast<unsigned char>(enMsg.p2);
			m_callbacks.funcGameStart(enBoardSize, static_cast<GAME_SETTING>(enMsg.p3));
		}
		break;

	case OTHELLO_MSG_ID::PUT_DISC:
#if 0 /* GameCom -> GameCtrl For Debug */
		if (NULL != m_callbacks.funcPutDisc)
		{
			DISC_MOVE enDiscMove;
			enDiscMove.enColor = static_cast<DISC>(enMsg.p1);
			enDiscMove.enPos.ucRow = static_cast<unsigned char>(enMsg.p2);
			enDiscMove.enPos.ucCol = static_cast<unsigned char>(enMsg.p3);
			m_callbacks.funcPutDisc(enDiscMove);
		}
#endif
		DISC_MOVE enDiscMove;
		enDiscMove.enColor = static_cast<DISC>(enMsg.p1);
		enDiscMove.enPos.ucRow = static_cast<unsigned char>(enMsg.p2);
		enDiscMove.enPos.ucCol = static_cast<unsigned char>(enMsg.p3);

		if (DISC::BLACK == enDiscMove.enColor)
		{
			m_pcUiListenerBlack->ListenHumanInput(enDiscMove);
		}
		else if (DISC::WHITE == enDiscMove.enColor)
		{
			m_pcUiListenerWhite->ListenHumanInput(enDiscMove);
		}
		else
		{
			/* エラーログ残す */
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
		/* エラーログ残す */
		return;
	}

	CmnCom::ReadShm(penShm);
	memcpy(penShm->enBoard, enBoardInfo.penDiscs,
		sizeof(DISC) * enBoardInfo.enSize.ucRow * enBoardInfo.enSize.ucCol);
	CmnCom::WriteShm(penShm);

	free(penShm);
}