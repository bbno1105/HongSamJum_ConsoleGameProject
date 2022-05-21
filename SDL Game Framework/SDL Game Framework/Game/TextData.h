#pragma once

#include "Framework.h"
#include "Type.h"

typedef struct TagTextData
{
	int32	ID;
	Text    GuideLine[50];       // [id][�ٹٲ�]
	int32   TextLine;            // �ؽ�Ʈ ��
	int32   TotalLine;           // �� �������� üũ
	int32   FontSize;
	COORD	Coord;

	//������ �����ε� ���̵� ������Ʈ�ϰ� �ؽ�Ʈ ����ؾ��ؼ� ������. ��c���ִ� ������ �ϴ� ����� �ȵ�
	int32   MovingPageNumberIndex;            // �� �ε���
	int32   SelectMovingPageNumber[3]; // �� ��ȯ��
	float	ElapsedTimeForRenderingLineByLine;
} TextData;

void TextData_Init(TextData* textData);
void TextData_Update(TextData* textData);
void TextData_Render(TextData* textData);
void TextData_Release(TextData* textData);

void SaveOneTextLine(TextData* textData);

void SwingText(TextData* textData);