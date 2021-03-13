#pragma once
#include "gameNode.h"

//�ӵ��� ���� ���漱��
class animation;

class effect : public gameNode
{
protected:
	int			_x;					//����Ʈ ��ǥ��ġ left
	int			_y;					//����Ʈ ��ǥ��ġ top
		
	image*		_effectImage;		//����Ʈ �̹���
	animation*	_effectAnimation;	//����Ʈ �ִϸ��̼�
	BOOL		_isRunning;			//����Ʈ Ȱ��ȭ ����
	float		_elapsedTime;		//����Ʈ ����ð�


public:
	effect();
	virtual ~effect();

	virtual HRESULT init(image* effectImage, int frameW, int frameH, int fps, float elapsedTime);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	void startEffect(int x, int y);
	virtual void killEffect(void);

	BOOL getIsRunning(void) { return _isRunning; }
};

