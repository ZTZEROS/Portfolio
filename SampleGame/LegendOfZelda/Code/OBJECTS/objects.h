#pragma once
#include "../gameNode.h"
#include "../animation.h"

#define HORIZONTALRANGE 300
#define ITEMSIZE 48
#define VERTICALRANGE HORIZONTALRANGE*4/3

enum OBJECTSTATE
{
	UNTOUCHED,
	TOUCHED,
	ATTACKED
};

enum FIREDIRECTION
{
	FIRE_UP,
	FIRE_DOWN,
	FIRE_LEFT,
	FIRE_RIGHT
};

struct tagObject
{
	image* objectImage;
	image* throwObjectImage;
	image* destroyObjectImage;
	animation* objectDestroyedAni;
	RECT* rcObject;
	RECT* rcThrowObject;
	OBJECTSTATE objectState;
	int speed;
	int centerX, centerY;
	int throwCenterX, throwCenterY;
	int startX, startY;
	bool isFire;
	bool isLift;
	bool throwObjectDestroy;
	int frameX;
};

class objects : public gameNode
{
protected:
	tagObject _objectInfo;

public:
	void isFire(int x, int y);
	void objectMove(FIREDIRECTION fireDirection);
	void deleteObject();
	void objectLifting(int x, int y);
	void objectDestroy(void);
public:
	//getter, setter
	void setObjectCoordinateY(int y) { _objectInfo.centerY = y; }
	int getObjectCoordinateY(void) { return _objectInfo.centerY; }
	void setObjectCoordinateX(int X) { _objectInfo.centerX = X; }
	int getObjectCoordinateX(void) { return _objectInfo.centerX; }
	void setThrowObjectY(int y) { _objectInfo.throwCenterY = y; }
	int getThrowObjectY(void) { return _objectInfo.throwCenterY; }
	void setThrowObjectX(int X) { _objectInfo.throwCenterX = X; }
	int getThrowObjectX(void) { return _objectInfo.throwCenterX; }
	void setObjectState(OBJECTSTATE objectState) { _objectInfo.objectState = objectState; }
	OBJECTSTATE getObjectState(void) { return _objectInfo.objectState; }
	RECT* getObjectRect(void) { return _objectInfo.rcObject; }
	RECT* getThrowObjectRect(void) { return _objectInfo.rcThrowObject; }
	image* getThrowImage(void) { return _objectInfo.throwObjectImage; }
	bool getisFire(void) { return _objectInfo.isFire; }
	bool getIsLift(void) { return _objectInfo.isLift; }
	bool getThrowObjectDestroy(void) { return _objectInfo.throwObjectDestroy; }
	void setIsLift(bool isLift) { _objectInfo.isLift = isLift; }
	
	objects();
	~objects();
};

