#include "GuiState.h"

#define BOARD_ROW_LEN 6
#define BOARD_COL_LEN 6

DISC GuiState::s_currentTurn = DISC::BLACK;

GuiState::GuiState(GuiBoardVV*& vv)
	:m_discVV(vv)
{

}

void GuiState::UpdateBoard(const DISC* penBoard)
{
	unsigned char ucRow = m_discVV->GetRow();
	unsigned char ucCol = m_discVV->GetCol();

	for (unsigned char r = 0;r < ucRow;r++)
	{
		for (unsigned char c = 0;c < ucCol;c++)
		{
			m_discVV->SetDisc(r, c, penBoard[ucCol * r + c]);
		}
	}
}

void GuiState::DrawBoard(GuiMainWnd*& pcMainWnd)
{
	pcMainWnd->DrawBoard(m_discVV);
}

void GuiState::ProceedNextTurn(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd, bool bRes, DISC enDiscCol)
{
	DISC* penBoard = pcCom->FetchCurrentBoard();
	UpdateBoard(penBoard);
	DrawBoard(pcMainWnd);
	delete penBoard;

	if (bRes)
	{
		if (DISC::BLACK == enDiscCol)
		{
			s_currentTurn = DISC::WHITE;
		}
		else if (DISC::WHITE == enDiscCol)
		{
			s_currentTurn = DISC::BLACK;
		}
	}
}

void GuiState::GameStart_Sub(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GAME_SETTING enGameSetting = pcMainWnd->GetCurrentGameSetting();

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::GAME_START;
	msg.p1 = BOARD_ROW_LEN;
	msg.p2 = BOARD_COL_LEN;
	msg.p3 = static_cast<unsigned short>(enGameSetting);
	msg.p4 = 0;

	pcCom->UpdateBoardSize(BOARD_ROW_LEN, BOARD_COL_LEN);
	pcCom->SendMsg(OTHELLO_PROCESS_ID::GAME, msg);
}