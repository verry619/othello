#pragma once

#include "Cmn.h"

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <functional>

#define WIN32_LEAN_AND_MEAN

enum class SOCKET_ROLE :unsigned char
{
	SERVER,
	CLIENT,
	NONE
};

class CmnCom
{
	using CallbackFunc = std::function<void(char* pcBuf, unsigned int unBufLen)>;

private:
	/* for shared memory */
	static HANDLE m_hMapFile;

	/* for socket */
	static SOCKET m_socket;
	static SOCKET_ROLE m_enRole;
	static CallbackFunc m_clbFunc_rcvMsg;

	static void CreateThreadForRcvMsg(void);
	static void ThreadProc(void);
	static void NotifyComStartComp(SOCKET_ROLE enRole);
	static void ReceiveMsg(void);
	static BOOL StartCom_Internal(SOCKET_ROLE enRole);

public:
	typedef struct ShmData_
	{
		BOARD_SIZE enBoardSize;
		DISC enBoard[DEF_ROW_MAX * DEF_COL_MAX];
		DISC_MOVE enHist[DEF_HIST_MAX];
		unsigned short idHistCurrent;
	}ShmData;

	static BOOL Initialize(CallbackFunc clbFunc_rcvMsg);
	static BOOL StartCom(SOCKET_ROLE enRole);
	static BOOL Finalize(void);
	static void ReadShm(ShmData* penShmDst);
	static void WriteShm(const ShmData* penShmSrc);
	static void SendMsg(const char* cBuf, unsigned int unLen);

	static void ConvMsgToCbuf(const OTHELLO_MSG* enMsg, char* pcBuf);
	static void ConvCbufToMsg(const char* pcBuf, OTHELLO_MSG* enMsg);
};

