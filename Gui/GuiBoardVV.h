#pragma once

#include "Cmn.h"

#include <vector>
#include <mutex>

class GuiBoardVV
{
private:
	std::vector<std::vector<DISC>> vv;
	std::mutex mutex;
public:
	GuiBoardVV(unsigned char ucRow, unsigned char ucCol);
	DISC GetDisc(unsigned char ucRow, unsigned char ucCol);
	void SetDisc(unsigned char ucRow, unsigned char ucCol, DISC enCol);
	unsigned char GetRow(void);
	unsigned char GetCol(void);
	std::vector<std::vector<DISC>> GetVV(void);
};

