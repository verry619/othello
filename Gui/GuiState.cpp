#include "GuiState.h"

GuiState::GuiState(std::vector<std::vector<DISC>>& vv)
	:m_discVV(vv)
{

}

void GuiState::UpdateBoard(const DISC* penBoard)
{
	unsigned char ucRow = m_discVV.size();
	unsigned char ucCol = m_discVV.at(0).size();

	for (unsigned char r = 0;r < ucRow;r++)
	{
		for (unsigned char c = 0;c < ucCol;c++)
		{
			m_discVV[r][c] = penBoard[ucCol * r + c];
		}
	}
}

void GuiState::DrawBoard(GuiMainWnd*& pcMainWnd)
{
	pcMainWnd->DrawBoard(m_discVV);
}