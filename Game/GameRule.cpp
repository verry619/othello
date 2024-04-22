#include "GameRule.h"

#include <windows.h>
#include <stdlib.h>

void GameRule::InitializeBoard(BOARD_INFO enBoard)
{
	memset(enBoard.penDiscs, static_cast<int>(DISC::NONE), sizeof(DISC) * enBoard.enSize.ucRow * enBoard.enSize.ucCol);

	unsigned char iCenterUL =
		(enBoard.enSize.ucRow / 2 - 1) * enBoard.enSize.ucCol +
		(enBoard.enSize.ucCol / 2 - 1);

	enBoard.penDiscs[iCenterUL] = DISC::BLACK;
	enBoard.penDiscs[iCenterUL + 1] = DISC::WHITE;
	enBoard.penDiscs[iCenterUL + enBoard.enSize.ucCol] = DISC::WHITE;
	enBoard.penDiscs[iCenterUL + 1 + enBoard.enSize.ucCol] = DISC::BLACK;
}

bool GameRule::FlipDiscs(const DISC_MOVE enMove, BOARD_INFO enBoard)
{
	if ((enMove.enPos.ucRow > enBoard.enSize.ucRow) ||
		(enMove.enPos.ucCol > enBoard.enSize.ucCol))
	{
		/* エラーログ残す */
		return false;
	}

	unsigned int iMovePos = enBoard.enSize.ucCol * enMove.enPos.ucRow + enMove.enPos.ucCol;

	if (DISC::NONE != enBoard.penDiscs[iMovePos])
	{
		/* エラーログ残す */
		return false;
	}

	unsigned int unFlipTotal = 0;
	unsigned int unBoardByte = sizeof(DISC) * enBoard.enSize.ucRow * enBoard.enSize.ucCol;
	DISC* tmpDiscs;

	tmpDiscs = static_cast<DISC*>(malloc(unBoardByte));
	if (NULL == tmpDiscs)
	{
		/* エラーログ残す */
		return false;
	}
	memcpy(tmpDiscs, enBoard.penDiscs, unBoardByte);

	BOARD_INFO tmpBoardInfo;
	tmpBoardInfo.penDiscs = tmpDiscs;
	tmpBoardInfo.enSize = enBoard.enSize;

	unFlipTotal += FlipDisc_OneDirection(enMove, &tmpBoardInfo, -1, -1);	/* Upper Left */
	unFlipTotal += FlipDisc_OneDirection(enMove, &tmpBoardInfo, -1, 0);	/* Up */
	unFlipTotal += FlipDisc_OneDirection(enMove, &tmpBoardInfo, -1, 1);	/* Upper Right */
	unFlipTotal += FlipDisc_OneDirection(enMove, &tmpBoardInfo, 0, -1);	/* Left */
	unFlipTotal += FlipDisc_OneDirection(enMove, &tmpBoardInfo, 0, 1);	/* Right */
	unFlipTotal += FlipDisc_OneDirection(enMove, &tmpBoardInfo, 1, -1);	/* Lower Left */
	unFlipTotal += FlipDisc_OneDirection(enMove, &tmpBoardInfo, 1, 0);	/* Down */
	unFlipTotal += FlipDisc_OneDirection(enMove, &tmpBoardInfo, 1, 1);	/* Lower Right */

	if (0 == unFlipTotal)
	{
		return false;
	}

	tmpDiscs[iMovePos] = enMove.enColor;
	memcpy(enBoard.penDiscs, tmpDiscs, unBoardByte);

	return true;
}

unsigned char GameRule::FlipDisc_OneDirection(const DISC_MOVE enMove, BOARD_INFO* enBoard, const signed char ofsRow, const signed char ofsCol)
{
	unsigned char ucFlipTotal = 0;
	unsigned char ucTmpRow = enMove.enPos.ucRow + ofsRow;
	unsigned char ucTmpCol = enMove.enPos.ucCol + ofsCol;
	unsigned int iTmpPos = enBoard->enSize.ucCol * ucTmpRow + ucTmpCol;

	while (1)
	{
		/* Check Disc Color */
		if (DISC::NONE == enBoard->penDiscs[iTmpPos]) /* Finish Count Without Flip */
		{
			ucFlipTotal = 0;
			break;
		}
		else if (enMove.enColor == enBoard->penDiscs[iTmpPos]) /* Finish Count With Flip */
		{
			for (int i = 0;i < ucFlipTotal;i++)
			{
				ucTmpRow -= ofsRow;
				ucTmpCol -= ofsCol;
				iTmpPos = enBoard->enSize.ucCol * ucTmpRow + ucTmpCol;
				enBoard->penDiscs[iTmpPos] = enMove.enColor;
			}
			break;
		}
		else
		{
			ucFlipTotal++;
		}

		/* Check Next Position */
		if (((0 == ucTmpRow) && (0 > ofsRow)) ||
			((0 == ucTmpCol) && (0 > ofsCol)) ||
			((ucTmpRow >= (enBoard->enSize.ucRow - 1)) && (0 < ofsRow)) ||
			((ucTmpCol >= (enBoard->enSize.ucCol - 1)) && (0 < ofsCol))) /* Finish Count Without Flip */
		{
			ucFlipTotal = 0;
			break;
		}

		ucTmpRow += ofsRow;
		ucTmpCol += ofsCol;
		iTmpPos = enBoard->enSize.ucCol * ucTmpRow + ucTmpCol;
	}

	return ucFlipTotal;
}