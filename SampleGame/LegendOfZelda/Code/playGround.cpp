#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
	:_scene(INTRO)
{
}


playGround::~playGround()
{
}

//===============================================
// 생성자와 소멸자 잘가~ ㅠ _ㅠ)/
//===============================================



//초기화 여기에 하세요!!
HRESULT playGround::init(void)
{
	gameNode::init(true);
	
	sceneInit(); //씬 등록 및 초기화에요~~
	imagePack(); //이미지 등록~~~

	return S_OK;
}

//동적할당 등 메모리 해제는 여기에!!
void playGround::release(void)
{
	gameNode::release();
	
}

//연산은 여기에!!!
void playGround::update(void)
{
	gameNode::update( );

	SCENEMANAGER->update();
	sceneUpdate();
}


//그려주는 것은 여기에!
void playGround::render(void)
{
	if (_scene !=INTRO)
	{
		PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
		//****************************************************//
	
		SCENEMANAGER->render();
		
		//****************************************************//
		TIMEMANAGER->render(getMemDC());
		//백버퍼에 옮겨 그려준다
		this->getBackBuffer()->render(getHDC(), 0, 0);
	}
}

