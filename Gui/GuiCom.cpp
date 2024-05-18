#include "GuiCom.h"
#include "CmnLog.h"

GuiCom::GuiCom(GuiComCallbackFuncs funcs)
	:m_callbacks(funcs)
{
	if (!CmnCom::Initialize(std::bind(&GuiCom::RcvMsg, this, std::placeholders::_1, std::placeholders::_2)))
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "INITIALIZE ERROR!");
		return;
	}
	if (!CmnCom::StartCom(SOCKET_ROLE::CLIENT))
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "START COM ERROR!");
		return;
	}
}

void GuiCom::SendMsg(OTHELLO_PROCESS_ID enDst, OTHELLO_MSG enMsg)
{
	char* pcBuf = NULL;
	if (OTHELLO_PROCESS_ID::GAME == enDst)
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

void GuiCom::RcvMsg(const char* pcBuf, unsigned int unBufLen)
{
	OTHELLO_MSG enMsg;
	CmnCom::ConvCbufToMsg(pcBuf, &enMsg);

	switch (enMsg.enId)
	{
	case OTHELLO_MSG_ID::COM_START:
		if (SOCKET_ROLE::SERVER == static_cast<SOCKET_ROLE>(enMsg.p1))
		{
			WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "COM START");
			m_callbacks.funcComStart(true);
		}
		break;
	case OTHELLO_MSG_ID::GAME_START:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_START);
		m_callbacks.funcGameStart(true);
		break;
	case OTHELLO_MSG_ID::GAME_QUIT:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_QUIT);
		{
			OTHELLO_LOG_PARAM p = { static_cast<unsigned short>(GAME_RESULT::NO_CONTEST),0,0,0 };
			WRITE_DEV_LOG(OTHELLO_LOG_ID::GAME_RESULT, p);
		}
		break;
	case OTHELLO_MSG_ID::GAME_END:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::GAME_END);
		m_callbacks.funcGameEnd();
		break;
	case OTHELLO_MSG_ID::PUT_DISC:
	{
		OTHELLO_LOG_PARAM p = { enMsg.p1,enMsg.p2,0,0 };
		WRITE_DEV_LOG(OTHELLO_LOG_ID::PUT_DISC, p);

		m_callbacks.funcPutDisc(enMsg.p1, static_cast<DISC>(enMsg.p2));
	}
	break;
	case OTHELLO_MSG_ID::PASS_TURN:
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::PASS_TURN);

		m_callbacks.funcPassTurn(static_cast<DISC>(enMsg.p1));
		break;
	default:
		break;
	}
}

DISC* GuiCom::FetchCurrentBoard(void)
{
	CmnCom::ShmData* pcShm;
	pcShm = new CmnCom::ShmData;

	if (nullptr == pcShm)
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "SHM ERROR!");
		delete pcShm;
		return nullptr;
	}

	CmnCom::ReadShm(pcShm);

	unsigned short arraySize = pcShm->enBoardSize.ucRow * pcShm->enBoardSize.ucCol;
	DISC* penRet = new DISC[arraySize];
	memcpy(penRet, pcShm->enBoard, sizeof(DISC) * arraySize);

	delete pcShm;

	return penRet;
}

void GuiCom::UpdateBoardSize(unsigned char ucRow, unsigned char ucCol)
{
	CmnCom::ShmData* pcShm;
	pcShm = new CmnCom::ShmData;

	if (nullptr == pcShm)
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "SHM ERROR!");
		delete pcShm;
		return;
	}

	CmnCom::ReadShm(pcShm);

	pcShm->enBoardSize.ucRow = ucRow;
	pcShm->enBoardSize.ucCol = ucCol;

	CmnCom::WriteShm(pcShm);

	delete pcShm;
}