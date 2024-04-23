#include "pch.h"

#include "GameRule.h"

TEST(GameRuleTest, InitializeBoardTest001) {
	BOARD_INFO stBoardInfo_Tested;
	BOARD_INFO stBoardInfo_Expected;
	stBoardInfo_Tested.enSize = { 6,6 };
	stBoardInfo_Expected.enSize = { 6,6 };

	DISC enDiscsAllBlack[6][6] = { DISC::BLACK };
	stBoardInfo_Tested.penDiscs = &enDiscsAllBlack[0][0];

	DISC enDiscsInitialized[6][6] = { DISC::NONE };
	stBoardInfo_Expected.penDiscs = &enDiscsInitialized[0][0];

	GameRule::InitializeBoard(stBoardInfo_Tested);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucRow, stBoardInfo_Tested.enSize.ucRow);
	EXPECT_EQ(stBoardInfo_Expected.enSize.ucCol, stBoardInfo_Tested.enSize.ucCol);
	EXPECT_EQ(0, memcmp(stBoardInfo_Expected.penDiscs, stBoardInfo_Tested.penDiscs, sizeof(DISC) * 6 * 6));
}