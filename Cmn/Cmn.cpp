// Cmn.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "pch.h"
#include "framework.h"

#include "Cmn.h"

DISC CmnCountDiscs(BOARD_INFO enBoardInfo, unsigned int& unCountB, unsigned int& unCountW)
{
	unCountB = 0;
	unCountW = 0;

	for (unsigned char row = 0;row < enBoardInfo.enSize.ucRow;row++)
	{
		for (unsigned char col = 0;col < enBoardInfo.enSize.ucCol;col++)
		{
			unsigned int iPos = enBoardInfo.enSize.ucCol * row + col;
			if (DISC::BLACK == enBoardInfo.penDiscs[iPos])
			{
				unCountB++;
			}
			else if (DISC::WHITE == enBoardInfo.penDiscs[iPos])
			{
				unCountW++;
			}
		}
	}

	if (unCountB > unCountW)
	{
		return DISC::BLACK;
	}
	else if (unCountB < unCountW)
	{
		return DISC::WHITE;
	}
	else
	{
		return DISC::NONE;
	}
}