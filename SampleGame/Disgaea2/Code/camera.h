#pragma once
#include "singletonBase.h"

class UIManager;
class camera : public singletonBase<camera>
{
private:
	UIManager* _UIManager;
	int _cameraX, _cameraY;
	POINT _focus;

	bool _zoomIn, _zoomOut;
	int _wheelValue;
	int _maxValue;

	bool _shakeOn;
	int _shakeTime;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	
	int getCameraX() { return _cameraX; };
	void setCameraX(int camX) { _cameraX = camX; }

	int getCameraY() { return _cameraY; };
	void setCameraY(int camY) { _cameraY = camY; }

	int getFocusX() { return _focus.x; }
	void setFocusX(int focusX) { _focus.x = focusX; }

	int getFocusY() { return _focus.y; }
	void setFocusY(int focusY) { _focus.y = focusY; }

	bool getZoomIn() { return _zoomIn; }
	void setZoomIn(bool zoomIn) { _zoomIn = zoomIn; }

	bool getZoomOut() { return _zoomOut; }
	void setZoomOut(bool zoomOut) { _zoomOut = zoomOut; }

	int getWheelValue() { return _wheelValue; }
	void setWheelValue(int wheelValue) { _wheelValue = wheelValue; }

	int getMaxValue() { return _maxValue; }
	void setMaxValue(int maxvalue) { _maxValue = maxvalue; }

	bool getShake() { return _shakeOn; }
	void setShake(bool shake, int shakeTime);
	void setShake(bool shake) { _shakeOn = shake; }

	int getShakeTime() { return _shakeTime; }

	void addressUIManager(UIManager* UIManager) { _UIManager = UIManager; }
	camera();
	~camera();
};

