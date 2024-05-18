#include "GuiState.h"

DISC GuiState::s_currentTurn = DISC::BLACK;

GuiState::GuiState(GuiBoardVV*& vv)
	:m_discVV(vv)
{
	m_ucGameRow = vv->GetRow();
	m_ucGameCol = vv->GetCol();
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
		pcMainWnd->DrawTextInfo(s_currentTurn);
	}
}

void GuiState::GameStart_Sub(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	GAME_SETTING enGameSetting = pcMainWnd->GetCurrentGameSetting();

	OTHELLO_MSG msg;
	msg.enId = OTHELLO_MSG_ID::GAME_START;
	msg.p1 = m_ucGameRow;
	msg.p2 = m_ucGameCol;
	msg.p3 = static_cast<unsigned short>(enGameSetting);
	msg.p4 = 0;

	pcCom->UpdateBoardSize(m_ucGameRow, m_ucGameCol);
	pcCom->SendMsg(OTHELLO_PROCESS_ID::GAME, msg);

	pcMainWnd->DrawTextInfo(s_currentTurn, GuiMainWnd::EN_RESULT_VALID::INVALIDATE);
}

void GuiState::GameEnd_Sub(GuiCom*& pcCom, GuiMainWnd*& pcMainWnd)
{
	pcMainWnd->DrawTextInfo(s_currentTurn, GuiMainWnd::EN_RESULT_VALID::VALIDATE);
}