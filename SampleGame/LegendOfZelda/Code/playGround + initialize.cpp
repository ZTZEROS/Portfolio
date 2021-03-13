#include "stdafx.h"
#include "playGround.h"

void playGround:: sceneInit()
{
	//***************************************//
	//					씬 등록				 //
	//***************************************//
	_intro = new introScene;
	_select = new selectScene;
	_gamePlay = new gamePlayScene;

	SCENEMANAGER->addScene("인트로", _intro);
	SCENEMANAGER->addScene("선택", _select);
	SCENEMANAGER->addScene("게임", _gamePlay);

	SCENEMANAGER->changeScene("인트로"); //처음씬은 인뜨로~!
}