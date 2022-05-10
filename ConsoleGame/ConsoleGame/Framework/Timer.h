#pragma once

/// <summary>
/// Ÿ�̸Ӹ� �ʱ�ȭ �Ѵ�.
/// </summary>
/// <param name="fps">������ FPS</param>
void Timer_Init(int32 fps);

/// <summary>
/// Ÿ�̸Ӹ� ������Ʈ �Ѵ�.
/// </summary>
/// <return>������Ʈ�� �����ߴٸ� true, �ƴϸ� false</return>
bool Timer_Update(void);

/// <summary>
/// ��ŸŸ���� ���Ѵ�.
/// </summary>
/// <param name="">DetaTime : �����Ӱ��� �ð�. ���� ���������κ��� ���� �������� ������ ������ �ɸ� �ð�</param>
float Timer_GetDeltaTime(void);

#define DELTA_TIME Timer_GetDeltaTime()