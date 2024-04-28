#include "pch.h"

#include "GameRule.h"

constexpr unsigned int ROW_MAX = 6;
constexpr unsigned int COL_MAX = 6;

const DISC EN_ALL_NONE[ROW_MAX][COL_MAX] = { DISC::NONE };
const DISC EN_ALL_BLACK[ROW_MAX][COL_MAX] = { DISC::BLACK };
const DISC EN_ALL_WHITE[ROW_MAX][COL_MAX] = { DISC::WHITE };
const DISC EN_INIT[ROW_MAX][COL_MAX] =
{
	{DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::BLACK,DISC::WHITE,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::WHITE,DISC::BLACK,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE},
};

TEST(GameRuleTest, InitializeBoardTest001) {
	/* tested data */
	DISC enBoard_Tested[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Tested = { 0,0,&enBoard_Tested[0][0] };
	stBoardInfo_Tested.enSize = { ROW_MAX,COL_MAX };
	memcpy(stBoardInfo_Tested.penDiscs,
		&EN_ALL_NONE[0][0],
		sizeof(DISC) * ROW_MAX * COL_MAX);

	/* expected data */
	DISC enBoard_Expected[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Expected = { 0,0,&enBoard_Expected[0][0] };
	stBoardInfo_Expected.enSize = { ROW_MAX,COL_MAX };
	memcpy(stBoardInfo_Expected.penDiscs,
		&EN_INIT[0][0],
		sizeof(DISC) * ROW_MAX * COL_MAX);

	/* testing */
	GameRule::InitializeBoard(stBoardInfo_Tested);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucRow, stBoardInfo_Tested.enSize.ucRow);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucCol, stBoardInfo_Tested.enSize.ucCol);
	EXPECT_EQ(0, memcmp(stBoardInfo_Expected.penDiscs, stBoardInfo_Tested.penDiscs, sizeof(DISC) * ROW_MAX * COL_MAX));
}