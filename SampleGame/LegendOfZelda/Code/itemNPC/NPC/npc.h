#pragma once
#include "../../gameNode.h"
#include "../../link/link.h"
enum NPCSTATE
{
	STATE_ECO,
	STATE_NONE,
	STATE_ECO2,
	STATE_UP,
	STATE_DOWN,
	STATE_WALK_DOWN,
	STATE_LEFT,
	STATE_WALK_LEFT,
	STATE_RIGHT,
	STATE_WALK_RIGHT,
	STATE_DIE_WEAPON,
	STATE_DIED,
	STATE_SIT,
	STATE_SIT_LEFT,
	STATE_END
};
struct tagFrameImageNpc
{
	image* img;		//이미지 키값
	int nCount;		
	int nStart;		//시작프레임
	int nEnd;		//끝프레임
	int nRemainder;	//프레임속도
	int nCurrentX;	
};

class npc : public gameNode
{
protected:	
	POINT ptBodypos;
	tagFrameImageNpc _npcFImg[STATE_END];
	NPCSTATE _nState;
	image* _image;
	RECT _npcRc1;	//프레임이미지렉트
	
	int _frameSpeed;
	int _framecount;
	float _x, _y;

	int textframe;		//텍스트 한글자 한글자 출력

	bool Talking;
	int CurrentMenu;

	//1. 대사들을 저장할 맵을 만든다.
	typedef map<string, string>mapDialogueList;
	//2. 맵을 관리할 Iterator도 만들어준다.
	typedef map<string, string>::iterator mapDialogueIter;

	mapDialogueList _mList;
	mapDialogueIter _mIter;

public:
	HRESULT init(void);
	void release(void);
	virtual void update(void);
	virtual void render(void);
	
	//npc 이미지 (이미지좌표X, 이미지좌표Y)
	void npcUP();
	void npcDOWN();
	void npcLEFT();
	void npcRIGHT();
	void npcWEAPON();
	void npcDIE();
	void npcSIT();
	void npcSITLEFT();
	
	//npc 프레임 이미지 (시작프레임, 끝프레임, 프레임속도, 이미지좌표X, 이미지좌표Y)
	void npcWALKDOWN();
	void npcWALKLEFT();
	void npcWALKRIGHT();

	void framePlay();

	bool GetTalking() { return Talking; }
	int GetCurrentMenu() { return CurrentMenu; }

	void setNPCCoordinateY(int y) { ptBodypos.y = y; }
	int getNPCCoordinateY(void) { return ptBodypos.y; }
	void setNPCCoordinateX(int X) { ptBodypos.x = X; }
	int getNPCCoordinateX(void) { return ptBodypos.x; }
	void setNPCState(NPCSTATE npcState) { _nState = npcState; }
	mapDialogueList getmList(void) { return _mList; }
	RECT getnpcRc(void) { return _npcRc1; }

	void SetTalking(bool talking) { Talking = talking; }
	void SetCurrentMenu(int currentMenu) { CurrentMenu = currentMenu; }

	npc();
	~npc();
};
