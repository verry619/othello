#pragma once

#include "Cmn.h"

struct DRAW_POS
{
	int x;
	int y;
};
struct DRAW_SIZE
{
	int width;
	int height;
};
struct DRAW_DST
{
	DRAW_POS pos;
	DRAW_SIZE size;
};

class GuiCalc
{
public:
	static DRAW_DST ConvDiscPosToDrawDst(DISC_POS discPos, unsigned char rowMax, unsigned char colMax);
	static bool ConvDrawPosToDiscPos(DRAW_POS drawPos, DISC_POS& discPos, unsigned char rowMax, unsigned char colMax);
};

