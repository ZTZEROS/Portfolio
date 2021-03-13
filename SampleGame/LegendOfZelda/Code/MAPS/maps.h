#pragma once
#include "../gameNode.h"

#define DOORWIDTH 60
#define DOORHEIGHT 10

class maps : public gameNode
{
protected:
	image* _backgroundImage;
	image* _backgroundPixel;
	RECT _loopRect;
	RECT* _intraDoor;
	RECT* _interDoor;
	int _loopX;
	int _loopY;
	int _loopRectX;
	int _loopRectY;
	int _centerInterDoorX;
	int _centerInterDoorY;
	bool _linkIntheMap;
public:
	RECT* getInterDoor(void) { return _interDoor; }
	RECT* getIntraDoor(void) { return _intraDoor; }
	RECT getLoopRect(void) { return _loopRect; }
	bool getLinkInTheMap(void) { return _linkIntheMap; }
	int getLoopX(void) { return _loopX; }
	int getLoopY(void) { return _loopY; }
	int getCenterInterDoorX(void) { return _centerInterDoorX; }
	int getCenterInterDoorY(void) { return _centerInterDoorY; }
	int getLoopRectX(void) { return _loopRectX; }
	int getLoopRectY(void) { return _loopRectY; }
	void setLoopRectX(int loopRectX) { _loopRectX = loopRectX; }
	void setLoopRectY(int loopRectY) { _loopRectY = loopRectY; }
	void setLinkInTheMap(bool linkInTheMap) { _linkIntheMap = linkInTheMap; }
	void setLoopX(int loopX) { _loopX = loopX; }
	void setLoopY(int loopY) { _loopY = loopY; }
	void setCenterInterDoorX(int centerInterDoorX) { _centerInterDoorX = centerInterDoorX; }
	void setCenterInterDoorY(int centerInterDoorY) { _centerInterDoorY = centerInterDoorY; }
public:
	maps();
	~maps();
};