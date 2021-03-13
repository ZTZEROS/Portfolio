#include "stdafx.h"
#include "SceneManager.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	Scene[SceneType::SCENE_TEST] = new TestScene;
	Scene[SceneType::SCENE_OPENING] = new OpeningScene;
	Scene[SceneType::SCENE_TOWN] = new TownScene;
	Scene[SceneType::SCENE_DUNGEON] = new DungeonScene;
	Scene[SceneType::SCENE_LOADING] = new LoadingScene;

	currentScene = SceneType::SCENE_OPENING;
	Scene[currentScene]->OnEnterState();
}

void SceneManager::Destroy()
{
	Scene[currentScene]->OnLeaveState();
	for (int i = 0; i < SceneType::SCENE_END; ++i)
	{
		SAFE_DELETE(Scene[i]);	// OnLeaveState안에 들어있음
	}
}

void SceneManager::Update()
{
	Scene[currentScene]->Update();

}

void SceneManager::Render()
{
	Scene[currentScene]->Render();
}

LRESULT SceneManager::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Scene[currentScene]->InputProc(hWnd, message, wParam, lParam);
	return 0;
}

bool SceneManager::ChangeScene(SceneType state)
{
	bool result = false;
	if (Scene[state])
	{
		Scene[currentScene]->OnLeaveState();
		currentScene = state;
		Scene[currentScene]->OnEnterState();

		result = true;
	}

	return result;
}
