#include "stdafx.h"
#include "Scene.h"

#include "Framework.h"

Scene g_Scene;

static ESceneType s_nextScene = SCENE_NULL;

#pragma region START

#define SOLID 0
#define SHADED 1
#define BLENDED 2

typedef struct Start_Data
{
    Image		Start_BackGround_Image;
    float		Speed;
    int32		X;
    int32		Y;
    int32		Alpha;

} Start_Data;

void init_start(void)
{
    g_Scene.Data = malloc(sizeof(Start_Data));
    memset(g_Scene.Data, 0, sizeof(Start_Data));

    Start_Data* data = (Start_Data*)g_Scene.Data;

    Image_LoadImage(&data->Start_BackGround_Image, "main.png");

    data->X;
    data->Y;
    data->Alpha;
}

void update_start(void)
{
    Start_Data* data = (Start_Data*)g_Scene.Data;

    if (Input_GetKeyDown(VK_SPACE))
    {
        Scene_SetNextScene(SCENE_TITLE);
    }
}

void render_start(void)
{
    Start_Data* data = (Start_Data*)g_Scene.Data;

    data->Start_BackGround_Image.Width = 1920;
    data->Start_BackGround_Image.Height = 1080;
    Image_SetAlphaValue(&data->Start_BackGround_Image, 255);

    Renderer_DrawImage(&data->Start_BackGround_Image, 0, 0);
}

void release_start(void)
{
    Start_Data* data = (Start_Data*)g_Scene.Data;

    SafeFree(g_Scene.Data);
}

#pragma endregion

#pragma region TitleScene

const wchar_t* str1[] = {
   L"GAME START"
};

const wchar_t* str2[] = {
    L"�� ������ 1",
    L"�� ������ 2",
    L"�� ������ 3"
};

typedef struct TitleSceneData
{
    Text    GuideLine[50]; //[id][�ٹٲ�]
    Text    GuideLineMovingPage[3]; //[������3��][���������ڰ���]
    Text    ShadedSelect[3]; //[������3��][���������ڰ���]
    int32   FontSize;
    int32   RenderMode;
    int32   SelectNestScene;
    Image   BackGroundImage;
    int32   TextLine;
    int32   TotalLine;
    int32   ID;
    int32	X;
    int32	Y;
    int32	Alpha;
    Music   BGM;
    char    NowBGM[20];
    float   BGM_Volume;
    SoundEffect   SE;
    float   SE_Volume;
} TitleSceneData;

void init_title(void)
{
    // �̴� Ÿ��Ʋ �����Ͱ� �� ���� �����
    g_Scene.Data = malloc(sizeof(TitleSceneData));
    memset(g_Scene.Data, 0, sizeof(TitleSceneData));

    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    data->ID = 1;               // ID 1���� ����
    data->TextLine = 0;         // ID���� �ؽ�Ʈ �� 0���� ����
    data->FontSize = 18;        // ������ ��Ʈ ������ ����
    data->RenderMode = SOLID;   // ������� : ���ڸ� ������
    data->TotalLine = 0;


    // [ ���� ���� ]
    // BGM
    strcpy(data->NowBGM, csvFile.Items[data->ID + 1][BGM].RawData);
    Audio_LoadMusic(&data->BGM, csvFile.Items[data->ID + 1][BGM].RawData);
    Audio_Play(&data->BGM, INFINITY_LOOP);
    data->BGM_Volume = 0.5f;
    Audio_SetVolume(data->BGM_Volume);
    // SE
    Audio_LoadSoundEffect(&data->SE, "effect2.wav");
    Audio_PlaySoundEffect(&data->SE, 1);
    data->SE_Volume = 1.0f;
    Audio_SetEffectVolume(&data->SE, data->SE_Volume);


    // TODO : ���߿� �Լ��� �� �� �ִ��� �����ϱ�
    wchar_t* testtext = ParseToUnicode(csvFile.Items[data->ID+1][Text_s]); // csvFile.Items[ID+1][�÷���]

    for (int32 i = 0; i < 20; ++i)
    {
        wchar_t stringl[500] = L"";
        testtext = StringLine(testtext, stringl);
        Text_CreateText(&data->GuideLine[i], "d2coding.ttf", data->FontSize, stringl, wcslen(stringl));
        data->TotalLine++;
        if (*testtext == NULL) break;
    }
    // �������

    // ������ 3�� �̴�
    for (int32 i = 0; i < 3; ++i)
    {
        Text_CreateText(&data->GuideLineMovingPage[i], "d2coding.ttf", 16, str2[i], wcslen(str2[i]));
    }

    // ���̵� �ؽ�Ʈ �̴�
    for (int32 i = 0; i < 3; ++i)
    {
        Text_CreateText(&data->ShadedSelect[i], "d2coding.ttf", data->FontSize, str2[i], wcslen(str2[i]));
    }
    Image_LoadImage(&data->BackGroundImage, "Scene1Background.png");

    data->X;
    data->Y;
    data->Alpha;
}

void update_title(void)
{
    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    // ��ŸŸ�� ����
    static float elapsedTime;
    elapsedTime += Timer_GetDeltaTime();

    // �ð��� ������ �ؽ�Ʈ �� ��++
    if (elapsedTime >= 1.0f)
    {
        if (data->TextLine < 20)
        {
            data->TextLine++;
        }
        elapsedTime = 0.0f; 
    }

    // ���������� �ѱ�
    if (Input_GetKeyDown(VK_SPACE))
    {
        data->ID++;         // ID �������� �Ѿ

        // [ �ؽ�Ʈ ��� ]
        data->TextLine = 0; // �ؽ�Ʈ�� 0�ʱ�ȭ
        data->TotalLine = 0; // �� �� �� �ִ��� ī��Ʈ
        wchar_t* testtext = ParseToUnicode(csvFile.Items[data->ID + 1][Text_s]); // csvFile.Items[ID+1][�÷���]
        for (int32 i = 0; i < 20; ++i)
        {
            wchar_t stringl[500] = L"";
            testtext = StringLine(testtext, stringl);
            Text_CreateText(&data->GuideLine[i], "d2coding.ttf", data->FontSize, stringl, wcslen(stringl));
            data->TotalLine++;
            if (*testtext == NULL) break;
        }

        // [ ���� ���� ]
        if (strcmp(&data->NowBGM, csvFile.Items[data->ID + 1][BGM].RawData))
        {
            strcpy(data->NowBGM, csvFile.Items[data->ID + 1][BGM].RawData);
            Audio_LoadMusic(&data->BGM, csvFile.Items[data->ID + 1][BGM].RawData);
            Audio_Play(&data->BGM, INFINITY_LOOP);
        }
        Audio_LoadSoundEffect(&data->SE, "effect2.wav");
        Audio_PlaySoundEffect(&data->SE, 1);


    }

    // �ؽ�Ʈ ��ŵ
    if (Input_GetKeyDown(VK_RIGHT))
    {
        data->TextLine = 20;
    }

    // �����Ѻκ� �����ֱ�
    int SelectShadedIndex = 0;
    //&data->ShadedSelect[SelectShadedIndex] = str2[SelectShadedIndex];
    //if (0 <= SelectShadedIndex && SelectShadedIndex <= 3)
    //{
    //    if (Input_GetKeyDown(VK_DOWN))
    //    {
    //        SelectShadedIndex++;
    //    }
    //    if (Input_GetKeyDown(VK_UP))
    //    {
    //        SelectShadedIndex--;
    //    }
    //}
}

void render_title(void)
{
    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    bool isFinishTextLine = false;

    // ��ŸŸ���� �þ�� ���� �þ �ؽ�Ʈ �� ��ŭ ���

    for (int32 i = 0; i < data->TextLine && i < data->TotalLine; i++)
    {
        SDL_Color color = { .a = 255 };
        Renderer_DrawTextSolid(&data->GuideLine[i], 100, 100 + 28 * i, color);

        // �ؽ�Ʈ �ٿ� �ƹ��͵� ���� �ؽ�Ʈ ���� �������� ������ ������ ���
        //if (&data->GuideLine[i] == "" && &data->GuideLine[i+1] == "")
        //    isFinishTextLine = true;
    }

    // �ؽ�Ʈ �ٿ� �ƹ��͵� ���� �ؽ�Ʈ ���� �������� ������ ������ 3�� ���
    if (true)
    {
        for (int32 i = 0; i < 3; ++i)
        {
            SDL_Color color = { .a = 255 };
            Renderer_DrawTextSolid(&data->GuideLineMovingPage[i], 100, 600 + 30 * i, color);
        }
    }

    // ���õ� �� ���̵� ���
    SDL_Color bg = { .a = 255 };
    SDL_Color fg = { .r = 255, .g = 255, .a = 255 };
    Renderer_DrawTextShaded(&data->ShadedSelect, 400, 400, fg, bg);
    

    data->BackGroundImage.Width = 1920;
    data->BackGroundImage.Height = 1080;
    Image_SetAlphaValue(&data->BackGroundImage, 125);

    Renderer_DrawImage(&data->BackGroundImage, 0, 0);

    switch (data->RenderMode)
    {
    case SOLID:
    {
        SDL_Color color = { .a = 255 };
        Renderer_DrawTextSolid(&data->GuideLineMovingPage, 400, 400, color);
    }
    break;
    case SHADED:
    {
        SDL_Color bg = { .a = 255 };
        SDL_Color fg = { .r = 255, .g = 255, .a = 255 };
        Renderer_DrawTextShaded(&data->GuideLineMovingPage, 400, 400, fg, bg);
    }
    break;
    case BLENDED:
    {
        //Renderer_DrawImage(&data->TestImage, 400, 400);
        SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
        Renderer_DrawTextBlended(&data->GuideLineMovingPage, 400, 400, color);
    }
    break;
    }
}

void release_title(void)
{
    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    for (int32 i = 0; i < 50; ++i)
    {
        Text_FreeText(&data->GuideLine[i]);
    }
    for (int32 i = 0; i < 3; ++i)
    {
        Text_FreeText(&data->GuideLineMovingPage[i]);
    }
    for (int32 i = 0; i < 3; ++i)
    {
        Text_FreeText(&data->ShadedSelect[i]);
    }

    // [ ���� ���� ]
    Audio_FreeMusic(&data->BGM);
    Audio_FreeSoundEffect(&data->SE);

    SafeFree(g_Scene.Data);
}
#pragma endregion

#pragma region MainScene
#define GUIDELINE_COUNT 8


typedef struct MainSceneData
{
    Text      GuideLine[GUIDELINE_COUNT];
    Music      BGM;
    float      Volume;
    SoundEffect Effect;
    Image      Front;
    Image      BackGround;
    float      Speed;
    int32      X;
    int32      Y;
    int32      Alpha;
} MainSceneData;

void logOnFinished(void)
{
    LogInfo("You can show this log on stopped the music");
}

void log2OnFinished(int32 channel)
{
    LogInfo("You can show this log on stopped the effect");
}

void init_main(void)
{
    g_Scene.Data = malloc(sizeof(MainSceneData));
    memset(g_Scene.Data, 0, sizeof(MainSceneData));

    MainSceneData* data = (MainSceneData*)g_Scene.Data;

    for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
    {
        Text_CreateText(&data->GuideLine[i], "d2coding.ttf", 16, str2[i], wcslen(str2[i]));
    }

    Image_LoadImage(&data->Front, "main.png");

    Audio_LoadMusic(&data->BGM, "powerful.mp3");
    Audio_HookMusicFinished(logOnFinished);
    Audio_LoadSoundEffect(&data->Effect, "effect2.wav");
    Audio_HookSoundEffectFinished(log2OnFinished);
    Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 3000);

    data->Volume = 1.0f;

    data->Speed = 400.0f;
    data->X = 400;
    data->Y = 400;
    data->Alpha = 255;
}

void update_main(void)
{
    MainSceneData* data = (MainSceneData*)g_Scene.Data;

    if (Input_GetKeyDown('E'))
    {
        Audio_PlaySoundEffect(&data->Effect, 1);
    }

    if (Input_GetKeyDown('M'))
    {
        if (Audio_IsMusicPlaying())
        {
            Audio_Stop();
        }
        else
        {
            Audio_Play(&data->BGM, INFINITY_LOOP);
        }
    }

    if (Input_GetKeyDown('P'))
    {
        if (Audio_IsMusicPaused())
        {
            Audio_Resume();
        }
        else
        {
            Audio_Pause();
        }
    }

    if (Input_GetKey('1'))
    {
        data->Volume -= 0.01f;
        Audio_SetVolume(data->Volume);
    }

    if (Input_GetKey('2'))
    {
        data->Volume += 0.01f;
        Audio_SetVolume(data->Volume);
    }

    if (Input_GetKey(VK_DOWN))
    {
        data->Y += data->Speed * Timer_GetDeltaTime();
    }

    if (Input_GetKey(VK_UP))
    {
        data->Y -= data->Speed * Timer_GetDeltaTime();
    }

    if (Input_GetKey(VK_LEFT))
    {
        data->X -= data->Speed * Timer_GetDeltaTime();
    }

    if (Input_GetKey(VK_RIGHT))
    {
        data->X += data->Speed * Timer_GetDeltaTime();
    }

    if (Input_GetKey('W'))
    {
        data->BackGround.ScaleY -= 0.05f;
    }

    if (Input_GetKey('S'))
    {
        data->BackGround.ScaleY += 0.05f;
    }

    if (Input_GetKey('A'))
    {
        data->BackGround.ScaleX -= 0.05f;
    }

    if (Input_GetKey('D'))
    {
        data->BackGround.ScaleX += 0.05f;
    }

    if (Input_GetKey('K'))
    {
        data->Alpha = Clamp(0, data->Alpha - 1, 255);
        Image_SetAlphaValue(&data->BackGround, data->Alpha);
    }

    if (Input_GetKey('L'))
    {
        data->Alpha = Clamp(0, data->Alpha + 1, 255);
        Image_SetAlphaValue(&data->BackGround, data->Alpha);
    }

}

void render_main(void)
{
    MainSceneData* data = (MainSceneData*)g_Scene.Data;

    for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
    {
        SDL_Color color = { .a = 255 };
        Renderer_DrawTextSolid(&data->GuideLine[i], 100, 400 + 25 * i, color);
    }

    Renderer_DrawImage(&data->Front, 0, 0);
    Renderer_DrawImage(&data->BackGround, data->X, data->Y);
}

void release_main(void)
{
    MainSceneData* data = (MainSceneData*)g_Scene.Data;

    for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
    {
        Text_FreeText(&data->GuideLine[i]);
    }
    Audio_FreeMusic(&data->BGM);
    Audio_FreeSoundEffect(&data->Effect);

    SafeFree(g_Scene.Data);
}
#pragma endregion

//#pragma region ImageScene1
//
//const wchar_t* str3[] = {
//   L"GAME START"
//};
//
//typedef struct Scene1_Data
//{
//    Image      Scene1_BackGround;
//    float      Speed;
//    int32      X;
//    int32      Y;
//    int32      Alpha;
//    Text   GuideLine[10];
//    Text   TestText;
//    int32   FontSize;
//    int32   RenderMode;
//    int32      NextText;
//    int32      check;
//
//} Scene1_Data;
//
//void init_scene_1(void)
//{
//    g_Scene.Data = malloc(sizeof(Scene1_Data));
//    memset(g_Scene.Data, 0, sizeof(Scene1_Data));
//
//    Scene1_Data* data = (Scene1_Data*)g_Scene.Data;
//
//    Image_LoadImage(&data->Scene1_BackGround, "Background.png");
//    (data->Scene1_BackGround.ScaleX = 10);
//    (data->Scene1_BackGround.ScaleY = 10);
//
//    data->Speed = 400.0f;
//    data->X = 0;
//    data->Y = 0;
//    data->Alpha = 100;
//
//
//    data->FontSize = 50;
//    Text_CreateText(&data->TestText, "d2coding.ttf", data->FontSize, Data[GetCsvData(1)].Text, lstrlen(Data[GetCsvData(1)].Text));
//
//    data->RenderMode = SOLID;
//
//    data->NextText = false;
//    data->check = 0;
//}
//
//void update_scene_1(void)
//{
//    Scene1_Data* data = (Scene1_Data*)g_Scene.Data;
//
//    // ���߿� Ÿ��ġ���� �ϳ��� ��µ� ����
//
//}
//
//void render_scene_1(void)
//{
//
//    Scene1_Data* data = (Scene1_Data*)g_Scene.Data;
//
//
//    Image_SetAlphaValue(&data->Scene1_BackGround, data->Alpha);
//    Renderer_DrawImage(&data->Scene1_BackGround, 0, 0);
//
//    for (int32 i = 0; i < 10; ++i)
//    {
//        SDL_Color color = { .a = 255 };
//        Renderer_DrawTextSolid(&data->GuideLine[i], 400, 200 * i, color);
//    }
//
//    switch (data->RenderMode)
//    {
//    case SOLID:
//    {
//        SDL_Color color = { .a = 255 };
//        Renderer_DrawTextSolid(&data->TestText, 400, 400, color);
//    }
//    break;
//    case SHADED:
//    {
//        SDL_Color bg = { .a = 255 };
//        SDL_Color fg = { .r = 255, .g = 255, .a = 255 };
//        Renderer_DrawTextShaded(&data->TestText, 400, 400, fg, bg);
//    }
//    break;
//    }
//
//
//}
//
//void release_scene_1(void)
//{
//    Scene1_Data* data = (Scene1_Data*)g_Scene.Data;
//
//    for (int32 i = 0; i < 10; ++i)
//    {
//        Text_FreeText(&data->GuideLine[i]);
//    }
//    Text_FreeText(&data->TestText);
//
//    SafeFree(g_Scene.Data);
//
//}
//
//#pragma endregion

bool Scene_IsSetNextScene(void)
{
    if (SCENE_NULL == s_nextScene)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Scene_SetNextScene(ESceneType scene)
{
    assert(s_nextScene == SCENE_NULL);
    assert(SCENE_NULL < scene&& scene < SCENE_MAX);

    s_nextScene = scene;
}

void Scene_Change(void)
{
    assert(s_nextScene != SCENE_NULL);

    if (g_Scene.Release)
    {
        g_Scene.Release();
    }

    switch (s_nextScene)
    {
    case SCENE_START:
        g_Scene.Init = init_start;
        g_Scene.Update = update_start;
        g_Scene.Render = render_start;
        g_Scene.Release = release_start;
        break;
    case SCENE_TITLE:
        g_Scene.Init = init_title;
        g_Scene.Update = update_title;
        g_Scene.Render = render_title;
        g_Scene.Release = release_title;
        break;
    case SCENE_MAIN:
        g_Scene.Init = init_main;
        g_Scene.Update = update_main;
        g_Scene.Render = render_main;
        g_Scene.Release = release_main;
        break;
    }

    g_Scene.Init();

    s_nextScene = SCENE_NULL;
}