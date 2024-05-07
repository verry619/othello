#pragma once

#include "Cmn.h"

class GameRule
{
private:
	static unsigned char FlipDisc_OneDirection(const DISC_MOVE enMove, BoardInfo* pcBoard,
		const signed char ofsRow, const signed char ofsCol, bool withoutFlip = false);

public:
	static bool CanMoveStone(const DISC enDiscCol, const BoardInfo* pcBoard);
	static bool CheckFlip(const DISC_MOVE enMove, const BoardInfo* pcBoard);
	static bool FlipDiscs(const DISC_MOVE enMove, BoardInfo* pcBoard, bool withoutFlip = false);
};

