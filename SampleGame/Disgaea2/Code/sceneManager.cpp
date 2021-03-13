#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

//�ε� �����带 ���� ������ �Լ�
DWORD CALLBACK loadingThread(LPVOID prc)
{
	//��ü�� ���� init(�ʱ�ȭ) �Լ� ����
	sceneManager::_readyScene->init();

	//�غ�� ���� ���� ������!
	sceneManager::_currentScene = sceneManager::_readyScene;

	//�ε��� ����
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
	//�ٲٷ��ϰ� �ϴ� ���� ã�´�
	mapSceneIter find = _mSceneList.find(sceneName);

	//�� ���� ã�� ��������
	if (find == _mSceneList.end()) return E_FAIL;

	//�̹� �� ���̶��?
	if (find->second == _currentScene) return S_OK;

	//����Ǵ� ���� �ʱ�ȭ �����ϸ�
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
	//�ٲٷ��ϰ� �ϴ� ���� ã�´�
	mapSceneIter find = _mSceneList.find(sceneName);

	//�� ���� ã�� ��������
	if (find == _mSceneList.end()) return E_FAIL;

	//�̹� �� ���̶��
	if (find->second == _currentScene) return S_OK;

	//�ٲٷ��ϴ� �ε����� ã�´�
	mapSceneIter findLoading = _mLoadingSceneList.find(loadingSceneName);
	
	//����Ǵ� ���� �ʱ�ȭ �����ϸ�
	if (SUCCEEDED(find->second->init()))
	{
		//���� ���� �ִٸ� �������ش�
		if (_currentScene) _currentScene->release();

		//�ε����� ���� �����ش�.
		_loadingScene = findLoading->second;

		//�ε��� ���� ���� ���� �� ���� �����Ѵ�.
		_readyScene = find->second;

		CloseHandle(CreateThread(NULL, 0, loadingThread, NULL, 0, &_loadThreadID));

	}

	return E_FAIL;
}

