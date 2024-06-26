﻿// Cmn.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "pch.h"
#include "framework.h"

#include <new>

#include "Cmn.h"

BoardInfo::BoardInfo(unsigned char ucRow, unsigned char ucCol)
{
	m_ucRow = ucRow;
	m_ucCol = ucCol;
	m_penBoard = new DISC[ucRow * ucCol];
}

BoardInfo::BoardInfo(BOARD_SIZE enSize) :BoardInfo(enSize.ucRow, enSize.ucCol)
{

}

BoardInfo::BoardInfo(unsigned char ucRow, unsigned char ucCol, const DISC* penBoard) : BoardInfo(ucRow, ucCol)
{
	memcpy(m_penBoard, penBoard, GetSizeByte());
}

BoardInfo::BoardInfo(BOARD_SIZE enSize, const DISC* penBoard) : BoardInfo(enSize.ucRow, enSize.ucCol, penBoard)
{

}

BoardInfo::BoardInfo(std::vector<std::vector<DISC>> vvBoard)
	: BoardInfo(static_cast<unsigned char>(vvBoard.size()), static_cast<unsigned char>(vvBoard.at(0).size()))
{
	for (unsigned char r = 0;r < m_ucRow;r++)
	{
		for (unsigned char c = 0;c < m_ucCol;c++)
		{
			m_penBoard[m_ucCol * r + c] = vvBoard[r][c];
		}
	}
}

BoardInfo::~BoardInfo(void)
{
	delete[] m_penBoard;
}

void BoardInfo::Initialize(void)
{
	memset(m_penBoard, static_cast<unsigned char>(DISC::NONE), GetSizeByte());

	unsigned short iCenterUL = GetIndexCenterUL();

	m_penBoard[iCenterUL] = DISC::BLACK;
	m_penBoard[iCenterUL + 1] = DISC::WHITE;
	m_penBoard[iCenterUL + m_ucCol] = DISC::WHITE;
	m_penBoard[iCenterUL + 1 + m_ucCol] = DISC::BLACK;
}

DISC BoardInfo::CountDiscs(unsigned short& unCountB, unsigned short& unCountW) const
{
	unCountB = 0;
	unCountW = 0;

	for (unsigned char row = 0;row < m_ucRow;row++)
	{
		for (unsigned char col = 0;col < m_ucCol;col++)
		{
			unsigned short iPos = m_ucCol * row + col;
			if (DISC::BLACK == m_penBoard[iPos])
			{
				unCountB++;
			}
			else if (DISC::WHITE == m_penBoard[iPos])
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