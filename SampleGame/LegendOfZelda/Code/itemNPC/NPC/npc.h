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
	image* img;		//�̹��� Ű��
	int nCount;		
	int nStart;		//����������
	int nEnd;		//��������
	int nRemainder;	//�����Ӽӵ�
	int nCurrentX;	
};

class npc : public gameNode
{
protected:	
	POINT ptBodypos;
	tagFrameImageNpc _npcFImg[STATE_END];
	NPCSTATE _nState;
	image* _image;
	RECT _npcRc1;	//�������̹�����Ʈ
	
	int _frameSpeed;
	int _framecount;
	float _x, _y;

	int textframe;		//�ؽ�Ʈ �ѱ��� �ѱ��� ���

	bool Talking;
	int CurrentMenu;

	//1. ������ ������ ���� �����.
	typedef map<string, string>mapDialogueList;
	//2. ���� ������ Iterator�� ������ش�.
	typedef map<string, string>::iterator mapDialogueIter;

	mapDialogueList _mList;
	mapDialogueIter _mIter;

public:
	HRESULT init(void);
	void release(void);
	virtual void update(void);
	virtual void render(void);
	
	//npc �̹��� (�̹�����ǥX, �̹�����ǥY)
	void npcUP();
	void npcDOWN();
	void npcLEFT();
	void npcRIGHT();
	void npcWEAPON();
	void npcDIE();
	void npcSIT();
	void npcSITLEFT();
	
	//npc ������ �̹��� (����������, ��������, �����Ӽӵ�, �̹�����ǥX, �̹�����ǥY)
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
