#pragma once

#include "Cmn.h"
#include "CmnCom.h"

class GuiComCallbackFuncs
{
	using FuncComStart = std::function<void(bool bResult)>;
	using FuncGameStart = std::function<void(bool bResult)>;
	using FuncGameQuit = std::function<void(void)>;
	using FuncPutDisc = std::function<void(bool bResult, DISC enDiscCol)>;
public:
	FuncComStart funcComStart;
	FuncGameStart funcGameStart;
	FuncGameQuit funcGameQuit;
	FuncPutDisc funcPutDisc;
};

class GuiCom
{
private:
	GuiComCallbackFuncs m_callbacks;
public:
	GuiCom(GuiComCallbackFuncs funcs);
	void SendMsg(OTHELLO_PROCESS_ID enDst, OTHELLO_MSG enMsg);
	void RcvMsg(const char* pcBuf, unsigned int unBufLen);
	DISC* FetchCurrentBoard(void);
	void UpdateBoardSize(unsigned char ucRow, unsigned char ucCol);
};

