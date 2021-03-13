#pragma once
#include "../gameNode.h"
#include "../link/link.h"
#include "../UIManager/UIManager.h"
#include "../MAPS/mapManager.h"
#include "../monster/monstermanager.h"

class gamePlayScene :public gameNode
{
private: //JAYC AKA 163
	mapManager* _mapManager;
private:
	//�׳�׳粨
	link* _link;
private:
	//���� ���� ��
	UIManager _UIM; //User Interface Manager
	//UIManager* _UIM; //Declaration for dynamic allocation
private:
	//����� ��~~~
	monstermanager* _mm;

private:
	//��Ʋ�!
public:
	HRESULT init();
	void release();
	void update();
	void render();

public:

	gamePlayScene();
	~gamePlayScene();
};

