#pragma once

#include "Type.h"

struct Datas
{
	// @@@@@@@@@ ���ϴ� �������� �ٲܰ� @@@@@@@@@
	int ID;
	wchar_t ScenePage[20][500]; // 20�� 500��
	wchar_t Text[20][500];
	int Select1;
	wchar_t MovingPage1[20][500];
	int Select2;
	wchar_t MovingPage2[20][500];
	int Select3;
	wchar_t MovingPage3[20][500];
	wchar_t SoundFile[20][500];
	wchar_t ImageFile[20][500];

}Data[100];
// 100���� ������
// 2���� �÷���
// 20���� �ۿ�
// 500���� ����

int GetCsvData(int ID);

void CsvParse_Init(void);