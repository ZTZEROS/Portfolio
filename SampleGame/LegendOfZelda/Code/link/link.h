#pragma once
#include "../gameNode.h"

class UIManager;
class mapManager;
//class enemyManager;
//class

enum LINKSTATE
{
	SLEEP,
	BED_JUMP,
	STAND,
	MOVE,
	PUSH,
	NONLIFT,
	LIFTPOT,
	LIFTPOTSTAND,
	LIFTPOTMOVE,		// 2016. 01. 08 ������ 4���� ���� �Ϸ�
	LIFTGRASS,
	LIFTGRASSSTAND,
	LIFTGRASSMOVE,
	WEAPONSTAND,		// 2016. 01. 08 �߰�
	WEAPONMOVE,
	ATTACK,				// �ϴ� ������ �Ϸ�
	CHARGE,				// �Ϸ�
	FULLCHARGESTAND,	// �Ϸ�
	FULLCHARGEMOVE,
	CHARGEATTACK,		// ���� ����(�ʻ��) �Ϸ�
	HIT,
	HOLEDROP

};

enum LINKDIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum ITEMSTATE
{
	ITEM_EMPTY,
	ITEM_LAMP,
	ITEM_BOOMERANG
};


class link : public gameNode
{
private:
	image* _image;
	image* _fullChargeTwinkle;
	RECT _bodyRc;
	RECT _objectRc;
	RECT _swordRc;
	RECT _chargeAttackRc;
	RECT _boomerangRc;
	float _x;
	float _y;
	float _centerX;
	float _centerY;
	float _shadowX;			// �ȼ�Ž�� �� z-order ���
	float _shadowY;			// �ȼ�Ž�� �� z-order ��� 
	float _speed;
	float _bodyRcWidth;
	float _bodyRcHeight;
	int _maxHp;
	int _maxSp;
	int _currentHp;
	int _currentSp;
	int _frameCount;
	int _pushCount;			// Ǫ�� ���� �� ������ ���� ���°�
	int _chargeCount;
	int _fireDirection;
	LINKSTATE _linkState;	// ��ũ ���� ���� ��
	LINKDIRECTION _linkDir;	// ��ũ ����
	LINKDIRECTION _fullChargeDir;
	ITEMSTATE _itemState;	// ��ũ ������ ���� ��

	bool _weaponEquip;		// ���� ���� ����
	bool _bedJumpStart;		// ������ �� ħ�� ���� �ߴ��� ����
	bool _nonLiftObject;	// �ȼ� �˻� �� �� �� ���� ������Ʈ
	bool _liftObject;		// �ȼ� �˻� �� �� �� �ִ� ������Ʈ
	bool _itemOpen;			// ������ ����
	bool _nonLiftFrameStart;// �� �� ���� ���� ������ �����߳�?

	bool _cameraOnX;		// ī�޶� ����
	bool _cameraOnY;
	bool _wakeUp;


	bool _pushUp;
	bool _pushDown;
	bool _pushLeft;
	bool _pushRight;
	bool _pixelCollision;

	UIManager* _UIManager;
	mapManager* _linkWorld;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void frameUpdate(void);			// ��ũ ���¿� ���� ��� ó�� (������ ������Ʈ)
	void keyControl(void);		// ����� �Է� ó���� ���� ���� ��ȭ
	void imageChange(void);		// Ű ��Ʈ�� �� ��� ��ȭ�� ���� �̹��� ���� �����ִ� �Լ�
	//void rectChange(void);		// ��� ��ȭ�� ���� ��Ʈ �����Լ�, �׸��� ��ǥ ����
	void pixelCollision(LINKDIRECTION dir);		// ��� �浹 ���� ó�� �Լ�

	void hitDamage(int damage);

	// getter
	image* getImage(void) { return _image; }
	RECT getBodyRect(void) { return _bodyRc; }
	RECT getObjectRect(void) { return _objectRc; }
	LINKSTATE getLinkState(void) { return _linkState; }
	LINKDIRECTION getLinkDirection(void) { return _linkDir; }
	ITEMSTATE getLinkItemState(void) { return _itemState; }
	float getX(void) { return _centerX; }
	float getY(void) { return _centerY; }
	float getShadowX(void) { return _shadowX; }
	float getShadowY(void) { return _shadowY; }
	int getMaxHp(void) { return _maxHp; }
	int getMaxSp(void) { return _maxSp; }
	int getCurrentHp(void) { return _currentHp; }
	int getCurrentSp(void) { return _currentSp; }
	bool getWeaponEquip(void) { return _weaponEquip; }
	bool getItemOpen(void) { return _itemOpen; }
	bool getWakeUp(void) { return _wakeUp; }
	bool getPushUp(void) { return _pushUp; }
	bool getPushDown(void) { return _pushDown; }
	bool getPushLeft(void) { return _pushLeft; }
	bool getPushRight(void) { return _pushRight; }
	bool getPixelCollision(void) { return _pixelCollision; }
	mapManager* getMapManager(void) { return _linkWorld; }
	UIManager* getUIManager(void) { return _UIManager; }
	

	// setter
	void setX(float x) { _centerX = x; }
	void setY(float y) { _centerY = y; }
	void setLinkState(LINKSTATE state) { _linkState = state; }
	void setItemOpen(bool itemOpen) { _itemOpen = itemOpen; }
	void setCameraOnX(bool cameraOnX) { _cameraOnX = cameraOnX; }
	void setCameraOnY(bool cameraOnY) { _cameraOnY = cameraOnY; }
	void setItemState(ITEMSTATE itemState) { _itemState = itemState; }
	void setCurrentHp(int currentHp) { _currentHp = currentHp; }
	void setCurrentSp(int currentSp) { _currentSp = currentSp; }


	// ��ȣ���� �ּҸ�ũ
	void setUIManagerAddressLink(UIManager* UIManager) { _UIManager = UIManager; }
	void setMapManagerAddressLink(mapManager* mapManager) { _linkWorld = mapManager; }

	link();
	~link();
};

