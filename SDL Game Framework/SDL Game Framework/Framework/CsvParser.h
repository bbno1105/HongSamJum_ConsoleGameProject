#pragma once

#include "Type.h"

struct Datas
{
	// @@@@@@@@@ ���ϴ� �������� �ٲܰ� @@@@@@@@@
	int ID;
	wchar_t String[500];
}Data[100];

<<<<<<< Updated upstream
int GetData(int ID);
=======
// ���� : Data[GetCsvData(ID)].Text[��] << ID�� 10�� ���� String�� �ҷ��´�.

int GetCsvData(int ID);
>>>>>>> Stashed changes

void CsvParse_Init(void);