#pragma once

#define _WINSOCKAPI_
#include <windows.h>
#include <process.h> 

#define DEF_ROW_MAX 16
#define DEF_COL_MAX 16
#define DEF_HIST_MAX 512

#define O_FAILURE 0
#define O_SUCCESS 1

enum class DISC : unsigned char
{
	NONE,
	BLACK,
	WHITE,
	OUT_OF_BOARD
};

enum class GAME_SETTING : unsigned char
{
	HUMAN_HUMAN,
	HUMAN_CPU,
	CPU_HUMAN,
	CPU_CPU
};

enum class GAME_RESULT : unsigned char
{
	WHITE_WIN,
	BLACK_WIN,
	DRAW,
	NO_CONTEST
};

enum class OTHELLO_PROCESS_ID : unsigned char
{
	GUI,
	GAME,
	GUI_DUMMY,
	GAME_DUMMY,
	ID_NUM
};

enum class OTHELLO_MSG_ID : unsigned short
{
	COM_START = WM_USER,
	GAME_START,
	GAME_QUIT,
	GAME_END,
	PUT_DISC,
	PASS_TURN,
	ID_NUM
};

typedef struct OTHELLO_MSG_
{
	OTHELLO_MSG_ID enId;
	unsigned short p1;
	unsigned short p2;
	unsigned short p3;
	unsigned short p4;
}OTHELLO_MSG;

typedef struct BOARD_SIZE_
{
	unsigned char ucRow;
	unsigned char ucCol;
}BOARD_SIZE;

typedef struct DISC_POS_
{
	unsigned char ucRow;
	unsigned char ucCol;
}DISC_POS;

typedef struct DISC_MOVE_
{
	DISC enColor;
	DISC_POS enPos;
}DISC_MOVE;

class BoardInfo
{
private:
	unsigned char m_ucRow;
	unsigned char m_ucCol;
	DISC* m_penBoard;

	unsigned short GetIndexCenterUL(void);

public:
	BoardInfo(unsigned char ucRow, unsigned char ucCol);
	BoardInfo(BOARD_SIZE enSize);
	BoardInfo(unsigned char ucRow, unsigned char ucCol, DISC* penBoard);
	BoardInfo(BOARD_SIZE enSize, DISC* penBoard);
	~BoardInfo(void);
	unsigned short GetSizeArray(void) const;
	unsigned short GetSizeByte(void) const;
	DISC GetDiscCol(unsigned char ucRow, unsigned char ucCol) const;
	DISC GetDiscCol(DISC_POS enPos) const;
	DISC_POS GetFirstPos(void) const;
	DISC_POS GetNextPos(unsigned char ucRow, unsigned char ucCol) const;
	DISC_POS GetNextPos(DISC_POS enPos) const;
	void CopyBoardTo(void* pDst) const; /* pDst must be allocated enough memory for Copying m_penBoard (= GetSizeByte()[byte]) */
	bool IsOnBoard(unsigned char ucRow, unsigned char ucCol) const;
	bool IsOnBoard(DISC_POS enPos) const;

	bool MoveStone(DISC enColor, unsigned char ucRow, unsigned char ucCol);
	bool MoveStone(DISC enColor, DISC_POS enPos);
	bool MoveStone(DISC_MOVE enMove);

	void Initialize(void);
	DISC CountDiscs(unsigned short& unCountB, unsigned short& unCountW) const;
};

inline unsigned short BoardInfo::GetIndexCenterUL(void)
{
	return (m_ucRow / 2 - 1) * m_ucCol + (m_ucCol / 2 - 1);
}

inline unsigned short BoardInfo::GetSizeArray(void) const
{
	return m_ucRow * m_ucCol;
}

inline unsigned short BoardInfo::GetSizeByte(void) const
{
	return sizeof(DISC) * GetSizeArray();
}

inline DISC BoardInfo::GetDiscCol(unsigned char ucRow, unsigned char ucCol) const
{
	return m_penBoard[ucRow * m_ucCol + ucCol];
}

inline DISC BoardInfo::GetDiscCol(DISC_POS enPos) const
{
	return GetDiscCol(enPos.ucRow, enPos.ucCol);
}

inline DISC_POS BoardInfo::GetFirstPos(void) const
{
	return { 0,0 };
}

inline DISC_POS BoardInfo::GetNextPos(unsigned char ucRow, unsigned char ucCol) const
{
	DISC_POS enPos = { ucRow,ucCol };

	while ((enPos.ucRow < m_ucRow) && (enPos.ucCol < m_ucCol))
	{
		if (++enPos.ucCol >= m_ucCol)
		{
			enPos.ucCol = 0;
			enPos.ucRow++;
		}

		if (IsOnBoard(enPos))
		{
			/* Found */
			return enPos;
		}
	}

	/* Not found */
	return GetFirstPos();
}

inline DISC_POS BoardInfo::GetNextPos(DISC_POS enPos) const
{
	return GetNextPos(enPos.ucRow, enPos.ucCol);
}

inline void BoardInfo::CopyBoardTo(void* pDst) const
{
	memcpy(pDst, m_penBoard, GetSizeByte());
	return;
}

inline bool BoardInfo::IsOnBoard(unsigned char ucRow, unsigned char ucCol) const
{
	if ((ucRow < m_ucRow) && (ucCol < m_ucCol))
	{
		if (DISC::OUT_OF_BOARD == m_penBoard[ucRow * m_ucCol + ucCol])
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

inline bool BoardInfo::IsOnBoard(DISC_POS enPos) const
{
	return IsOnBoard(enPos.ucRow, enPos.ucCol);
}

inline bool BoardInfo::MoveStone(DISC enColor, unsigned char ucRow, unsigned char ucCol)
{
	if (IsOnBoard(ucRow, ucCol))
	{
		m_penBoard[ucRow * m_ucCol + ucCol] = enColor;
		return true;
	}
	else
	{
		return false;
	}
}

inline bool BoardInfo::MoveStone(DISC enColor, DISC_POS enPos)
{
	return MoveStone(enColor, enPos.ucRow, enPos.ucCol);
}

inline bool BoardInfo::MoveStone(DISC_MOVE enMove)
{
	return MoveStone(enMove.enColor, enMove.enPos);
}