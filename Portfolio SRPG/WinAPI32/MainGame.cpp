#include "stdafx.h"
#include "MainGame.h"

HRESULT MainGame::init()
{
    srand((UINT)time(NULL));
    ImGui::Create(g_hwnd, g_Device, g_DeviceContext);
    ImGui::StyleColorsClassic();


	int a;

    //게임을 실행할때 로딩이 길어진다.
    //Scene1 a ; //스택
    Scene1* sc1;
    sc1 = new Scene1(); //힙
    Scene2* sc2;
    sc2 = new Scene2();
    //SCENEMANAGER->SceneArray[0] = sc1;
    //SCENEMANAGER->SceneArray[0]->init();
    SCENEMANAGER->AddScene("TileMapEditor",sc1);
    

    //SCENEMANAGER->SceneArray[1] = sc2;
    //SCENEMANAGER->SceneArray[1]->init();
    SCENEMANAGER->AddScene("SCENE2",sc2);
    
    
    SOUNDMANAGER->addSound("BGM", "bgm.wav", true);
    SOUNDMANAGER->addSound("GUN", "gun.wav", false);
    //사운드 절대음량 조절
    //SOUNDMANAGER->play("BGM",0.3f*g_MasterVolume*g_BackgroundVolume);



    //각 씬에 생성한 플레이어 주소를 옮기기
    
    sc1->init();
    sc2->init();
    //현재씬은 씬1이다.
    //SCENEMANAGER->CurrentScene = SCENEMANAGER->SceneArray[0];
    //SCENEMANAGER->ChageScene("TileMapEditor");
    SCENEMANAGER->ChageScene("TileMapEditor");

	return S_OK;
}

void MainGame::release()
{
    
    SCENEMANAGER->ReleaseSingleton();
    TIMEMANAGER->ReleaseSingleton();
    KEYMANAGER->ReleaseSingleton();
    MAINCAM->ReleaseSingleton();
    SOUNDMANAGER->ReleaseSingleton();
    WRITEMANAGER->ReleaseSingleton();
    /*SCENEMANAGER->SceneArray[0]->release();
    SCENEMANAGER->SceneArray[1]->release();
    SAFE_DELETE(SCENEMANAGER->SceneArray[0]);
    SAFE_DELETE(SCENEMANAGER->SceneArray[1]);*/
    ImGui::Delete();
}

void MainGame::update()
{

	KEYMANAGER->Update();
    ImGui::Update();

    /*ImGui::SliderFloat("MasterVolume",&g_MasterVolume,0.0f,1.0f);
    ImGui::SliderFloat("BGMVolume",&g_BackgroundVolume,0.0f,1.0f);
    if(ImGui::Button("ChangeSound"))
    {
        SOUNDMANAGER->pause("BGM");
        SOUNDMANAGER->resume("BGM",0.3f*g_MasterVolume*g_BackgroundVolume);

    }*/

   SCENEMANAGER->CurrentScene->update();
   SOUNDMANAGER->Update();
}

void MainGame::render()
{
    Color background = Color(0.5f, 0.5f, 0.5f, 1.0f);//?
    //바탕색 깔기 g_RTV에 연결된 백버퍼가
    g_DeviceContext->ClearRenderTargetView(g_RTV, (float *)background);
    WRITEMANAGER->GetDc()->BeginDraw();
    TIMEMANAGER->Render();
    //카메라의 공간변환도 시켜줄수 있게 호출
    MAINCAM->CameraUpdate();
    ////////////////////////////////////////////////////////////



    //SceneArray[0]->render();
    //SceneArray[1]->render();

    SCENEMANAGER->CurrentScene->render();


   
    ////////////////////////////////////////////////////////////
    //back 과 front 를 교체하라는 명령어
    WRITEMANAGER->GetDc()->EndDraw();
    ImGui::Render();
    g_SwapChain->Present(0, 0);
  
}
