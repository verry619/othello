#include "GuiPainter.h"
#include "resource.h"

#include <windowsx.h>

#define BOARD_LEFTTOP_POS_X 100
#define BOARD_LEFTTOP_POS_Y 50
#define BOARD_WIDTH 400
#define BOARD_HEIGHT 400

void GuiPainter::DrawBoard(HWND hWnd, HINSTANCE hInst, unsigned char ucRow, unsigned char ucCol, DISC* penBoard)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	BoardInfo* pcBoardInfo = new BoardInfo(ucRow, ucCol, penBoard);

	for (unsigned char r = 0;r < ucRow;r++)
	{
		for (unsigned char c = 0;c < ucCol;c++)
		{
			DRAW_DST drawDst = ConvDiscPosToDrawDst(r, c, ucRow, ucCol);
			int resourceId = ConvDiscColToResourceId(pcBoardInfo->GetDiscCol(r, c));
			DrawCell(hdc, hInst, drawDst, resourceId);
		}
	}

	EndPaint(hWnd, &ps);
}

GuiPainter::DRAW_DST GuiPainter::ConvDiscPosToDrawDst(unsigned char row, unsigned char col, unsigned char rowMax, unsigned char colMax) const
{
	DRAW_DST enDrawDst;
	enDrawDst.size.width = BOARD_WIDTH / rowMax;
	enDrawDst.size.height = BOARD_HEIGHT / colMax;
	enDrawDst.pos.x = BOARD_LEFTTOP_POS_X + enDrawDst.size.width * row;
	enDrawDst.pos.y = BOARD_LEFTTOP_POS_Y + enDrawDst.size.height * col;

	return enDrawDst;
}

int GuiPainter::ConvDiscColToResourceId(DISC enDiscCol) const
{
	switch (enDiscCol)
	{
	case DISC::BLACK:
		return IDB_CELL_BLACK;
	case DISC::WHITE:
		return IDB_CELL_WHITE;
	default:
		return IDB_CELL_NONE;
	}
}

void GuiPainter::DrawCell(HDC hdc, HINSTANCE hInst, DRAW_DST drawDst, int resourceId)
{
	HBITMAP	hBmp;
	BITMAP bmp;
	HDC hmdc;

	//�r�b�g�}�b�v�̓ǂݍ���
	hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(resourceId));

	//�r�b�g�}�b�v���̎擾
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	//�������f�o�C�X�R���e�L�X�g�̎擾
	hmdc = CreateCompatibleDC(hdc);

	//�������f�o�C�X�R���e�L�X�g�Ƀr�b�g�}�b�v�n���h�����Z�b�g
	SelectObject(hmdc, hBmp);

	//�������f�o�C�X�R���e�L�X�g����
	//�f�o�C�X�R���e�L�X�g�Ƀf�[�^��]��
	StretchBlt(
		hdc, drawDst.pos.x, drawDst.pos.y, drawDst.size.width, drawDst.size.height,
		hmdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	DeleteDC(hmdc);
	DeleteObject(hBmp);
}