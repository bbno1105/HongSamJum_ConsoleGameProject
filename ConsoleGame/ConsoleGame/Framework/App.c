#include "App.h"
#include "Common.h"
#include "Renderer.h"
#include "Timer.h"

bool App_Init()
{
	if (false == Renderer_Init())
	{
		return false;
	}
	return true;
}

void processInput()
{

}

float elapsedTime = 0;
bool canShow = false;

void update()
{
	//char str[128] = "";
	//sprintf_s(str, sizeof(str), "���� FPS : %d", (int32)(1 / Timer_GetDeltaTime()));
	//Renderer_DrawText(str, strlen(str));

	elapsedTime += 1.0f * DELTA_TIME; // ��ũ�ε� �Ẹ��

	if (elapsedTime >= 0.5f)
	{
		elapsedTime = 0.0;
		canShow = !canShow;
	}

	if (canShow)
	{

		Renderer_DrawText("Hello Game", sizeof("Hello Game"), 5,5);
	}


}

void render()
{
	Renderer_Flip();
}

void cleanup()
{
	Renderer_Cleanup();
}

int32 App_Run()
{
	atexit(cleanup); // ���α׷��� ����� �� cleanup�� �ڵ����� ����

	Timer_Init(60); // ���� ���� ��

	// Game Loop : ������ ���۽�Ű�� ����. ������(Frame)�̶�� �Ѵ�.
	while (true)
	{
		if (Timer_Update())
		{
			processInput();
			update();
			render();
		}
	}
	return 0;
}