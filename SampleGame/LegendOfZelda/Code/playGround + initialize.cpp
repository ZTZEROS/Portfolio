#include "stdafx.h"
#include "playGround.h"

void playGround:: sceneInit()
{
	//***************************************//
	//					�� ���				 //
	//***************************************//
	_intro = new introScene;
	_select = new selectScene;
	_gamePlay = new gamePlayScene;

	SCENEMANAGER->addScene("��Ʈ��", _intro);
	SCENEMANAGER->addScene("����", _select);
	SCENEMANAGER->addScene("����", _gamePlay);

	SCENEMANAGER->changeScene("��Ʈ��"); //ó������ �ζ߷�~!
}