#pragma once
#include "singletonBase.h"
#include <map>

class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<string, gameNode*> mapSceneList;
	typedef map<string, gameNode*>::iterator mapSceneIter;

private:
	static gameNode* _currentScene; //현재 씬
	static gameNode* _loadingScene;	//로딩 씬
	static gameNode* _readyScene;	//준비된 씬(갈아끼울)

	mapSceneList _mSceneList;
	mapSceneList _mLoadingSceneList;

	DWORD _loadThreadID;

public:
	
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	gameNode* addScene(string sceneName, gameNode* scene);

	gameNode* addLoadingScene(string loadingSceneName, gameNode* scene);

	HRESULT changeScene(string sceneName);
	HRESULT changeScene(string sceneName, string loadingSceneName);

	//로딩 쓰레드를 위한 쓰레드 함수
	friend DWORD CALLBACK loadingThread(LPVOID prc);
	

};

