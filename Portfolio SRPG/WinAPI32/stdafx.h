#pragma once

#define PI          3.141592f
#define TORADIAN	0.017453f		//pi/180 의값
#define	TODEGREE	57.295779f		//180/pi 의값
#define M_PI 3.14159265358979323846

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일

//디버그,문자집합 매크로
#if defined UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#elif defined _MBCS
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif 


//c스타일
#include <windows.h> //winapi를 사용할수 있게 해준다.
#include <tchar.h>
#include <time.h>
#include <assert.h>
#include <math.h>

//c++ stl 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <thread>
#include <cmath>
#include <unordered_map>
#include <unordered_set>



#ifdef _MBCS
#endif
//_tchar
//유니코드 사용
#ifdef _UNICODE
#define _tstring wstring
#define _tcout   wcout
#else // _MBCS
#define _tstring string
#define _tcout   cout
#endif

#define MAKEGET(Type, Value)	\
	Type Get##Value(){return Value;} 
#define MAKESET(Type, Value)	\
	void Set##Value(Type val){Value = val;}
#define MAKEGETSET(Type, Value) \
	MAKEGET(Type, Value) MAKESET(Type, Value)
#define MAKEREFGET(Type, Value)	\
	Type& GetRef##Value(){return Value;}

//dx 라이브러리 추가
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dX10.h>
#include <d3dx10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//Imgui
#include "imgui/imgui.h"
#include "imgui/imguiDx11.h"
#pragma comment(lib, "imgui/imgui.lib")

//FMOD 라이브러리 추가
//#pragma comment (lib,"Lib/fmod/fmodex_vc.lib")
//#include "Lib/fmod/fmod.hpp"
#pragma comment (lib,"fmod/fmodex_vc.lib")
#include "fmod/fmod.hpp"

//부스트 라이브러리 추가



//D2D
#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

//Direct Write
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")



//typedef 별명
typedef D3DXVECTOR2 Vector2; //x,y
typedef D3DXVECTOR3 Vector3;//x,y,z
typedef D3DXVECTOR4 Vector4;//x,y,z,w
typedef D3DXMATRIX	Matrix; 
typedef D3DXCOLOR	Color;

//매크로 함수
#define SAFE_DELETE(p) {if(p){delete(p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[](p); (p) = nullptr;}}
#define SAFE_RELEASE(p)  {if(p){(p)->Release(); (p)=nullptr;}}



using namespace std;

//윈도우 창
//시작좌표
#define     WINSTARTX 100
#define     WINSTARTY 100
//너비,높이
#define     WINSIZEX 1600
#define     WINSIZEY 900

#define     FIXFRAME 60

#include "Scene/Scene.h"
#include "Obect/Vertex.h"
#include "Obect/Shader.h"

#include "Util/Collision.h"
#include "Util/Math.h"

#include "Obect/Object.h"
#include "Obect/Star.h"
#include "Obect/Rect.h"
#include "Obect/Circle.h"
#include "Obect/Line.h"
#include "Obect/Image.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileMapEdit.h"

#include "Util/ObjectCollision.h"

#include "Singleton/TimeManager.h"
#define TIMEMANAGER TimeManager::GetInstance()
#define DeltaTime   TimeManager::GetInstance()->GetElapsedTime()

#include "Singleton/CamManager.h"
#define MAINCAM CamManager::GetInstance()

#include "Singleton/KeyManager.h"
#define KEYMANAGER KeyManager::GetInstance()

#include "Singleton/SceneManager.h"
#define SCENEMANAGER SceneManager::GetInstance()

#include "Singleton/SoundManager.h"
#define SOUNDMANAGER SoundManager::GetInstance()

#include "Singleton/WriteManager.h"
#define WRITEMANAGER WriteManager::GetInstance()

//내가 쓸 클래스
#include "Shooting/Player.h"
#include "Shooting/Enemy.h"





#define     UP      Vector2(0.0f,1.0f)
#define     DOWN    Vector2(0.0f,-1.0f)
#define     RIGHT   Vector2(1.0f,0.0f)
#define     LEFT    Vector2(-1.0f,0.0f)

#include    "Scene/Scene1.h"
#include    "Scene/Scene2.h"

#include    "MainGame.h"

//전역 변수
extern HINSTANCE		            g_hInst;
extern HWND				            g_hwnd;
extern Vector2						g_MousePt;
extern IDXGISwapChain*				g_SwapChain;
extern ID3D11Device*				g_Device;
extern ID3D11DeviceContext*			g_DeviceContext;
extern ID3D11RenderTargetView*		g_RTV;  
extern float       g_MasterVolume;    
extern float       g_BackgroundVolume;