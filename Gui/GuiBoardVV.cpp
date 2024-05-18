#include "GuiBoardVV.h"

GuiBoardVV::GuiBoardVV(unsigned char ucRow, unsigned char ucCol)
{
	vv = std::vector<std::vector<DISC>>(ucRow, std::vector<DISC>(ucCol, DISC::NONE));
}

DISC GuiBoardVV::GetDisc(unsigned char ucRow, unsigned char ucCol)
{
	mutex.lock();
	DISC ret = vv[ucRow][ucCol];
	mutex.unlock();

	return ret;
}

void GuiBoardVV::SetDisc(unsigned char ucRow, unsigned char ucCol, DISC enCol)
{
	mutex.lock();
	vv[ucRow][ucCol] = enCol;
	mutex.unlock();
}

unsigned char GuiBoardVV::GetRow(void)
{
	mutex.lock();
	unsigned char ret = static_cast<unsigned char>(vv.size());
	mutex.unlock();

	return ret;
}

unsigned char GuiBoardVV::GetCol(void)
{
	mutex.lock();
	unsigned char ret = static_cast<unsigned char>(vv[0].size());
	mutex.unlock();

	return ret;
}

std::vector<std::vector<DISC>> GuiBoardVV::GetVV(void)
{
	mutex.lock();
	std::vector<std::vector<DISC>> ret = vv;
	mutex.unlock();

	return ret;
}