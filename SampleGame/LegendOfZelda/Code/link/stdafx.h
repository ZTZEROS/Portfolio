#pragma once

#include "../targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <iostream>
#include <tchar.h>

#include "../commonMacroFunction.h"
#include "../randomFunction.h"
#include "../keyManager.h"
#include "../imageManager.h"
#include "../timeManager.h"
#include "../effectManager.h"
#include "../soundManager.h"
#include "../sceneManager.h"
#include "../streamManager.h"
#include "../explosionManager.h"
#include "../utils.h"
#include "../collision.h"
#include "../txtData.h"
#include "../database.h"

using namespace std;
using namespace ZELING_UTIL;

//==================================================
// - 2015. 10. 16 ## 디파인문 ## 
//==================================================

#define WINNAME (LPTSTR)(TEXT("ZELING ZELING"))
#define WINSTARTX 50		
#define WINSTARTY 50			
#define WINSIZEX 1024	
#define WINSIZEY 768	
#define WINSTYLE WS_CAPTION | WS_SYSMENU

#define MAGENTA RGB(255,0,255)
#define IMAGESCALE 4
#define FIRSTFLOOR RGB(145,117,234)
#define SECONDFLOOR RGB(0,255,0)
#define OBJECT RGB(252,123,123)
#define BOARD RGB(251,248,0)
#define PIT RGB(205,55,243)
#define STAIRS RGB(0,22,186)
#define BUNGEE RGB(0,171,93)

#define SAFE_DELETE(p)			{if(p) { delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) { delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->release(); (p) = NULL;}}

#define RND randomFunction::getsingleton()
#define KEYMANAGER keyManager::getsingleton()
#define IMAGEMANAGER imageManager::getsingleton()
#define TIMEMANAGER timeManager::getsingleton()
#define EFFECTMANAGER effectManager::getsingleton()
#define EXPLOSIONMANAGER explosionManager::getsingleton()
#define STREAMMANAGER streamManager::getsingleton()
#define SOUNDMANAGER soundManager::getsingleton()
#define SCENEMANAGER sceneManager::getsingleton()
#define TXTDATA txtData::getsingleton()
#define DATABASE database::getsingleton()


//==============================================
// ## 2015.10.22 ## extern(전역) 선언
//==============================================

extern HINSTANCE _hInstance;
extern HWND _hWnd;
extern POINT _ptMouse;