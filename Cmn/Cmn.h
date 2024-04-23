#pragma once

#include <process.h> 

#define DEF_ROW_MAX 16
#define DEF_COL_MAX 16
#define DEF_HIST_MAX 512

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

enum class OTHELLO_MSG_ID : unsigned char
{
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
	unsigned int p1;
	unsigned int p2;
	unsigned int p3;
	unsigned int p4;
}OTHELLO_MSG;

typedef struct BOARD_SIZE_
{
	unsigned char ucRow;
	unsigned char ucCol;
}BOARD_SIZE;

typedef struct BOARD_INFO_
{
	BOARD_SIZE enSize;
	DISC* penDiscs;
}BOARD_INFO;

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