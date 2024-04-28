#include "pch.h"

#include "GameRule.h"

constexpr unsigned int ROW_MAX = 6;
constexpr unsigned int COL_MAX = 6;

enum class EN_BOARD_FOR_TEST
{
	ALL_NONE,
	ALL_BLACK,
	ALL_WHITE,
	INIT,
	BW_HALF,
};

const DISC BOARD_ALL_NONE[ROW_MAX][COL_MAX] = { DISC::NONE };
const DISC BOARD_ALL_BLACK[ROW_MAX][COL_MAX] = { DISC::BLACK };
const DISC BOARD_ALL_WHITE[ROW_MAX][COL_MAX] = { DISC::WHITE };
const DISC BOARD_INIT[ROW_MAX][COL_MAX] =
{
	{DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::BLACK,DISC::WHITE,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::WHITE,DISC::BLACK,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE},
	{DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE,DISC::NONE},
};
const DISC BOARD_BW_HALF[ROW_MAX][COL_MAX] =
{
	{DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE},
	{DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE},
	{DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE},
	{DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE},
	{DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE},
	{DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE,DISC::BLACK,DISC::WHITE},
};

static const std::map<EN_BOARD_FOR_TEST, DISC const> g_mapBoardForTest =
{
	{EN_BOARD_FOR_TEST::ALL_NONE,	BOARD_ALL_NONE[0][0]	},
	{EN_BOARD_FOR_TEST::ALL_BLACK,	BOARD_ALL_BLACK[0][0]	},
	{EN_BOARD_FOR_TEST::ALL_WHITE,	BOARD_ALL_WHITE[0][0]	},
	{EN_BOARD_FOR_TEST::INIT,		BOARD_INIT[0][0]		},
	{EN_BOARD_FOR_TEST::BW_HALF,	BOARD_BW_HALF[0][0]	},
};

void InitializeBoardTest_dataset(BOARD_INFO& stBoardInfo_Tested, EN_BOARD_FOR_TEST enIdx, BOARD_INFO& stBoardInfo_Expected)
{
	/* tested data */
	stBoardInfo_Tested.enSize = { ROW_MAX,COL_MAX };
	memcpy(stBoardInfo_Tested.penDiscs,
		&g_mapBoardForTest.at(enIdx),
		sizeof(DISC) * ROW_MAX * COL_MAX);

	/* expected data */
	stBoardInfo_Expected.enSize = { ROW_MAX,COL_MAX };
	memcpy(stBoardInfo_Expected.penDiscs,
		&BOARD_INIT[0][0],
		sizeof(DISC) * ROW_MAX * COL_MAX);
}

TEST(GameRuleTest, InitializeBoardTest001) {
	/* data initializing */
	DISC enBoard_Tested[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Tested = { 0,0,&enBoard_Tested[0][0] };
	DISC enBoard_Expected[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Expected = { 0,0,&enBoard_Expected[0][0] };

	InitializeBoardTest_dataset(stBoardInfo_Tested, EN_BOARD_FOR_TEST::ALL_NONE, stBoardInfo_Expected);

	/* testing */
	GameRule::InitializeBoard(stBoardInfo_Tested);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucRow, stBoardInfo_Tested.enSize.ucRow);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucCol, stBoardInfo_Tested.enSize.ucCol);
	EXPECT_EQ(0, memcmp(stBoardInfo_Expected.penDiscs, stBoardInfo_Tested.penDiscs, sizeof(DISC) * ROW_MAX * COL_MAX));
}

TEST(GameRuleTest, InitializeBoardTest002) {
	/* data initializing */
	DISC enBoard_Tested[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Tested = { 0,0,&enBoard_Tested[0][0] };
	DISC enBoard_Expected[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Expected = { 0,0,&enBoard_Expected[0][0] };

	InitializeBoardTest_dataset(stBoardInfo_Tested, EN_BOARD_FOR_TEST::ALL_BLACK, stBoardInfo_Expected);

	/* testing */
	GameRule::InitializeBoard(stBoardInfo_Tested);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucRow, stBoardInfo_Tested.enSize.ucRow);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucCol, stBoardInfo_Tested.enSize.ucCol);
	EXPECT_EQ(0, memcmp(stBoardInfo_Expected.penDiscs, stBoardInfo_Tested.penDiscs, sizeof(DISC) * ROW_MAX * COL_MAX));
}

TEST(GameRuleTest, InitializeBoardTest003) {
	/* data initializing */
	DISC enBoard_Tested[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Tested = { 0,0,&enBoard_Tested[0][0] };
	DISC enBoard_Expected[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Expected = { 0,0,&enBoard_Expected[0][0] };

	InitializeBoardTest_dataset(stBoardInfo_Tested, EN_BOARD_FOR_TEST::ALL_WHITE, stBoardInfo_Expected);

	/* testing */
	GameRule::InitializeBoard(stBoardInfo_Tested);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucRow, stBoardInfo_Tested.enSize.ucRow);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucCol, stBoardInfo_Tested.enSize.ucCol);
	EXPECT_EQ(0, memcmp(stBoardInfo_Expected.penDiscs, stBoardInfo_Tested.penDiscs, sizeof(DISC) * ROW_MAX * COL_MAX));
}

TEST(GameRuleTest, InitializeBoardTest004) {
	/* data initializing */
	DISC enBoard_Tested[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Tested = { 0,0,&enBoard_Tested[0][0] };
	DISC enBoard_Expected[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Expected = { 0,0,&enBoard_Expected[0][0] };

	InitializeBoardTest_dataset(stBoardInfo_Tested, EN_BOARD_FOR_TEST::INIT, stBoardInfo_Expected);

	/* testing */
	GameRule::InitializeBoard(stBoardInfo_Tested);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucRow, stBoardInfo_Tested.enSize.ucRow);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucCol, stBoardInfo_Tested.enSize.ucCol);
	EXPECT_EQ(0, memcmp(stBoardInfo_Expected.penDiscs, stBoardInfo_Tested.penDiscs, sizeof(DISC) * ROW_MAX * COL_MAX));
}

TEST(GameRuleTest, InitializeBoardTest005) {
	/* data initializing */
	DISC enBoard_Tested[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Tested = { 0,0,&enBoard_Tested[0][0] };
	DISC enBoard_Expected[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Expected = { 0,0,&enBoard_Expected[0][0] };

	InitializeBoardTest_dataset(stBoardInfo_Tested, EN_BOARD_FOR_TEST::BW_HALF, stBoardInfo_Expected);

	/* testing */
	GameRule::InitializeBoard(stBoardInfo_Tested);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucRow, stBoardInfo_Tested.enSize.ucRow);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucCol, stBoardInfo_Tested.enSize.ucCol);
	EXPECT_EQ(0, memcmp(stBoardInfo_Expected.penDiscs, stBoardInfo_Tested.penDiscs, sizeof(DISC) * ROW_MAX * COL_MAX));
}

TEST(GameRuleTest, FlipDiscs) {
	/* data initializing */
	DISC enBoard_Tested[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Tested = { 0,0,&enBoard_Tested[0][0] };
	DISC enBoard_Expected[ROW_MAX][COL_MAX];
	BOARD_INFO stBoardInfo_Expected = { 0,0,&enBoard_Expected[0][0] };

	InitializeBoardTest_dataset(stBoardInfo_Tested, EN_BOARD_FOR_TEST::BW_HALF, stBoardInfo_Expected);

	/* testing */
	GameRule::InitializeBoard(stBoardInfo_Tested);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucRow, stBoardInfo_Tested.enSize.ucRow);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucCol, stBoardInfo_Tested.enSize.ucCol);
	EXPECT_EQ(0, memcmp(stBoardInfo_Expected.penDiscs, stBoardInfo_Tested.penDiscs, sizeof(DISC) * ROW_MAX * COL_MAX));
}