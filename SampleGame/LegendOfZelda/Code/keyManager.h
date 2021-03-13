#pragma once
#include "singletonBase.h"
#include <bitset>

//������ �� �ִ� Ű �� ���
#define KEYMAX 256

using namespace std;

class keyManager : public singletonBase<keyManager>
{
private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;

public:
	HRESULT init(void);
	void release(void);

	//Ű�� �ѹ� ������
	bool isOnceKeyDown(int key);
	//Ű�� ������ ����
	bool isOnceKeyUp(int key);
	//Ű�� ��� ������ ������
	bool isStayKeyDown(int key);
	//��� Ű�� ���ٸ� ==capslock, numLockó�� ���������� �ҵ���
	//Lima ȭ����!!!
	bool isToggleKey(int key);

	bitset<KEYMAX> getKeyUp() { return _keyUp; }
	bitset<KEYMAX> getKeyDown() { return _keyDown; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }

	keyManager();
	~keyManager();
};

