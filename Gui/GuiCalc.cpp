#include "GuiCalc.h"

#define BOARD_LEFTTOP_POS_X 100
#define BOARD_LEFTTOP_POS_Y 50
#define BOARD_WIDTH 400
#define BOARD_HEIGHT 400

DRAW_DST GuiCalc::ConvDiscPosToDrawDst(DISC_POS discPos, unsigned char rowMax, unsigned char colMax)
{
	DRAW_DST enDrawDst;
	enDrawDst.size.width = BOARD_WIDTH / colMax;
	enDrawDst.size.height = BOARD_HEIGHT / rowMax;
	enDrawDst.pos.x = BOARD_LEFTTOP_POS_X + enDrawDst.size.width * discPos.ucCol;
	enDrawDst.pos.y = BOARD_LEFTTOP_POS_Y + enDrawDst.size.height * discPos.ucRow;

	return enDrawDst;
}

bool GuiCalc::ConvDrawPosToDiscPos(DRAW_POS drawPos, DISC_POS& discPos, unsigned char rowMax, unsigned char colMax)
{
	if (
		(drawPos.x < BOARD_LEFTTOP_POS_X) ||
		(drawPos.y < BOARD_LEFTTOP_POS_Y) ||
		(drawPos.x >= BOARD_LEFTTOP_POS_X + BOARD_WIDTH) ||
		(drawPos.y >= BOARD_LEFTTOP_POS_Y + BOARD_HEIGHT)
		)
	{
		return false;
	}

	discPos.ucCol = (drawPos.x - BOARD_LEFTTOP_POS_X) / (BOARD_WIDTH / colMax);
	discPos.ucRow = (drawPos.y - BOARD_LEFTTOP_POS_Y) / (BOARD_HEIGHT / rowMax);

	return true;
}