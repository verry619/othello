#pragma once

#include "Cmn.h"

class GameRule
{
private:
	static unsigned char FlipDisc_OneDirection(const DISC_MOVE enMove, BOARD_INFO* enBoard, const signed char ofsRow, const signed char ofsCol);

public:
	static void InitializeBoard(BOARD_INFO enBoard);
	static bool FlipDiscs(const DISC_MOVE enMove, BOARD_INFO enBoard);
};

