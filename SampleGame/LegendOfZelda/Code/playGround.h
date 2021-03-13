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
	//			��庯��			  //
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

	void sceneInit(); //�� ������ִ� �Լ� (�����������Ǹ� ���� �Լ�)
	void imagePack(); //�̹��� ������ִ� �Լ�~
	void sceneUpdate();
	playGround();
	~playGround();
};

