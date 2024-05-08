#pragma once

#include "Cmn.h"

class GuiPainter
{
private:
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
		GuiPainter::DRAW_POS pos;
		GuiPainter::DRAW_SIZE size;
	};
	DRAW_DST ConvDiscPosToDrawDst(unsigned char row, unsigned char col, unsigned char rowMax, unsigned char colMax) const;
	int ConvDiscColToResourceId(DISC enDiscCol) const;
	void DrawCell(HDC hdc, HINSTANCE hInst, DRAW_DST drawDst, int resourceId);
public:
	void DrawBoard(HWND hWnd, HINSTANCE hInst, unsigned char ucRow, unsigned char ucCol, DISC* penBoard);
};

