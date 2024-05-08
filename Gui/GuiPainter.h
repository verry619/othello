#pragma once

#include "Cmn.h"
#include "GuiCalc.h"

class GuiPainter
{
private:
	int ConvDiscColToResourceId(DISC enDiscCol) const;
	void DrawCell(HDC hdc, HINSTANCE hInst, DRAW_DST drawDst, int resourceId);
public:
	void DrawBoard(HWND hWnd, HINSTANCE hInst, unsigned char ucRow, unsigned char ucCol, DISC* penBoard);
};

