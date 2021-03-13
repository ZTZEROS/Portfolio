#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

//로딩 쓰레드를 위한 쓰레드 함수
DWORD CALLBACK loadingThread(LPVOID prc)
{
	//교체될 씬의 init(초기화) 함수 실행
	sceneManager::_readyScene->init();

	//준비된 씬을 현재 씬으로!
	sceneManager::_currentScene = sceneManager::_readyScene;

	//로딩씬 해제
	sceneManager::_loadingScene->release();
	sceneManager::_loadingScene = NULL;
	sceneManager::_readyScene = NULL;

	return 0;
}

gameNode* sceneManager::_currentScene = NULL;
gameNode* sceneManager::_loadingScene = NULL;
gameNode* sceneManager::_readyScene = NULL;

sceneManager::sceneManager()
{
}


sceneManager::~sceneManager()
{
}

HRESULT sceneManager::init()
{
	_currentScene = NULL;
	_loadingScene = NULL;
	_readyScene = NULL;

	return S_OK;
}

void sceneManager::release()
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			if (miSceneList->second == _currentScene) miSceneList->second->release();

			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else ++miSceneList;
	}

	_mSceneList.clear();
}

void sceneManager::update()
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render()
{
	if (_currentScene) _currentScene->render();
}

gameNode* sceneManager::addScene(string sceneName, gameNode* scene)
{
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

gameNode* sceneManager::addLoadingScene(string loadingSceneName, gameNode* scene)
{
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
	//바꾸려하고 하는 씬을 찾는다
	mapSceneIter find = _mSceneList.find(sceneName);

	//그 씬을 찾지 못했으면
	if (find == _mSceneList.end()) return E_FAIL;

	//이미 그 씬이라면?
	if (find->second == _currentScene) return S_OK;

	//변경되는 씬이 초기화 성공하면
	if (SUCCEEDED(find->second->init()))
	{
		if (_currentScene) _currentScene->release();
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT sceneManager::changeScene(string sceneName, string loadingSceneName)
{
	//바꾸려하고 하는 씬을 찾는다
	mapSceneIter find = _mSceneList.find(sceneName);

	//그 씬을 찾지 못했으면
	if (find == _mSceneList.end()) return E_FAIL;

	//이미 그 씬이라면
	if (find->second == _currentScene) return S_OK;

	//바꾸려하는 로딩씬을 찾는다
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);
	
	//변경되는 씬이 초기화 성공하면
	if (SUCCEEDED(find->second->init()))
	{
		//기존 씬이 있다면 해제해준다
		if (_currentScene) _currentScene->release();

		//로딩씬을 먼저 보여준다.
		_loadingScene = findLoading->second;

		//로딩이 끝난 다음 변경 될 씬을 셋팅한다.
		_readyScene = find->second;

		CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &_loadThreadID));

	}

	return E_FAIL;
}

