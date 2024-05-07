#include "GameRule.h"
#include "CmnLog.h"

#include <windows.h>
#include <stdlib.h>

bool GameRule::CanMoveStone(const DISC enDiscCol, const BoardInfo* pcBoard)
{
	DISC_POS p = pcBoard->GetFirstPos();
	do /* loop until go back to first */
	{
		DISC_MOVE enDiscMove = { enDiscCol,p };
		if (CheckFlip(enDiscMove, pcBoard))
		{
			return true;
		}

		p = pcBoard->GetNextPos(p);

	} while (
		(p.ucRow != pcBoard->GetFirstPos().ucRow) ||
		(p.ucCol != pcBoard->GetFirstPos().ucCol)
		);

	return false;
}

bool GameRule::CheckFlip(const DISC_MOVE enMove, const BoardInfo* pcBoard)
{
	/* FlipDiscs() is const function if "withoutFlip" */
	return FlipDiscs(enMove, const_cast<BoardInfo*>(pcBoard), true);
}

bool GameRule::FlipDiscs(const DISC_MOVE enMove, BoardInfo* pcBoard, bool withoutFlip)
{
	if ((nullptr == pcBoard) ||
		(!pcBoard->IsOnBoard(enMove.enPos)))
	{
		WRITE_DEV_LOG_NOPARAM(OTHELLO_LOG_ID::NONE, "PARAM ERROR!");
		return false;
	}

	if (DISC::NONE != pcBoard->GetDiscCol(enMove.enPos))
	{
		return false;
	}

	unsigned short unFlipTotal = 0;

	unFlipTotal += FlipDisc_OneDirection(enMove, pcBoard, -1, -1, withoutFlip);	/* Upper Left */
	unFlipTotal += FlipDisc_OneDirection(enMove, pcBoard, -1, 0, withoutFlip);	/* Up */
	unFlipTotal += FlipDisc_OneDirection(enMove, pcBoard, -1, 1, withoutFlip);	/* Upper Right */
	unFlipTotal += FlipDisc_OneDirection(enMove, pcBoard, 0, -1, withoutFlip);	/* Left */
	unFlipTotal += FlipDisc_OneDirection(enMove, pcBoard, 0, 1, withoutFlip);	/* Right */
	unFlipTotal += FlipDisc_OneDirection(enMove, pcBoard, 1, -1, withoutFlip);	/* Lower Left */
	unFlipTotal += FlipDisc_OneDirection(enMove, pcBoard, 1, 0, withoutFlip);	/* Down */
	unFlipTotal += FlipDisc_OneDirection(enMove, pcBoard, 1, 1, withoutFlip);	/* Lower Right */

	if (0 == unFlipTotal)
	{
		return false;
	}

	if (!withoutFlip)
	{
		pcBoard->MoveStone(enMove);
	}

	return true;
}

unsigned char GameRule::FlipDisc_OneDirection(const DISC_MOVE enMove, BoardInfo* pcBoard,
	const signed char ofsRow, const signed char ofsCol, bool withoutFlip)
{
	unsigned char ucFlipTotal = 0;
	unsigned char ucTmpRow = enMove.enPos.ucRow + ofsRow;
	unsigned char ucTmpCol = enMove.enPos.ucCol + ofsCol;

	while (1)
	{
		/* Check Disc Color */
		if (DISC::NONE == pcBoard->GetDiscCol(ucTmpRow, ucTmpCol)) /* Finish Count Without Flip */
		{
			ucFlipTotal = 0;
			break;
		}
		else if (enMove.enColor == pcBoard->GetDiscCol(ucTmpRow, ucTmpCol)) /* Finish Count With Flip */
		{
			if (withoutFlip)
			{
				break;
			}

			for (int i = 0;i < ucFlipTotal;i++)
			{
				ucTmpRow -= ofsRow;
				ucTmpCol -= ofsCol;
				if (!pcBoard->MoveStone(enMove.enColor, ucTmpRow, ucTmpCol))
				{
					OTHELLO_LOG_PARAM p = { static_cast<unsigned short>(enMove.enColor, ucTmpRow, ucTmpCol) };
					WRITE_DEV_LOG(OTHELLO_LOG_ID::NONE, p, "INVALID MOVE POS");
				}
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
			(!pcBoard->IsOnBoard(ucTmpRow + ofsRow, ucTmpCol + ofsCol))) /* Finish Count Without Flip */
		{
			ucFlipTotal = 0;
			break;
		}

		ucTmpRow += ofsRow;
		ucTmpCol += ofsCol;
	}

	return ucFlipTotal;
}