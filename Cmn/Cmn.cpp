// Cmn.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "pch.h"
#include "framework.h"

#include "Cmn.h"

void CmnConvMsgToCbuf(const OTHELLO_MSG* enMsg, char* pcBuf)
{
	pcBuf[0] = static_cast<char>((static_cast<unsigned int>(enMsg->enId)&0x00ff));
	pcBuf[1] = static_cast<char>(enMsg->p1 & 0x00ff);
	pcBuf[2] = static_cast<char>((enMsg->p1 >> 8) & 0x00ff);
	pcBuf[3] = static_cast<char>(enMsg->p2 & 0x00ff);
	pcBuf[4] = static_cast<char>((enMsg->p2 >> 8) & 0x00ff);
	pcBuf[5] = static_cast<char>(enMsg->p3 & 0x00ff);
	pcBuf[6] = static_cast<char>((enMsg->p3 >> 8) & 0x00ff);
	pcBuf[7] = static_cast<char>(enMsg->p4 & 0x00ff);
	pcBuf[8] = static_cast<char>((enMsg->p4 >> 8) & 0x00ff);
}

void CmnConvCbufToMsg(const char* pcBuf, OTHELLO_MSG* enMsg)
{
	enMsg->enId = static_cast<OTHELLO_MSG_ID>(pcBuf[0]);

	unsigned int unTmp = (static_cast<unsigned int>(pcBuf[1]) & 0x00ff) | ((static_cast<unsigned int>(pcBuf[2]) << 8) & 0xff00);
	enMsg->p1 = static_cast<unsigned int>(unTmp);

	unTmp = (static_cast<unsigned int>(pcBuf[3]) & 0x00ff) | ((static_cast<unsigned int>(pcBuf[4]) << 8) & 0xff00);
	enMsg->p2 = static_cast<unsigned int>(unTmp);

	unTmp = (static_cast<unsigned int>(pcBuf[5]) & 0x00ff) | ((static_cast<unsigned int>(pcBuf[6]) << 8) & 0xff00);
	enMsg->p3 = static_cast<unsigned int>(unTmp);

	unTmp = (static_cast<unsigned int>(pcBuf[7]) & 0x00ff) | ((static_cast<unsigned int>(pcBuf[8]) << 8) & 0xff00);
	enMsg->p4 = static_cast<unsigned int>(unTmp);
}
