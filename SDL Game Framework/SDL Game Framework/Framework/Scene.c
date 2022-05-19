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

typedef struct TitleSceneData
{
    // ��� ����
    int32   ID;

    // �ؽ�Ʈ ����
    Text    GuideLine[50];          // [id][�ٹٲ�]
    int32   TextLine;               // �ؽ�Ʈ ��
    int32   TotalLine;              // �� �������� üũ
    int32   FontSize;
    int32   RenderMode;
    bool    isSelect[200][3];       // ������ ������ ǥ�ø� ���� ����
    bool    TextStrong;

    // ����������
    int32	Pointer_X;
    int32	Pointer_Y;
    Text    SelectText[3];       // [������3��][���������ڰ���]
    int32   SelectId;            // �� �ε���
    int32   SelectMovingPage[3]; // �� ��ȯ��

    // �̹�������
    Image   BackGroundImage;
    Image   FrontImage;
    Image   Icon;
    int32	X;
    int32	Y;
    int32	Alpha;

    // �������
    Music   BGM;
    char    NowBGM[20];
    float   BGM_Volume;
    SoundEffect   SE;
    float   SE_Volume;

} TitleSceneData;

void init_title(void)
{ 
    // [ ���� ]
    // �̴� Ÿ��Ʋ �����Ͱ� �� ���� �����
    g_Scene.Data = malloc(sizeof(TitleSceneData));
    memset(g_Scene.Data, 0, sizeof(TitleSceneData));

    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    data->ID = 1;               // ID 1���� ����


    // [ �ؽ�Ʈ ]
    data->TextLine = 0;         // ID���� �ؽ�Ʈ �� 0���� ����
    data->FontSize = 18;        // ������ ��Ʈ ������ ����
    data->RenderMode = SOLID;   // �������� : ���ڸ� ������
    data->TotalLine = 0;
    memset(data->isSelect, false, sizeof(data->isSelect)); // ���� false�� �ʱ�ȭ

    // testtext�� Test_s �����߰�
    wchar_t* IdText = ParseToUnicode(csvFile.Items[data->ID + 1][Text_s]); // csvFile.Items[ID+1][�÷���]

    for (int32 i = 0; i < 20; ++i)
    {
        wchar_t stringl[500] = L""; //�ؽ�Ʈ �� ����
        IdText = StringLine(IdText, stringl); // IdText�ȿ��� �ι��� ���� �� ���� ��Ʈ�� ����
        Text_CreateText(&data->GuideLine[i], "HeirofLightBold.ttf", data->FontSize, stringl, wcslen(stringl));//��Ʈ�� ���
        data->TotalLine++;
        
        if (*IdText == NULL)
        {
            break; // ��Ż���� �÷��� �Ǵ°� ����
        }
    }


    // [ ������ ]
    data->SelectId = 0;
    // SelectText 1, 2, 3�� ������ cvs select1, 2, 3 ���ڿ� ����
    Text_CreateText(&data->SelectText[0], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select1_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select1_s])));
    Text_CreateText(&data->SelectText[1], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select2_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select2_s])));
    Text_CreateText(&data->SelectText[2], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select3_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select3_s])));
    // SelectMovingPage 1, 2, 3�� ������ cvs [MovingPage 1, 2, 3 ���ڿ� ����
    data->SelectMovingPage[0] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage1_i]);
    data->SelectMovingPage[1] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage2_i]);
    data->SelectMovingPage[2] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage3_i]);

    // [ ���� ]
    // BGM
    strcpy(data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
    Audio_LoadMusic(&data->BGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
    Audio_Play(&data->BGM, INFINITY_LOOP);
    data->BGM_Volume = 0.1f;
    Audio_SetVolume(data->BGM_Volume);
    // SE
    if (*ParseToAscii(csvFile.Items[data->ID + 1][SE]) != NULL)
    {
        Audio_LoadSoundEffect(&data->SE, ParseToAscii(csvFile.Items[data->ID + 1][SE]));
        if (ParseToInt(csvFile.Items[data->ID + 1][SE_loop]))
        {
            Audio_PlaySoundEffect(&data->SE, INFINITY_LOOP); // 1 : ���ѷ���
        }
        else
        {
            Audio_PlaySoundEffect(&data->SE, 0); // 0 : 1ȸ ���
        }
        data->SE_Volume = 1.0f;
        Audio_SetEffectVolume(&data->SE, data->SE_Volume);
    }

    // [ �̹��� ]
    Image_LoadImage(&data->BackGroundImage, "Background.jpg");
    Image_LoadImage(&data->Icon, "ICON.png");
    Image_LoadImage(&data->FrontImage, ParseToAscii(csvFile.Items[data->ID + 1][ImageFile_s]));

    data->X = 170;
    data->Y = 855;
    data->Alpha;

    //Audio_LoadMusic(&data->BGM, "powerful.mp3");
    //Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 3000);
}

void update_title(void)
{
    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    // ��ŸŸ�� ����
    static float elapsedTime;
    elapsedTime += Timer_GetDeltaTime();

    // �ð��� �����ϸ� �ؽ�Ʈ �� ��++
    if (elapsedTime >= 1.0f)
    {
        if (data->TextLine < 20)
        {
            data->TextLine++;
        }
        elapsedTime = 0.0f; 
    }

    if (Input_GetKeyDown(VK_SPACE))
    {
        if(data->TextLine < data->TotalLine)
        {
            data->TextLine = data->TotalLine;
        }
        else
        {
            Image_LoadImage(&data->FrontImage, ParseToAscii(csvFile.Items[data->ID + 1][ImageFile_s]));

            data->isSelect[data->ID][data->SelectId] = true;
            data->ID = data->SelectMovingPage[data->SelectId];         // ID �������� �Ѿ
            data->SelectId = 0; // ������ ������ 0���� �ʱ�ȭ
            data->TextLine = 0; // �ؽ�Ʈ�� 0�ʱ�ȭ
            data->TotalLine = 0; // �� �������� üũ
            data->X = 170;
            data->Y = 855;
       
            wchar_t* IdText = ParseToUnicode(csvFile.Items[data->ID + 1][Text_s]); // csvFile.Items[ID+1][�÷���]

            for (int32 i = 0; i < 20; ++i)
            {
                wchar_t stringl[500] = L""; //�ؽ�Ʈ �� ����
                IdText = StringLine(IdText, stringl); // IdText�ȿ��� �ι��� ���� �� ���� ��Ʈ�� ����
                Text_CreateText(&data->GuideLine[i], "HeirofLightBold.ttf", data->FontSize, stringl, wcslen(stringl));//��Ʈ�� ���
                data->TotalLine++;

                if (*IdText == NULL)
                {
                    break; // ��Ż���� �÷��� �Ǵ°� ����
                }
        }
    }

        // [ ������ ]
        Text_CreateText(&data->SelectText[0], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select1_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select1_s])));
        Text_CreateText(&data->SelectText[1], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select2_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select2_s])));
        Text_CreateText(&data->SelectText[2], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select3_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select3_s])));
        data->SelectMovingPage[0] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage1_i]);
        data->SelectMovingPage[1] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage2_i]);
        data->SelectMovingPage[2] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage3_i]);
            
        // [ ���� ]
        if (strcmp(&data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM])))
        {
            strcpy(data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
            Audio_LoadMusic(&data->BGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
            Audio_Play(&data->BGM, INFINITY_LOOP);
        }
        Audio_StopSoundEffect();
        if (*ParseToAscii(csvFile.Items[data->ID + 1][SE]) != NULL)
        {
            Audio_LoadSoundEffect(&data->SE, ParseToAscii(csvFile.Items[data->ID + 1][SE]));
            if (ParseToInt(csvFile.Items[data->ID + 1][SE_loop]))
            {
                Audio_PlaySoundEffect(&data->SE, INFINITY_LOOP); // 1 : ���ѷ���
            }
            else 
            {
                Audio_PlaySoundEffect(&data->SE, 0); // 0 : 1ȸ ���
            }
        }
        LogInfo("Now ID Loading... %d", data->ID);
    }




    // [ ������ ]
    int selectIDCount = 0;
    for (int i = 1; i < 3; i++)
    {
        if (data->SelectMovingPage[i] > 0)
        {
            selectIDCount++;
        }
    }

    if (Input_GetKeyDown(VK_UP) && data->SelectId > 0)
    {
        data->SelectId--;
        LogInfo("SelectId : %d", data->SelectMovingPage[data->SelectId]);
        data->Y -= 40;
    }
    if (Input_GetKeyDown(VK_DOWN) && data->SelectId < selectIDCount)
    {
        data->SelectId++;
        LogInfo("SelectId : %d", data->SelectMovingPage[data->SelectId]);
        data->Y += 40;
    }

}

void render_title(void)
{
    // [ �ؽ�Ʈ ]
    // ��ŸŸ���� �þ�� ���� �þ �ؽ�Ʈ �� ��ŭ ���
    for (int32 i = 0; i < data->TextLine && i < data->TotalLine; i++)
    for (int32 i = 0; i < data->TextLineCheck; i++)
    {
        Renderer_DrawTextSolid(&data->GuideLine[i], 200, 200 + 40 * i, color);
    }

    // [ ������ ]
    // �ؽ�Ʈ �ٿ� �ƹ��͵� ������ ������ 3�� ���
    if (data->TextLine >= data->TotalLine)
    {

        if (data->isSelect[data->ID][0])
        {
            SDL_Color color = { .r = 100, .g = 0, .b = 80, .a = 255 };          
            Renderer_DrawTextSolid(&data->SelectText[0], 200, 850, color);
        }
        else
        {
            SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = 255 };
            Renderer_DrawTextSolid(&data->SelectText[0], 200, 850, color);
        }
        

        if (data->isSelect[data->ID][1])
        {
            SDL_Color color = { .r = 100, .g = 0, .b = 80, .a = 255 };
            Renderer_DrawTextSolid(&data->SelectText[1], 200, 890, color);
        }
        else
        {
            SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = 255 };
            Renderer_DrawTextSolid(&data->SelectText[1], 200, 890, color);
        }
        

        if (data->isSelect[data->ID][2])
        {
            SDL_Color color = { .r = 100, .g = 0, .b = 80, .a = 255 };
            Renderer_DrawTextSolid(&data->SelectText[2], 200, 930, color);
        }
        else
        {
            SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = 255 };
            Renderer_DrawTextSolid(&data->SelectText[2], 200, 930, color);
        }

        data->Icon.Width = 40;
        data->Icon.Height = 30;
        Image_SetAlphaValue(&data->Icon, 255);
        Renderer_DrawImage(&data->Icon, data->X, data->Y); //xy �ʱⰪ 180, 850
        }


        
    // [ �̹��� ]
    data->BackGroundImage.Width = 1920;  
    data->BackGroundImage.Height = 1080;  // ��� �̹��� ������
    Image_SetAlphaValue(&data->BackGroundImage, 125); // ��� �̹��� ����
    Renderer_DrawImage(&data->BackGroundImage, 0, 0); // ��� �̹��� ��ǥ
    Image_SetAlphaValue(&data->BackGroundImage, 125);
    Image_SetAlphaValue(&data->FrontImage, 255);  // �� �̹��� ����
    Renderer_DrawImage(&data->FrontImage, 1100, 200);  // �� �̹��� ��ǥ
    Renderer_DrawImage(&data->FrontImage, 1100, 200);
}

void release_title(void)
{
    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
    // [ �ؽ�Ʈ ]
    for (int32 i = 0; i < 50; ++i)
    {
        Text_FreeText(&data->GuideLine[i]);
    }

    // [ ������ ]
    for (int32 i = 0; i < 3; ++i)
    for (int32 i = 0; i < 10; ++i)
        Text_FreeText(&data->SelectText[i]);
        Text_FreeText(&data->GuideLine[data->TextIdCheck][i]);
 
    // [ ���� ]
    Audio_FreeMusic(&data->BGM);
    Audio_FreeSoundEffect(&data->SE);
    Text_FreeText(&data->TestText);

    SafeFree(g_Scene.Data);
}
#pragma endregion

#pragma region �����

// #################################################### ����� ������ ####################################################
/*


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
//    Text_CreateText(&data->TestText, "HeirofLightBold.ttf", data->FontSize, Data[GetCsvData(1)].Text, lstrlen(Data[GetCsvData(1)].Text));
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
*/
// ###################################################################################################################

#pragma endregion



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
    //case SCENE_MAIN:
    //    g_Scene.Init = init_main;
    //    g_Scene.Update = update_main;
    //    g_Scene.Render = render_main;
    //    g_Scene.Release = release_main;
    //    break;
        break;
    }

    g_Scene.Init();

    s_nextScene = SCENE_NULL;
}