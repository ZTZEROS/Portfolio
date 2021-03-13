#pragma once
#include "../txtData.h"
#include "../gameNode.h"
#include "../animation.h"


enum  SLOTS
{
	SLOT1,
	SLOT2,
	SLOT3,
	COPYPLAYER,
	DELETEPLAYER
};

enum SLOTCHARACTER
{
	NONE,
	SLOTEMPTY,
	SLOTOCCUPIED
};

enum SELECTIONSCREEN
{
	CHARACTER_SELECT,
	CHARACTER_NAMING,
	CHARACTER_DELETE
};

enum DELETIONSCREEN
{
	D_SLOT1,
	D_SLOT2,
	D_SLOT3,
	QUIT
};

enum ALPHABETLEVEL
{
	TOP,
	MIDDLE,
	BOTTOM,
	ROCKBOTTOM
};

typedef struct tagSlots
{
	SLOTCHARACTER slotCharacter;
};

class selectScene : public gameNode
{
private:
	vector<image*> _vTempImage;
	vector<image*>::iterator _viTempImage;
	vector<image*> _vImageSlot1;
	vector<string> _vStr;
	vector<string> _vName;
	vector<string> _vName2;
	vector<string> _vName3;
private:
	animation* _fairyAni; //요정 애니매이션~~
	SLOTS _slot;
	tagSlots _slots[3];
	SELECTIONSCREEN _selectionScreen;
	ALPHABETLEVEL _alphabetLevel;
	DELETIONSCREEN _deleteScreen;
private:
	image* _letterImage;
	RECT* _rcCapitalLetter;
	RECT _nameRect[6];
	RECT _endRect;

	POINT _alphabetSniper;
private:
	
	int _alphabetLoopX;
	int _horizontaliLineLoopY;
	int _fairyY;
	int _letterCount;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
public:
	void adjustCharacterInfo(void);
	void fairyInit(void);
	void fairyUpdate(void);
	void selectionUpdate(void);
	void selectionRender(void);
	void makeName(void);
	selectScene();
	~selectScene();
};

