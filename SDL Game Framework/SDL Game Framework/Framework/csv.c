#include "stdafx.h"

static char* s_Buffer;
static char* s_BufferPointer;

void readFileToBuffer(const char* filename)
{
	FILE* fp;
	if (0 != fopen_s(&fp, filename, "r"))
	{
		return;
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);

	s_Buffer = malloc(sizeof(char) * (fileSize + 1));
	memset(s_Buffer, 0, sizeof(char) * (fileSize + 1));

	fseek(fp, 0, SEEK_SET);
	fread(s_Buffer, fileSize, 1, fp);

	fclose(fp);
}

int countCategory(const char* firstLine)
{
	int result = 1;
	while (*firstLine != '\n')
	{
		if (*firstLine == '@')
		{
			++result;
		}

		++firstLine;
	}

	return result;
}

void CreateCsvFile(CsvFile* csvFile, const char* filename)
{
	readFileToBuffer(filename);

	csvFile->ColumnCount = countCategory(s_Buffer);
	for (int i = 0; i < MAXIMUM_ROW; ++i)
	{
		csvFile->Items[i] = (CsvItem*)malloc(sizeof(CsvItem) * csvFile->ColumnCount);
	}

	s_BufferPointer = s_Buffer;
	while (*s_BufferPointer != '\0')
	{
		int row = csvFile->RowCount;

		// �� ���� �о���δ�.
		int commaCount = 0;
		const char* lineStart = s_BufferPointer;
		const char* lineEnd = lineStart;
		while (true)
		{
			if (csvFile->ColumnCount - 1 <= commaCount && '\n' == *lineEnd)
			{
				break;
			}

			if (*lineEnd == '@')
			{
				++commaCount;
			}

			++lineEnd;
		}

		// �޸� �з�
		const char* recordStart = lineStart;
		const char* recordEnd = recordStart;
		for (int i = 0; i < csvFile->ColumnCount; ++i)
		{
			while (*recordEnd != '@' && recordEnd != lineEnd)
			{
				++recordEnd;
			}

			int size = recordEnd - recordStart;
			csvFile->Items[row][i].RawData = (char*)malloc(sizeof(char) * (size + 1));
			memcpy(csvFile->Items[row][i].RawData, recordStart, size);
			csvFile->Items[row][i].RawData[size] = '\0';

			recordStart = recordEnd + 1;
			recordEnd = recordStart;
		}

		++csvFile->RowCount;

		s_BufferPointer = lineEnd + 1;
	}
	
}

int ParseToInt(const CsvItem item)
{
	char* end;
	return strtol(item.RawData, &end, 10);
}

char* ParseToAscii(const CsvItem item)
{
	return item.RawData;
}

wchar_t* ParseToUnicode(const CsvItem item)
{
	int32 size = strlen(item.RawData);
	int32 wideLen = MultiByteToWideChar(CP_ACP, NULL, item.RawData, -1, NULL, 0);
	wchar_t* result = (wchar_t*)malloc(sizeof(wchar_t) * wideLen);
	memset(result, 0, sizeof(wchar_t) * wideLen);

	if (item.RawData[0] == '"' && item.RawData[size - 1] == '"')
	{
		MultiByteToWideChar(CP_ACP, NULL, &item.RawData[1], -1, result, wideLen - 3);
	}
	else
	{
		MultiByteToWideChar(CP_ACP, NULL, item.RawData, -1, result, wideLen);
	}

	return result;
}

void csv_Init(void)
{
	memset(&csvFile, 0, sizeof(CsvFile));
	CreateCsvFile(&csvFile, "GoodmorningSylvester.csv");

	// CSV ���� �Ľ��� �� �ؽ�Ʈ �׷��� ���� ����� ��� �ȵǸ�
	// App_Init()�� �Ʒ� ���� �߰�
	setlocale(LC_ALL, "kr_KR.utf8");
}


bool isString = false;

wchar_t* StringLine(wchar_t* string, wchar_t* stringl)
{

	while (true)
	{
		if (*string == L'\n')
		{
			*stringl = NULL;
			string++;
			break;
		}
		else if (*string == NULL)
		{
			*stringl = NULL;
			break;
		}
		*stringl = *string;
		stringl++;  string++;
	}
	return string;
}