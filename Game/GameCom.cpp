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
	if (!CmnCom::StartCom(EN_SOCKET_ROLE::SERVER))
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

void GameCom::SendMsg(OTHELLO_THREAD_ID enDst, OTHELLO_MSG enMsg)
{
	char* pcBuf = NULL;
	if (OTHELLO_THREAD_ID::GUI == enDst)
	{
		pcBuf = (char*)malloc(sizeof(OTHELLO_MSG));
		if (NULL != pcBuf)
		{
			CmnConvMsgToCbuf(&enMsg, pcBuf);
			CmnCom::SendMsg(pcBuf, sizeof(OTHELLO_MSG));
			free(pcBuf);
		}
	}
}

void GameCom::RcvMsg(char* pcBuf, unsigned int unBufLen)
{
	OTHELLO_MSG enMsg;
	CmnConvCbufToMsg(pcBuf, &enMsg);

	switch (enMsg.enId)
	{
	case OTHELLO_MSG_ID::GAME_START:
		if (NULL != m_callbacks.funcGameStart)
		{
			m_callbacks.funcGameStart(enMsg.p1, enMsg.p2, static_cast<GAME_SETTING>(enMsg.p3));
		}
		break;

	case OTHELLO_MSG_ID::PUT_DISC:
		if (NULL != m_callbacks.funcPutDisc)
		{
			m_callbacks.funcPutDisc(static_cast<DISC>(enMsg.p1), enMsg.p2, enMsg.p3);
		}
		break;

	default:
		break;
	}
}

void GameCom::UpdateBoard(DISC* penBoard, unsigned int unSize)
{
	CmnCom::ShmData* penShm;
	penShm = (CmnCom::ShmData*)malloc(sizeof(CmnCom::ShmData));

	if (NULL == penShm)
	{
		/* エラーログ残す */
		return;
	}

	CmnCom::ReadShm(penShm);
	memcpy(penShm->enBoard, penBoard, unSize);
	CmnCom::WriteShm(penShm);

	free(penShm);
}