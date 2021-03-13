#pragma once
#include "../../GameScene/SceneParent.h"
#include "../../GameScene/TestScene.h"
#include "../../GameScene/LoadingScene.h"
#include "../../GameScene/OpeningScene.h"
#include "../../GameScene/TownScene.h"
#include "../../GameScene/LoadingScene.h"
#include "../../GameScene/DungeonScene.h"

enum SceneType
{
	SCENE_TEST,
	SCENE_OPENING,
	SCENE_TOWN,
	SCENE_DUNGEON,
	SCENE_LOADING,
	SCENE_END
};

class SceneManager
{
private:
	SceneManager();
	SceneManager(const SceneManager& other);
	~SceneManager();

public:
	static SceneManager& Get()
	{
		static SceneManager instance;
		return instance;
	}

	void Initialize();
	void Destroy();
	void Update();
	void Render();
	LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool ChangeScene(SceneType state);
	static inline SceneType GetCurrentScene() { return Get().InstanceGetCurrentScene(); }

protected:
	SceneType currentScene = SceneType::SCENE_TEST;
	SceneParent* Scene[SceneType::SCENE_END];

	SceneType InstanceGetCurrentScene() { return currentScene; }
};

