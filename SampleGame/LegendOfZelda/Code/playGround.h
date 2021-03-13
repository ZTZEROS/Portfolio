#pragma once
#include "gameNode.h"
#include "scene/introScene.h"
#include "scene/selectScene.h"
#include "scene/gamePlayScene.h"

enum SCENE
{
	INTRO,
	SELECT,
	GAMEPLAY
};

class playGround : public gameNode
{
private:
	//****************************//
	//			노드변수			  //
	//****************************//
	gameNode* _intro;
	gameNode* _select;
	gameNode* _gamePlay;

private:
	SCENE _scene;
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	void sceneInit(); //씬 등록해주는 함수 (유지보수편의를 위한 함수)
	void imagePack(); //이미지 등록해주는 함수~
	void sceneUpdate();
	playGround();
	~playGround();
};

