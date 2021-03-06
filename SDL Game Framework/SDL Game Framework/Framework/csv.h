#pragma once

#include "Type.h"
#include <wchar.h>

#define MAXIMUM_ROW 10000

typedef struct CsvItem
{
	char*	RawData;
} CsvItem;

typedef struct CsvFile
{
	int			ColumnCount;
	int			RowCount;
	CsvItem*	Items[MAXIMUM_ROW];
} CsvFile;

CsvFile csvFile;

void CreateCsvFile(CsvFile* csvFile, const char* filename);
void csv_Init(void);

int			ParseToInt(const CsvItem item);
char*		ParseToAscii(const CsvItem item);
wchar_t*	ParseToUnicode(const CsvItem item);
wchar_t*	StringLine(wchar_t* string, wchar_t* stringl);

enum column
{
	ID_i,
	ScenePage_s,
	Text_s,
	TextLine_i,// 테이블에 추가되어야 함
	TextEffect_i,
	Select1_s,
	MovingPage1_i,
	Select2_s,
	MovingPage2_i,
	Select3_s,
	MovingPage3_i,
	BGM,
	SE,
	SE_loop,
	SoundTime,
	ImageFile_s,
	Image_Time_i
};