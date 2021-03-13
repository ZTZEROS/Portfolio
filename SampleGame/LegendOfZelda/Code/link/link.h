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
	LIFTPOTMOVE,		// 2016. 01. 08 여까지 4방향 구현 완료
	LIFTGRASS,
	LIFTGRASSSTAND,
	LIFTGRASSMOVE,
	WEAPONSTAND,		// 2016. 01. 08 추가
	WEAPONMOVE,
	ATTACK,				// 일단 여까지 완료
	CHARGE,				// 완료
	FULLCHARGESTAND,	// 완료
	FULLCHARGEMOVE,
	CHARGEATTACK,		// 원형 공격(필살기) 완료
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
	float _shadowX;			// 픽셀탐지 및 z-order 겸용
	float _shadowY;			// 픽셀탐지 및 z-order 겸용 
	float _speed;
	float _bodyRcWidth;
	float _bodyRcHeight;
	int _maxHp;
	int _maxSp;
	int _currentHp;
	int _currentSp;
	int _frameCount;
	int _pushCount;			// 푸시 상태 값 변경을 위한 상태값
	int _chargeCount;
	int _fireDirection;
	LINKSTATE _linkState;	// 링크 현재 상태 값
	LINKDIRECTION _linkDir;	// 링크 방향
	LINKDIRECTION _fullChargeDir;
	ITEMSTATE _itemState;	// 링크 아이템 상태 값

	bool _weaponEquip;		// 무기 장착 여부
	bool _bedJumpStart;		// 오프닝 후 침대 점프 했는지 여부
	bool _nonLiftObject;	// 픽셀 검사 시 들 수 없는 오브젝트
	bool _liftObject;		// 픽셀 검사 시 들 수 있는 오브젝트
	bool _itemOpen;			// 아이템 오픈
	bool _nonLiftFrameStart;// 들 수 없는 물건 프레임 시작했냐?

	bool _cameraOnX;		// 카메라 세팅
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

	void frameUpdate(void);			// 링크 상태에 따른 모션 처리 (프레임 업데이트)
	void keyControl(void);		// 사용자 입력 처리에 따른 상태 변화
	void imageChange(void);		// 키 컨트롤 시 모션 변화에 따라 이미지 갈아 끼워주는 함수
	//void rectChange(void);		// 모션 변화에 따른 렉트 갱신함수, 그림자 좌표 갱신
	void pixelCollision(LINKDIRECTION dir);		// 모든 충돌 관련 처리 함수

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


	// 상호참조 주소링크
	void setUIManagerAddressLink(UIManager* UIManager) { _UIManager = UIManager; }
	void setMapManagerAddressLink(mapManager* mapManager) { _linkWorld = mapManager; }

	link();
	~link();
};

