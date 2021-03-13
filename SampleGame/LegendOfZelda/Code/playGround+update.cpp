#include "stdafx.h"
#include "playGround.h"

void playGround:: sceneUpdate()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		switch (_scene)
		{
		case INTRO:
			_scene = SELECT, SCENEMANAGER->changeScene("¼±ÅÃ"), _intro->release();
			//_UIM.SetCurrentScene(SELECT);
			break;
		case SELECT:
			//_UIM.SetCurrentScene(GAMEPLAY);
			break;
		}
	}
}