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
// �����ڿ� �Ҹ��� �߰�~ �� _��)/
//===============================================



//�ʱ�ȭ ���⿡ �ϼ���!!
HRESULT playGround::init(void)
{
	gameNode::init(true);
	
	sceneInit(); //�� ��� �� �ʱ�ȭ����~~
	imagePack(); //�̹��� ���~~~

	return S_OK;
}

//�����Ҵ� �� �޸� ������ ���⿡!!
void playGround::release(void)
{
	gameNode::release();
	
}

//������ ���⿡!!!
void playGround::update(void)
{
	gameNode::update( );

	SCENEMANAGER->update();
	sceneUpdate();
}


//�׷��ִ� ���� ���⿡!
void playGround::render(void)
{
	if (_scene !=INTRO)
	{
		PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
		//****************************************************//
	
		SCENEMANAGER->render();
		
		//****************************************************//
		TIMEMANAGER->render(getMemDC());
		//����ۿ� �Ű� �׷��ش�
		this->getBackBuffer()->render(getHDC(), 0, 0);
	}
}

