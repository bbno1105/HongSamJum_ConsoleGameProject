#pragma once

#include "Type.h"

struct Datas
{
	// @@@@@@@@@ ���ϴ� �������� �ٲܰ� @@@@@@@@@
	int ID;
	wchar_t String[500];
}Data[100];

int GetData(int ID);

void CsvParse_Init(void);