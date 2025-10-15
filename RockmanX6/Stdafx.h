#pragma once

// 경고 무시
#pragma warning(disable : 4005)

#include<SDKDDKVER.h>

#define WIN32_LEAN_AND_MEAN

// 디버깅 콘솔창
#pragma comment (linker, "/entry:WinMainCRTStartup /subsystem:console")

#include "./inc/FMOD/fmod.hpp"

// ! 라이브러리 추가
#pragma comment (lib, "msimg32.lib")		// 이미지 사용을 위한 기본 라이브러리
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "./lib/FMOD/fmodex64_vc.lib")

//! Windows 헤더 파일
#include <Windows.h>

//fmod 헤더


//! C 런타임 헤더 파일
// stdlib :  C언어의 표준 라이브러리 함수의 헤더 파일
#include <stdlib.h>

// malloc : C언어에서 메모리를 할당할때 사용한다
#include <malloc.h>

// memory : 메모리 관련 헤더파일 -> ex) memset / memcpy 같음 함수들이 있다
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>

//! C++ 런타임 헤더 파일
#include<iostream>

#include<chrono>
#include<random>
#include<vector>
#include<queue>
#include<map>

#include<unordered_map>

#include<cassert>
#include<bitset>

#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;


// ! D2D / D3D헤더파일
/*
#include<D2D1.h>
#include<d2d1helper.h>
#include<d3dx9.h>		// 자주쓰는데 오래된거임
#include<D3DX11.h>		// ㅈㄴ 자주 씀 -> 그래픽스 라이브러리 추가해야 사용 가능
#include<d3d11shader.h>	// 그래서 쉐이더를 꼭 가져와라
#include<dinput.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

ID2D1Factory*					_ID2DFactory = nullptr;
ID2D1HwndRenderTarget*			_ID2DRenderTarget = nullptr;
*/

// # 사용자 정의 헤더파일 # 
#include "CommonMacroFunction.h"
#include "RandomFunction.h"
#include "KeyManager.h"
#include "ImageManager.h"
#include "SoundManager.h"
#include "CameraManager.h"
#include "UiManager.h"
#include "EffectManager.h"
//#include "ObjectManager.h"
#include "SceneManager.h"
#include "Utils.h"
#include "TimeManager.h"
#include "TextManager.h"

using namespace MY_UTIL;

// # 싱글톤 #
#define RND RandomFunction::getSingleton()
#define KEYMANAGER KeyManager::getSingleton()
#define IMAGEMANAGER ImageManager::getSingleton()
#define SOUNDMANAGER SoundManager::getSingleton()
#define CAMERAMANAGER CameraManager::getSingleton()
#define UIMANAGER UiManager::getSingleton()
//#define OBJECTMANAGER ObjectManager::getSingleton()
#define SCENEMANAGER SceneManager::getSingleton()
#define TIMEMANAGER TimeManager::getSingleton()
#define TEXTMANAGER TextManager::getSingleton()
#define EFFECTMANAGER EffectManager::getSingleton()


//! 타이틀
#define WINNAME				(LPTSTR)(TEXT("WindowsAPI"))

//! 전체 화면
//#define FULLSCREEN

#ifdef FULLSCREEN

#define WINSTART_X			0
#define WINSTART_Y			0
#define WINSIZE_X			GetSystemMetrics(SM_CXSCREEN)
#define WINSIZE_Y			GetSystemMetrics(SM_CYSCREEN)
#define WINSTYLE			WS_POPUPWINDOW | WS_MAXIMIZE 
#define PI					3.1415926
#define MAGENTA				RGB(255, 0, 255)

#else

#define WINSTART_X			1000
#define WINSTART_Y			180
#define WINSIZE_X			960
#define WINSIZE_Y			720
#define WINSTYLE			WS_CAPTION | WS_SYSMENU
#define PI					3.1415926
#define MAGENTA				RGB(255, 0, 255)

constexpr int SCALE_FACTOR = 3;

#endif

#define SAFE_DELETE(p)			{if(p) {delete (p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = nullptr;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->release(); (p) = nullptr;}}

extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;
