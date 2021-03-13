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
	//±×³ç±×³ç²¨
	link* _link;
private:
	//º´¹¬ ºê¶ó´õ ²¨
	UIManager _UIM; //User Interface Manager
	//UIManager* _UIM; //Declaration for dynamic allocation
private:
	//»ó¿ì»ó¿ì ²¨~~~
	monstermanager* _mm;

private:
	//°æ¾Æ²¨!
public:
	HRESULT init();
	void release();
	void update();
	void render();

public:

	gamePlayScene();
	~gamePlayScene();
};

