#pragma once

#include <Windows.h>
#include <Windowsx.h>

#include <assert.h>

#include <list>
#include <map>
#include <vector>
#include <string>

// Windows 헤더 파일:

#include <d3d9.h>
#include <d3dx9.h>

#include "MemoryLeakCheck.h"

#include "FVF.h"
#include "Triangle.h"
#include "Particle.h"
#include "GlobalEnumeration.h"

#include "Collision/BoundingVolume.h"
#include "Collision/Collision.h"

#include "Util/RandomUtil.h"
#include "Util/Ray.h"
#include "Util/RectMake2D.h"

#include "Action/ActionBase.h"
#include "Action/ActionMove.h"
#include "Action/ActionSequence.h"

#include "Mesh/AseFrame.h"
#include "Mesh/AseLoader.h"
#include "Mesh/MaterialTexture.h"
#include "Mesh/ObjFileGroup.h"
#include "Mesh/ObjLoader.h"
#include "Mesh/Obj.h"
#include "Mesh/SkinnedMesh.h"

#include "Object/GameObject.h"
#include "Object/Character.h"

//#include "./Panel/Panel_AlphaMaterial.h"
//#include "./Panel/Panel_AlphaTexture.h"
//#include "./Panel/Panel_Alpha.h"
//#include "./Panel/Panel_ColorOP.h"

#include "Manager/SoundManager.h"
#include "Manager/SkinnedMeshManager.h"
#include "Manager/UI_Manager.h"
#include "Manager/TextureManager.h"
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"



//Heroes
//#include "./ItemBase.h"
#include "../UI_Mnager.h"

//Heroes/DataBase
#include "../CharacData.h"
#include "../ItemData.h"
//#include "../Heroes/Database/CharacData.h"
//#include "../Heroes/Database/ItemData.h"



#pragma comment(lib, "winmm.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MapToolProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK OptionProcedure(HWND, UINT, WPARAM, LPARAM);

static const WCHAR WINDOW_CLASS_NAME[] = L"Win32App";
static const WCHAR MAP_TOOL_WINDOW_CLASS_NAME[] = L"MapTool";
static const WCHAR OPTION_WINDOW_CLASS_NAME[] = L"Option";
static const WCHAR WINDOW_TITLE_CAPTION[] = L"SGA 3D";

static const unsigned int RESOLUTION_X = 1280;
static const unsigned int RESOLUTION_Y = 720;


template<typename T>
ULONG SAFE_RELEASE(T& object)
{
	ULONG result = 0;
	if (object)
	{
		result = object->Release();
		object = nullptr;
	}
	return result;
}

template<typename T>
void SAFE_DELETE(T& object)
{
	if (object)
	{
		object->Destroy();
		delete object;
		object = nullptr;
	}
}

template<typename T>
void SAFE_ARRAY_DELETE(T& object, UINT32 arrayCount)
{
	if (object)
	{
		for (int i = 0; i < arrayCount; ++i)
		{
			object[i]->Destroy();
		}
		delete[] object;
		object = nullptr;
	}
}

template<typename T>
void SAFE_DELETE_ARRAY_ONLY(T& object)
{
	if (object)
	{
		delete[] object;
		object = nullptr;
	}
}

extern HWND HWnd[3];
extern HINSTANCE HInstance;