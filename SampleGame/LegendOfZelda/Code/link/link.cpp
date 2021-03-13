#include "stdafx.h"
#include "../stdafx.h"
#include "link.h"
#include "../UIManager/UIManager.h"
#include "../MAPS/mapManager.h"

link::link()
{
}


link::~link()
{
}


HRESULT link::init(void)
{
	_image = IMAGEMANAGER->findImage("linkSleep");
	//_x = 227;
	//_y = 270;
	_centerX = 287;
	_centerY = 335;
	_speed = 5;
	_bodyRc = RectMakeCenter(_centerX, _centerY, 60, 80);
	_objectRc = RectMake(0, 0, 0, 0);
	_maxHp = 6;
	_currentHp = 6;
	_maxSp = 10;
	_currentSp = 0;
	_itemState = ITEM_EMPTY;
	_linkState = SLEEP;
	_linkDir = RIGHT;
	_shadowX = _centerX;
	_shadowY = _centerY;
	_weaponEquip = false;
	_frameCount = 0;
	_pushCount = 0;
	_chargeCount = 0;
	_bedJumpStart = false;
	_bodyRcWidth = _bodyRc.right - _bodyRc.left;
	_bodyRcHeight = _bodyRc.bottom - _bodyRc.top;

	_nonLiftObject = false;
	_liftObject = false;
	_itemOpen = false;
	_nonLiftFrameStart = false;
	_wakeUp = false;

	_pushUp = false;
	_pushDown = false;
	_pushLeft = false;
	_pushRight = false;
	_pixelCollision = false;

	return S_OK;
}

void link::release(void)
{

}

void link::update(void)
{
	keyControl();

	frameUpdate();

	if (_linkWorld->getVJar().size() > 0)
	{
		for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
		{

			if (!_linkWorld->getVJar()[i]->getisFire()) continue;
			_linkWorld->getVJar()[i]->objectMove((FIREDIRECTION)_fireDirection);
		}
	}
	if (_linkWorld->getVGrass().size() > 0)
	{
		for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
		{

			if (!_linkWorld->getVGrass()[i]->getisFire()) continue;
			_linkWorld->getVGrass()[i]->objectMove((FIREDIRECTION)_fireDirection);
		}
	}


	_shadowX = _centerX;
	_shadowY = _centerY;
	_bodyRc = RectMakeCenter(_centerX, _centerY, 60, 80);
}

void link::render(void)
{
	RECT temp;
	Rectangle(getMemDC(), _bodyRc.left, _bodyRc.top, _bodyRc.right, _bodyRc.bottom);

	switch (_linkState)
	{
	case SLEEP:
		_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2, _image->getFrameX(), _image->getFrameY());
		break;
	case BED_JUMP:
		_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2, _image->getFrameX(), _image->getFrameY());
		break;
	case MOVE:
		switch (_linkDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 2,
				_image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 2,
				_image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 + 6, _centerY - _image->getFrameHeight() / 2 - 2,
				_image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 - 6, _centerY - _image->getFrameHeight() / 2 - 2,
				_image->getFrameX(), _image->getFrameY());
			break;
		}

		break;
	case PUSH:
		switch (_linkDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 + 4,
				_image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 + 4,
				_image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 + 4, _centerY - _image->getFrameHeight() / 2,
				_image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 - 4, _centerY - _image->getFrameHeight() / 2,
				_image->getFrameX(), _image->getFrameY());
			break;
		}

		break;
	case NONLIFT:
		switch (_linkDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 + 2,
				_image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2,
				_image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 + 18, _centerY - _image->getFrameHeight() / 2 + 4,
				_image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 - 18, _centerY - _image->getFrameHeight() / 2 + 4,
				_image->getFrameX(), _image->getFrameY());
			break;
		}

		break;
	case LIFTPOT:
		switch (_linkDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		}
		break;
	case LIFTPOTSTAND:
		switch (_linkDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		}
		break;
	case LIFTPOTMOVE:
		switch (_linkDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		}
		break;

	case ATTACK:
		switch (_linkDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 - 9, _centerY - _image->getFrameHeight() / 2 - 24, _image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 + 15, _centerY - _image->getFrameHeight() / 2 + 28, _image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 - 28, _centerY - _image->getFrameHeight() / 2 + 16, _image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 + 28, _centerY - _image->getFrameHeight() / 2 + 16, _image->getFrameX(), _image->getFrameY());
			break;
		}
		break;
	case CHARGE:
		switch (_fullChargeDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 + 6, _centerY - _image->getFrameHeight() / 2 - 20, _image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 - 4, _centerY - _image->getFrameHeight() / 2 + 26, _image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 - 18, _centerY - _image->getFrameHeight() / 2, _image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 + 18, _centerY - _image->getFrameHeight() / 2, _image->getFrameX(), _image->getFrameY());
			break;
		}
		break;
	case FULLCHARGESTAND:
		switch (_fullChargeDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 + 8, _image->getFrameX(), _image->getFrameY());
			_fullChargeTwinkle->frameRender(getMemDC(), _bodyRc.left - 5, _bodyRc.top - 35, _fullChargeTwinkle->getFrameX(),
				_fullChargeTwinkle->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 + 8, _image->getFrameX(), _image->getFrameY());
			_fullChargeTwinkle->frameRender(getMemDC(), _bodyRc.right - 55, _bodyRc.bottom + 20, _fullChargeTwinkle->getFrameX(),
				_fullChargeTwinkle->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 + 8, _image->getFrameX(), _image->getFrameY());
			_fullChargeTwinkle->frameRender(getMemDC(), _bodyRc.left - 55, _centerY - 20, _fullChargeTwinkle->getFrameX(),
				_fullChargeTwinkle->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 + 8, _image->getFrameX(), _image->getFrameY());
			_fullChargeTwinkle->frameRender(getMemDC(), _bodyRc.right + 15, _centerY - 20, _fullChargeTwinkle->getFrameX(),
				_fullChargeTwinkle->getFrameY());
			break;
		}
		break;
	case CHARGEATTACK:
		switch (_fullChargeDir)
		{
		case UP:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 + 10, _image->getFrameX(), _image->getFrameY());
			break;
		case DOWN:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2 - 6, _image->getFrameX(), _image->getFrameY());
			break;
		case LEFT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 + 12, _centerY - _image->getFrameHeight() / 2 - 2, _image->getFrameX(), _image->getFrameY());
			break;
		case RIGHT:
			_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2 - 8, _centerY - _image->getFrameHeight() / 2 - 2, _image->getFrameX(), _image->getFrameY());
			break;
		}
		break;

	default:
		_image->frameRender(getMemDC(), _centerX - _image->getFrameWidth() / 2, _centerY - _image->getFrameHeight() / 2, _image->getFrameX(), _image->getFrameY());
		break;
	}

	temp = RectMake(_shadowX, _shadowY, 10, 10);
	Rectangle(getMemDC(), temp.left, temp.top, temp.right, temp.bottom);
	Rectangle(getMemDC(), _objectRc.left, _objectRc.top, _objectRc.right, _objectRc.bottom);
	Rectangle(getMemDC(), _swordRc.left, _swordRc.top, _swordRc.right, _swordRc.bottom);


	//char str[128];
	//sprintf(str, "링크 푸쉬 카운트 :%d", _pushCount);
	//TextOut(getMemDC(), 100, 300, str, strlen(str));
	//
	//char str1[128];
	//sprintf(str1, "차지 카운트 : %d", _chargeCount);
	//TextOut(getMemDC(), 500, 420, str1, strlen(str1));

	char str3[128];
	char str4[128];
	sprintf(str3, "shadowX : %f", _shadowX);
	sprintf(str4, "shadowY : %f", _shadowY);
	TextOut(getMemDC(), 100, 360, str3, strlen(str3));
	TextOut(getMemDC(), 100, 380, str4, strlen(str4));

	char str1[128];
	char str2[128];
	sprintf(str2, "맵 루프량 X : %d", _linkWorld->getMapPixelLoopX());
	sprintf(str1, "맵 루프량 Y : %d", _linkWorld->getMapPixelLoopY());
	TextOut(getMemDC(), 100, 320, str1, strlen(str1));
	TextOut(getMemDC(), 100, 340, str2, strlen(str2));

}

void link::frameUpdate(void)
{
	_frameCount += TIMEMANAGER->getElapsedTime() * 100.0f;
	if (_linkState == MOVE || _linkState == WEAPONMOVE) _pushCount += TIMEMANAGER->getElapsedTime() * 100.0f;
	else _pushCount = 0;
	//if (_linkState == ATTACK) _chargeCount += TIMEMANAGER->getElapsedTime() * 100.0f;
	//else _chargeCount = 0;


	switch (_linkState)
	{
	case SLEEP:
		if (_frameCount % 10 == 0)
		{
			_image->setFrameX(_image->getFrameX() + 1);
			if (_image->getFrameX() >= _image->getMaxFrameX())
			{
				_image = IMAGEMANAGER->findImage("linkBedJump");
				_image->setFrameX(0);
				_linkState = BED_JUMP;
				_wakeUp = true;
				_centerX = 325;
				_centerY = 343;
			}
			_frameCount = 0;
		}
		break;
	case BED_JUMP:
		if (_frameCount % 2 == 0 && _bedJumpStart)
		{
			_image->setFrameX(_image->getFrameX() + 1);
			if (_image->getFrameX() >= _image->getMaxFrameX())
			{
				_image = IMAGEMANAGER->findImage("linkStand");
				_image->setFrameX(2);
				_image->setFrameY(0);
				_linkState = STAND;
				_linkDir = RIGHT;
				_centerX = 395;
				_centerY = 350;
			}
			_frameCount = 0;
		}
		break;
	case MOVE:
		switch (_linkDir)
		{
		case UP:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX()) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX()) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() <= 0) _image->setFrameX(_image->getMaxFrameX() - 1);
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX()) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		}
		break;

	case PUSH:
		switch (_linkDir)
		{
		case UP:
			if (_frameCount % 8 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX()) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 8 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX()) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 8 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() < 0) _image->setFrameX(_image->getMaxFrameX() - 1);
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 8 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX()) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		}
		break;
	case NONLIFT:
		if (_nonLiftFrameStart)
		{
			switch (_linkDir)
			{
			case UP:
				if (_frameCount % 8 == 0)
				{
					_image->setFrameX(_image->getFrameX() + 1);
					if (_image->getFrameX() >= _image->getMaxFrameX() - 2) _image->setFrameX(1);
					_frameCount = 0;
				}
				break;
			case DOWN:
				if (_frameCount % 8 == 0)
				{
					_image->setFrameX(_image->getFrameX() + 1);
					if (_image->getFrameX() >= _image->getMaxFrameX() - 2) _image->setFrameX(1);
					_frameCount = 0;
				}
				break;
			case LEFT:
				if (_frameCount % 8 == 0)
				{
					_image->setFrameX(_image->getFrameX() - 1);
					if (_image->getFrameX() <= 0) _image->setFrameX(2);
					_frameCount = 0;
				}
				break;
			case RIGHT:
				if (_frameCount % 8 == 0)
				{
					_image->setFrameX(_image->getFrameX() + 1);
					if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(1);
					_frameCount = 0;
				}
				break;
			}
		}
		break;
	case LIFTPOT:
		switch (_linkDir)
		{
		case UP:
			if (_frameCount % 10 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1)
				{
					_linkState = LIFTPOTSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 10 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1)
				{
					_linkState = LIFTPOTSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 10 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1)
				{
					_linkState = LIFTPOTSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 10 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() <= 0)
				{
					_linkState = LIFTPOTSTAND;
					imageChange();
				}
			}
			_frameCount = 0;
			break;
		}
		break;
	case LIFTPOTMOVE:
		switch (_linkDir)
		{
		case UP:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() <= 0) _image->setFrameX(3);
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		}
		break;
	case WEAPONMOVE:
		switch (_linkDir)
		{
		case UP:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() <= 0) _image->setFrameX(_image->getMaxFrameX() - 1);
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		}
		break;
	case ATTACK:
		switch (_linkDir)
		{
		case UP:
			if (_frameCount % 3 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() == 5)
				{
					_swordRc = RectMake(_bodyRc.left, _bodyRc.top - 55, 30, 50);

					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_swordRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_linkWorld->getVGrass()[i]->setObjectState(ATTACKED);
							break;
						}
					}
				}
				else _swordRc = RectMake(0, 0, 0, 0);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_image->setFrameX(0);
					_linkState = WEAPONSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 5 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() == 3)
				{
					_swordRc = RectMake(_shadowX, _bodyRc.bottom + 5, 30, 50);
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_swordRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_linkWorld->getVGrass()[i]->setObjectState(ATTACKED);
							break;
						}
					}
				}
				else _swordRc = RectMake(0, 0, 0, 0);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_image->setFrameX(0);
					_linkState = WEAPONSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 3 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() == 3)
				{
					_swordRc = RectMake(_bodyRc.left - 65, _shadowY, 50, 30);
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_swordRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_linkWorld->getVGrass()[i]->setObjectState(ATTACKED);
							break;
						}
					}
				}
				else _swordRc = RectMake(0, 0, 0, 0);
				if (_image->getFrameX() <= 0)
				{
					_image->setFrameX(_image->getMaxFrameX() - 1);
					_linkState = WEAPONSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 3 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() == 5)
				{
					_swordRc = RectMake(_bodyRc.right + 15, _shadowY, 50, 30);
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_swordRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_linkWorld->getVGrass()[i]->setObjectState(ATTACKED);
							break;
						}
					}
				}
				else _swordRc = RectMake(0, 0, 0, 0);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_image->setFrameX(0);
					_linkState = WEAPONSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		}
		break;
	case CHARGE:
		switch (_fullChargeDir)
		{
		case UP:
			if (_frameCount % 3 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_image->setFrameX(0);
					_linkState = FULLCHARGESTAND;
					_chargeAttackRc = RectMake(0, 0, 0, 0);
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 3 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_image->setFrameX(0);
					_linkState = FULLCHARGESTAND;
					_chargeAttackRc = RectMake(0, 0, 0, 0);
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 3 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() < 0)
				{
					_image->setFrameX(_image->getMaxFrameX() - 1);
					_linkState = FULLCHARGESTAND;
					_chargeAttackRc = RectMake(0, 0, 0, 0);
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 3 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_image->setFrameX(0);
					_linkState = FULLCHARGESTAND;
					_chargeAttackRc = RectMake(0, 0, 0, 0);
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		}
		break;
	case FULLCHARGESTAND:
		if (_frameCount % 2 == 0)
		{
			_fullChargeTwinkle->setFrameX(_fullChargeTwinkle->getFrameX() + 1);
			if (_fullChargeTwinkle->getFrameX() >= _fullChargeTwinkle->getMaxFrameX())
				_fullChargeTwinkle->setFrameX(0);
			_frameCount = 0;
		}
		break;
	case CHARGEATTACK:
		_chargeAttackRc = RectMakeCenter(_centerX, _centerY, 100, 100);
		if (_frameCount % 2 == 0)
		{
			_image->setFrameX(_image->getFrameX() + 1);
			if (_image->getFrameX() >= _image->getMaxFrameX())
			{
				_image->setFrameX(0);
				_linkState = WEAPONSTAND;
				_chargeAttackRc = RectMake(0, 0, 0, 0);
				imageChange();
			}
			_frameCount = 0;
		}
		break;
	case LIFTGRASS:
		switch (_linkDir)
		{
		case UP:
			if (_frameCount % 8 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_linkState = LIFTGRASSSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 8 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_linkState = LIFTGRASSSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 8 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() <= 0)
				{
					_linkState = LIFTGRASSSTAND;
					imageChange();
				}
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 8 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX())
				{
					_linkState = LIFTGRASSSTAND;
					imageChange();
				}
			}
			_frameCount = 0;
			break;
		}
		break;
	case LIFTGRASSMOVE:
		switch (_linkDir)
		{
		case UP:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case DOWN:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX() - 1) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case LEFT:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() + 1);
				if (_image->getFrameX() >= _image->getMaxFrameX()) _image->setFrameX(0);
				_frameCount = 0;
			}
			break;
		case RIGHT:
			if (_frameCount % 2 == 0)
			{
				_image->setFrameX(_image->getFrameX() - 1);
				if (_image->getFrameX() <= 0) _image->setFrameX(_image->getMaxFrameX() - 1);
				_frameCount = 0;
			}
			break;
		}
		break;


	}
}

void link::keyControl(void)
{
	// 키조작 (상하좌우)
	// 상
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (_linkState == BED_JUMP && !_bedJumpStart) _bedJumpStart = true;
		_pushUp = true;
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		if (_linkState == STAND)
		{
			_linkState = MOVE;
			_linkDir = UP;
			imageChange();
		}
		else if (_linkState == WEAPONSTAND)
		{
			_linkState = WEAPONMOVE;
			_linkDir = UP;
			imageChange();
		}
		else if (_linkState == FULLCHARGESTAND)
		{
			_linkState = FULLCHARGEMOVE;
			imageChange();
		}
		else if (_linkState == LIFTPOTSTAND)
		{
			_linkState = LIFTPOTMOVE;
			_linkDir = UP;
			imageChange();
		}
		else if (_linkState == LIFTGRASSSTAND)
		{
			_linkState = LIFTGRASSMOVE;
			_linkDir = UP;
			imageChange();
		}

		if (_linkState == MOVE || _linkState == LIFTPOTMOVE || _linkState == LIFTGRASSMOVE
			|| _linkState == WEAPONMOVE)
		{

			if (_cameraOnY) _centerY -= _speed;
			if (_linkDir == UP)	pixelCollision(UP);

			if (_nonLiftObject && (_linkState == MOVE || _linkState == WEAPONMOVE))
			{
				if (_pushCount % 100 == 0)
				{
					_linkState = PUSH;
					imageChange();
				}
			}
		}
		if (_linkState == NONLIFT) _nonLiftFrameStart = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_UP))
	{
		_pushUp = false;
		if (_linkState == MOVE && _linkDir == UP) _linkState = STAND;
		if (_linkState == WEAPONMOVE && _linkDir == UP) _linkState = WEAPONSTAND;
		if (_linkState == LIFTPOTMOVE && _linkDir == UP) _linkState = LIFTPOTSTAND;
		if (_linkState == LIFTGRASSMOVE && _linkDir == UP) _linkState = LIFTGRASSSTAND;
		if (_linkState == PUSH && _linkDir == UP)
		{
			if (!_weaponEquip) _linkState = STAND;
			else _linkState = WEAPONSTAND;
			_nonLiftObject = false;
			_pushCount = 0;
		}
		if (_nonLiftFrameStart) _nonLiftFrameStart = false;
		imageChange();
	}

	// 하
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (_linkState == BED_JUMP && !_bedJumpStart) _bedJumpStart = true;
		_pushDown = true;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{

		if (_linkState == STAND)
		{
			_linkState = MOVE;
			_linkDir = DOWN;
			imageChange();
		}
		if (_linkState == WEAPONSTAND)
		{
			_linkState = WEAPONMOVE;
			_linkDir = DOWN;
			imageChange();
		}
		if (_linkState == LIFTPOTSTAND)
		{
			_linkState = LIFTPOTMOVE;
			_linkDir = DOWN;
			imageChange();
		}
		else if (_linkState == LIFTGRASSSTAND)
		{
			_linkState = LIFTGRASSMOVE;
			_linkDir = DOWN;
			imageChange();
		}

		if (_linkState == MOVE || _linkState == LIFTPOTMOVE || _linkState == LIFTGRASSMOVE
			|| _linkState == WEAPONMOVE)
		{
			if (_cameraOnY) _centerY += _speed;

			if (_linkDir == DOWN) pixelCollision(DOWN);
			if (_nonLiftObject && (_linkState == MOVE || _linkState == WEAPONMOVE))
			{
				if (_pushCount % 100 == 0)
				{
					_linkState = PUSH;
					imageChange();
				}
			}
		}

		if (_linkState == NONLIFT) _nonLiftFrameStart = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		_pushDown = false;
		if (_linkState == MOVE && _linkDir == DOWN) _linkState = STAND;
		if (_linkState == WEAPONMOVE && _linkDir == DOWN) _linkState = WEAPONSTAND;
		if (_linkState == LIFTPOTMOVE && _linkDir == DOWN) _linkState = LIFTPOTSTAND;
		if (_linkState == LIFTGRASSMOVE && _linkDir == DOWN) _linkState = LIFTGRASSSTAND;
		if (_linkState == PUSH && _linkDir == DOWN)
		{
			if (!_weaponEquip) _linkState = STAND;
			else _linkState = WEAPONSTAND;
			_nonLiftObject = false;
			_pushCount = 0;
		}
		if (_nonLiftFrameStart) _nonLiftFrameStart = false;
		imageChange();
	}

	// 좌
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		if (_linkState == BED_JUMP && !_bedJumpStart) _bedJumpStart = true;
		_pushLeft = true;
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (_linkState == STAND)
		{
			_linkState = MOVE;
			_linkDir = LEFT;
			imageChange();
		}
		if (_linkState == WEAPONSTAND)
		{
			_linkState = WEAPONMOVE;
			_linkDir = LEFT;
			imageChange();
		}
		if (_linkState == LIFTPOTSTAND)
		{
			_linkState = LIFTPOTMOVE;
			_linkDir = LEFT;
			imageChange();
		}
		else if (_linkState == LIFTGRASSSTAND)
		{
			_linkState = LIFTGRASSMOVE;
			_linkDir = LEFT;
			imageChange();
		}

		if (_linkState == MOVE || _linkState == LIFTPOTMOVE || _linkState == LIFTGRASSMOVE
			|| _linkState == WEAPONMOVE)
		{
			if (_cameraOnX) _centerX -= _speed;
			if (_linkDir == LEFT) pixelCollision(LEFT);
			if (_nonLiftObject && (_linkState == MOVE || _linkState == WEAPONMOVE))
			{
				if (_pushCount % 100 == 0)
				{
					_linkState = PUSH;
					imageChange();
				}
			}
		}

		if (_linkState == NONLIFT) _nonLiftFrameStart = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
	{
		_pushLeft = false;
		if (_linkState == MOVE && _linkDir == LEFT) _linkState = STAND;
		if (_linkState == WEAPONMOVE && _linkDir == LEFT) _linkState = WEAPONSTAND;
		if (_linkState == LIFTPOTMOVE && _linkDir == LEFT) _linkState = LIFTPOTSTAND;
		if (_linkState == LIFTGRASSMOVE && _linkDir == LEFT) _linkState = LIFTGRASSSTAND;
		if (_linkState == PUSH && _linkDir == LEFT)
		{
			if (!_weaponEquip) _linkState = STAND;
			else _linkState = WEAPONSTAND;
			_nonLiftObject = false;
			_pushCount = 0;
		}
		if (_nonLiftFrameStart) _nonLiftFrameStart = false;
		imageChange();
	}

	// 우
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		if (_linkState == BED_JUMP && !_bedJumpStart) _bedJumpStart = true;
		_pushRight = true;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if (_linkState == STAND)
		{
			_linkState = MOVE;
			_linkDir = RIGHT;
			imageChange();
		}
		if (_linkState == WEAPONSTAND)
		{
			_linkState = WEAPONMOVE;
			_linkDir = RIGHT;
			imageChange();
		}
		if (_linkState == LIFTPOTSTAND)
		{
			_linkState = LIFTPOTMOVE;
			_linkDir = RIGHT;
			imageChange();
		}
		else if (_linkState == LIFTGRASSSTAND)
		{
			_linkState = LIFTGRASSMOVE;
			_linkDir = RIGHT;
			imageChange();
		}

		if (_linkState == MOVE || _linkState == LIFTPOTMOVE || _linkState == LIFTGRASSMOVE
			|| _linkState == WEAPONMOVE)
		{
			if (_cameraOnX) _centerX += _speed;
			if (_linkDir == RIGHT) pixelCollision(RIGHT);
			if (_nonLiftObject && (_linkState == MOVE || _linkState == WEAPONMOVE))
			{
				if (_pushCount % 100 == 0)
				{
					_linkState = PUSH;
					imageChange();
				}
			}
		}

		if (_linkState == NONLIFT) _nonLiftFrameStart = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		_pushRight = false;
		if (_linkState == MOVE && _linkDir == RIGHT) _linkState = STAND;
		if (_linkState == WEAPONMOVE && _linkDir == RIGHT) _linkState = WEAPONSTAND;
		if (_linkState == LIFTPOTMOVE && _linkDir == RIGHT) _linkState = LIFTPOTSTAND;
		if (_linkState == LIFTGRASSMOVE && _linkDir == RIGHT) _linkState = LIFTGRASSSTAND;
		if (_linkState == PUSH && _linkDir == RIGHT)
		{
			if (!_weaponEquip) _linkState = STAND;
			else _linkState = WEAPONSTAND;
			_nonLiftObject = false;
			_pushCount = 0;
		}
		if (_nonLiftFrameStart) _nonLiftFrameStart = false;
		imageChange();
	}

	// 키조작 (A, S, Z, X)
	if (KEYMANAGER->isOnceKeyDown('A'))		// 마법 및 부메랑 사용
	{
		if (_linkState == BED_JUMP && !_bedJumpStart) _bedJumpStart = true;

		//if (_itemState == ITEM_LAMP)
	}

	// 공격 관련 움직임
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		if (_linkState == BED_JUMP && !_bedJumpStart) _bedJumpStart = true;

		if (_linkState == WEAPONSTAND || _linkState == WEAPONMOVE || _linkState == ATTACK)
		{
			_linkState = ATTACK;
			imageChange();
		}
	}
	if (KEYMANAGER->isStayKeyDown('Z'))
	{
		if (_linkState == ATTACK) _chargeCount++;
		if (_chargeCount % 10 == 0 && _linkState == ATTACK)
		{
			_linkState = CHARGE;
			_fullChargeDir = _linkDir;
			_chargeCount = 0;
			imageChange();
		}

	}

	if (KEYMANAGER->isOnceKeyUp('Z'))
	{
		_chargeCount = 0;
		if (_linkState == CHARGE)
		{
			_linkState = WEAPONSTAND;
			imageChange();
		}
		if (_linkState == FULLCHARGESTAND || _linkState == FULLCHARGEMOVE)
		{
			_linkState = CHARGEATTACK;
			imageChange();
		}

	}


	// 오브젝트 들고 던지기 및 아이템 획득
	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		if (_linkState == BED_JUMP && !_bedJumpStart) _bedJumpStart = true;
		switch (_linkDir)
		{
		case UP:
			_objectRc = RectMake(_shadowX, _bodyRc.top + 30, 10, 10);
			if (_liftObject)
			{
				// 항아리 든 상태에서 쏘라고 명령 할 때
				if (_linkState == LIFTPOTSTAND || _linkState == LIFTPOTMOVE)
				{
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						if (!_linkWorld->getVJar()[i]->getIsLift()) continue;
						_linkWorld->getVJar()[i]->isFire(_shadowX, _bodyRc.top);
						_fireDirection = FIRE_UP;
						_liftObject = false;
						if (!_weaponEquip) _linkState = STAND;
						else _linkState = WEAPONSTAND;
						imageChange();
						break;
					}
				} // 항아리 쏠 때
				else if (_linkState == LIFTGRASSSTAND || _linkState == LIFTGRASSMOVE)
				{
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (!_linkWorld->getVGrass()[i]->getIsLift()) continue;
						_linkWorld->getVGrass()[i]->isFire(_shadowX, _bodyRc.top);
						_fireDirection = FIRE_UP;
						_liftObject = false;
						if (!_weaponEquip) _linkState = STAND;
						else _linkState = WEAPONSTAND;
						imageChange();
						break;
					}
				}
			}
			else
			{
				// 항아리 들 때
				if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
				{
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_objectRc, &(*_linkWorld->getVJar()[i]->getThrowObjectRect())))
						{
							if (_linkWorld->getVJar()[i]->getIsLift()) continue;
							_linkWorld->getVJar()[i]->setIsLift(true);
							_linkWorld->getVJar()[i]->objectLifting(_shadowX, _bodyRc.top);
							if (!_liftObject) _liftObject = true;
							_linkState = LIFTPOT;
							imageChange();
							_objectRc = RectMake(0, 0, 0, 0);
							break;
						}
					}
				}
				else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
				{
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_objectRc, &(*_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							if (_linkWorld->getVGrass()[i]->getIsLift()) continue;
							_linkWorld->getVGrass()[i]->setIsLift(true);
							_linkWorld->getVGrass()[i]->objectLifting(_shadowX, _bodyRc.top);
							if (!_liftObject) _liftObject = true;
							_linkState = LIFTGRASS;
							imageChange();
							_objectRc = RectMake(0, 0, 0, 0);
							break;
						}
					}
				}

			}

			for (int i = _shadowY - 5; i < _shadowY; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _shadowX, i + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))		// 벽 및 들 수없는 개체
				{
					_nonLiftObject = true;
					if (_linkState == STAND || _linkState == MOVE || _linkState == WEAPONSTAND ||
						_linkState == WEAPONMOVE) _linkState = NONLIFT;
					imageChange();
					break;
				}
				if (color == CHESTBOX)
				{
					_itemOpen = true;
					break;
				}
			}

			break;
		case DOWN:
			_objectRc = RectMake(_shadowX, _bodyRc.bottom + 30, 10, 10);
			if (_liftObject)
			{
				// 항아리 든 상태에서 쏘라고 명령 할 때
				if (_linkState == LIFTPOTSTAND || _linkState == LIFTPOTMOVE)
				{
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						if (!_linkWorld->getVJar()[i]->getIsLift()) continue;
						_linkWorld->getVJar()[i]->isFire(_shadowX, _bodyRc.top);
						_fireDirection = FIRE_DOWN;
						_liftObject = false;
						if (!_weaponEquip) _linkState = STAND;
						else _linkState = WEAPONSTAND;
						imageChange();
						break;
					}
				}
				else if (_linkState == LIFTGRASSSTAND || _linkState == LIFTGRASSMOVE)
				{
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (!_linkWorld->getVGrass()[i]->getIsLift()) continue;
						_linkWorld->getVGrass()[i]->isFire(_shadowX, _bodyRc.top);
						_fireDirection = FIRE_DOWN;
						_liftObject = false;
						if (!_weaponEquip) _linkState = STAND;
						else _linkState = WEAPONSTAND;
						imageChange();
						break;
					}
				}
			}
			else
			{
				if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
				{
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_objectRc, &(*_linkWorld->getVJar()[i]->getThrowObjectRect())))
						{
							if (_linkWorld->getVJar()[i]->getIsLift()) continue;
							_linkWorld->getVJar()[i]->setIsLift(true);
							_linkWorld->getVJar()[i]->objectLifting(_shadowX, _bodyRc.top);
							if (!_liftObject) _liftObject = true;
							_linkState = LIFTPOT;
							imageChange();
							_objectRc = RectMake(0, 0, 0, 0);
							break;
						}
					}
				}
				else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
				{
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_objectRc, &(*_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							if (_linkWorld->getVGrass()[i]->getIsLift()) continue;
							_linkWorld->getVGrass()[i]->setIsLift(true);
							_linkWorld->getVGrass()[i]->objectLifting(_shadowX, _bodyRc.top);
							if (!_liftObject) _liftObject = true;
							_linkState = LIFTGRASS;
							imageChange();
							_objectRc = RectMake(0, 0, 0, 0);
							break;
						}
					}
				}
			}

			for (int i = _bodyRc.bottom; i < _bodyRc.bottom + 5; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _shadowX, i + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 248 && g == 146 && b == 0))		// 보물상자
				{
					_nonLiftObject = true;
					_linkState = NONLIFT;
					imageChange();
					break;
				}
				//if ((r == 252 && g == 123 && b == 123))		// 풀 & 항아리
				//{
				//	if (!_liftObject) _liftObject = true;
				//	if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA) _linkState = LIFTPOT;
				//	imageChange();
				//	break;
				//}
				if ((r == 145 && g == 117 && b == 234))		// 벽 및 들 수없는 개체
				{
					_nonLiftObject = true;
					_linkState = NONLIFT;
					imageChange();
					break;
				}
			}
			break;
		case LEFT:
			_objectRc = RectMake(_bodyRc.left - 30, _shadowY, 10, 10);
			if (_liftObject)
			{
				// 항아리 든 상태에서 쏘라고 명령 할 때
				if (_linkState == LIFTPOTSTAND || _linkState == LIFTPOTMOVE)
				{
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						if (!_linkWorld->getVJar()[i]->getIsLift()) continue;
						_linkWorld->getVJar()[i]->isFire(_shadowX, _bodyRc.top);
						_fireDirection = FIRE_LEFT;
						_liftObject = false;
						if (!_weaponEquip) _linkState = STAND;
						else _linkState = WEAPONSTAND;
						imageChange();
						break;
					}
				}
				else if (_linkState == LIFTGRASSSTAND || _linkState == LIFTGRASSMOVE)
				{
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (!_linkWorld->getVGrass()[i]->getIsLift()) continue;
						_linkWorld->getVGrass()[i]->isFire(_shadowX, _bodyRc.top);
						_fireDirection = FIRE_LEFT;
						_liftObject = false;
						if (!_weaponEquip) _linkState = STAND;
						else _linkState = WEAPONSTAND;
						imageChange();
						break;
					}
				}
			}
			else
			{
				// 항아리 들 때
				if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
				{
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_objectRc, &(*_linkWorld->getVJar()[i]->getThrowObjectRect())))
						{
							if (_linkWorld->getVJar()[i]->getIsLift()) continue;
							_linkWorld->getVJar()[i]->setIsLift(true);
							_linkWorld->getVJar()[i]->objectLifting(_shadowX, _bodyRc.top);
							if (!_liftObject) _liftObject = true;
							_linkState = LIFTPOT;
							imageChange();
							_objectRc = RectMake(0, 0, 0, 0);
							break;
						}
					}
				}
				else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
				{
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_objectRc, &(*_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							if (_linkWorld->getVGrass()[i]->getIsLift()) continue;
							_linkWorld->getVGrass()[i]->setIsLift(true);
							_linkWorld->getVGrass()[i]->objectLifting(_shadowX, _bodyRc.top);
							if (!_liftObject) _liftObject = true;
							_linkState = LIFTGRASS;
							imageChange();
							_objectRc = RectMake(0, 0, 0, 0);
							break;
						}
					}
				}
			}

			for (int i = _shadowY; i < _bodyRc.bottom; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _bodyRc.left, i + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 248 && g == 146 && b == 0))		// 보물상자
				{
					_nonLiftObject = true;
					_linkState = NONLIFT;
					imageChange();
					break;
				}
				//if ((r == 252 && g == 123 && b == 123))		// 풀 & 항아리
				//{
				//	if (!_liftObject) _liftObject = true;
				//	if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA) _linkState = LIFTPOT;
				//	imageChange();
				//	break;
				//}
				if ((r == 145 && g == 117 && b == 234))		// 벽 및 들 수없는 개체
				{
					_nonLiftObject = true;
					_linkState = NONLIFT;
					imageChange();
					break;
				}
			}
			break;
		case RIGHT:
			_objectRc = RectMake(_bodyRc.right + 30, _shadowY, 10, 10);
			if (_liftObject)
			{
				// 항아리 든 상태에서 쏘라고 명령 할 때
				if (_linkState == LIFTPOTSTAND || _linkState == LIFTPOTMOVE)
				{
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						if (!_linkWorld->getVJar()[i]->getIsLift()) continue;
						_linkWorld->getVJar()[i]->isFire(_shadowX, _bodyRc.top);
						_fireDirection = FIRE_RIGHT;
						_liftObject = false;
						if (!_weaponEquip) _linkState = STAND;
						else _linkState = WEAPONSTAND;
						imageChange();
						break;
					}
				}
				else if (_linkState == LIFTGRASSSTAND || _linkState == LIFTGRASSMOVE)
				{
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (!_linkWorld->getVGrass()[i]->getIsLift()) continue;
						_linkWorld->getVGrass()[i]->isFire(_shadowX, _bodyRc.top);
						_fireDirection = FIRE_RIGHT;
						_liftObject = false;
						if (!_weaponEquip) _linkState = STAND;
						else _linkState = WEAPONSTAND;
						imageChange();
						break;
					}
				}
			}
			else
			{
				// 항아리 들 때
				if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
				{
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_objectRc, &(*_linkWorld->getVJar()[i]->getThrowObjectRect())))
						{
							if (_linkWorld->getVJar()[i]->getIsLift()) continue;
							_linkWorld->getVJar()[i]->setIsLift(true);
							_linkWorld->getVJar()[i]->objectLifting(_shadowX, _bodyRc.top);
							if (!_liftObject) _liftObject = true;
							_linkState = LIFTPOT;
							imageChange();
							_objectRc = RectMake(0, 0, 0, 0);
							break;
						}
					}
				}
				else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
				{
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						RECT temp;
						if (IntersectRect(&temp, &_objectRc, &(*_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							if (_linkWorld->getVGrass()[i]->getIsLift()) continue;
							_linkWorld->getVGrass()[i]->setIsLift(true);
							_linkWorld->getVGrass()[i]->objectLifting(_shadowX, _bodyRc.top);
							if (!_liftObject) _liftObject = true;
							_linkState = LIFTGRASS;
							imageChange();
							_objectRc = RectMake(0, 0, 0, 0);
							break;
						}
					}
				}
			}
			for (int i = _shadowY; i < _bodyRc.bottom; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _bodyRc.right, i + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 248 && g == 146 && b == 0))		// 보물상자
				{
					_nonLiftObject = true;
					_linkState = NONLIFT;
					imageChange();
					break;
				}
				//if ((r == 252 && g == 123 && b == 123))		// 풀 & 항아리
				//{
				//	if (!_liftObject) _liftObject = true;
				//	if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA) _linkState = LIFTPOT;
				//	imageChange();
				//	break;
				//}
				if ((r == 145 && g == 117 && b == 234))		// 벽 및 들 수없는 개체
				{
					_nonLiftObject = true;
					_linkState = NONLIFT;
					imageChange();
					break;
				}
			}
			break;

		}
	}
	if (KEYMANAGER->isStayKeyDown('X') && _linkState == NONLIFT)
	{
		// 물체 잡고 있는 모션 유지
		if (!_nonLiftFrameStart)
		{
			switch (_linkDir)
			{
			case UP:
				_image->setFrameY(1);
				_image->setFrameX(0);
				break;
			case DOWN:
				_image->setFrameY(0);
				_image->setFrameX(0);
				break;
			case LEFT:
				_image->setFrameY(1);
				_image->setFrameX(3);
				break;
			case RIGHT:
				_image->setFrameY(0);
				_image->setFrameX(0);
				break;
			}
		}
	}
	if (KEYMANAGER->isOnceKeyUp('X'))
	{
		if (_linkState == NONLIFT)
		{
			_nonLiftFrameStart = false;
			if (!_weaponEquip) _linkState = STAND;
			else _linkState = WEAPONSTAND;
			imageChange();
		}
		_objectRc = RectMake(0, 0, 0, 0);
	}

	//if (KEYMANAGER->isOnceKeyDown(VK_RETURN)) // 컨티뉴 게임, 세이브 및 종료
	//{
	//	if (_linkState == SLEEP && _image->getFrameX() == 0) _linkState = BED_JUMP;
	//}

	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		_linkState = WEAPONSTAND;
		_weaponEquip = true;
		_swordRc = RectMake(0, 0, 0, 0);
		imageChange();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		_itemState = ITEM_BOOMERANG;
	}

}

void link::imageChange(void)
{
	switch (_linkState)
	{
	case STAND:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkStand");
			_image->setFrameY(0);
			_image->setFrameX(1);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkStand");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkStand");
			_image->setFrameY(0);
			_image->setFrameX(3);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkStand");
			_image->setFrameY(0);
			_image->setFrameX(2);
			break;
		}
		break;
	case MOVE:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkMoveUpandDown");
			_image->setFrameY(1);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkMoveUpandDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkMoveLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(6);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkMoveLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case PUSH:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkPushUp");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkPushDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkPushLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(3);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkPushLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case NONLIFT:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkNonLiftUpandDown");
			_image->setFrameY(1);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkNonLiftUpandDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkNonLiftLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(3);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkNonLiftLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case LIFTPOT:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("potLiftUp");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("potLiftDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("potLiftLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("potLiftLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(5);
			break;
		}
		break;
	case LIFTPOTSTAND:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("potLiftStand");
			_image->setFrameY(0);
			_image->setFrameX(1);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("potLiftStand");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("potLiftStand");
			_image->setFrameY(0);
			_image->setFrameX(3);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("potLiftStand");
			_image->setFrameY(0);
			_image->setFrameX(2);
			break;
		}
		break;
	case LIFTPOTMOVE:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("potLiftMoveUp");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("potLiftMoveDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("potLiftMoveLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(3);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("potLiftMoveLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case WEAPONSTAND:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkStand");
			_image->setFrameY(1);
			_image->setFrameX(1);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkStand");
			_image->setFrameY(1);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkStand");
			_image->setFrameY(1);
			_image->setFrameX(3);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkStand");
			_image->setFrameY(1);
			_image->setFrameX(2);
			break;
		}
		break;
	case WEAPONMOVE:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkWeaponMoveUp");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkWeaponMoveDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkWeaponMoveLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(6);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkWeaponMoveLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case ATTACK:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkAttackUp");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkAttackDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkAttackLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(8);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkAttackLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case CHARGE:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkStandUpCharge");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkStandDownCharge");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkStandLeftandRightCharge");
			_image->setFrameY(1);
			_image->setFrameX(_image->getMaxFrameX() - 1);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkStandLeftandRightCharge");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case CHARGEATTACK:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("linkChargeAttackUpandDown");
			_image->setFrameY(1);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("linkChargeAttackUpandDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("linkChargeAttackLeft");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("linkChargeAttackRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case FULLCHARGESTAND:
		switch (_fullChargeDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("fullChargeStand");
			_fullChargeTwinkle = IMAGEMANAGER->findImage("swordTipTwinkelUpandDown");
			_fullChargeTwinkle->setFrameY(0);
			_fullChargeTwinkle->setFrameX(0);
			_image->setFrameY(0);
			_image->setFrameX(1);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("fullChargeStand");
			_fullChargeTwinkle = IMAGEMANAGER->findImage("swordTipTwinkelUpandDown");
			_fullChargeTwinkle->setFrameY(0);
			_fullChargeTwinkle->setFrameX(0);
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("fullChargeStand");
			_fullChargeTwinkle = IMAGEMANAGER->findImage("swordTipTwinkleLeftandRight");
			_fullChargeTwinkle->setFrameY(0);
			_fullChargeTwinkle->setFrameX(0);
			_image->setFrameY(0);
			_image->setFrameX(3);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("fullChargeStand");
			_fullChargeTwinkle = IMAGEMANAGER->findImage("swordTipTwinkleLeftandRight");
			_fullChargeTwinkle->setFrameY(0);
			_fullChargeTwinkle->setFrameX(0);
			_image->setFrameY(0);
			_image->setFrameX(2);
			break;
		}
		break;
	case LIFTGRASS:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("grassLiftUp");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("grassLiftDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("grassLiftLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(5);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("grassLiftLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		}
		break;
	case LIFTGRASSSTAND:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("liftGrassStand");
			_image->setFrameY(0);
			_image->setFrameX(1);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("liftGrassStand");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("liftGrassStand");
			_image->setFrameY(0);
			_image->setFrameX(3);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("liftGrassStand");
			_image->setFrameY(0);
			_image->setFrameX(2);
			break;
		}
		break;
	case LIFTGRASSMOVE:
		switch (_linkDir)
		{
		case UP:
			_image = IMAGEMANAGER->findImage("grassLiftMoveUp");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case DOWN:
			_image = IMAGEMANAGER->findImage("grassLiftMoveDown");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case LEFT:
			_image = IMAGEMANAGER->findImage("grassLiftMoveLeftandRight");
			_image->setFrameY(0);
			_image->setFrameX(0);
			break;
		case RIGHT:
			_image = IMAGEMANAGER->findImage("grassLiftMoveLeftandRight");
			_image->setFrameY(1);
			_image->setFrameX(3);
			break;
		}
		break;


	}
}

void link::hitDamage(int damage)
{
	_currentHp -= damage;
}

void link::pixelCollision(LINKDIRECTION dir)
{
	switch (dir)
	{
	case UP:
		// 집안 기본 픽셀층
		for (int i = _bodyRc.left + 10; i < _bodyRc.right - 10; ++i)
		{
			// 위쪽 체크
			if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), i, (int)_shadowY + 5 + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 248 && g == 146 && b == 0))		// 보물상자
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 252 && g == 123 && b == 123))
				{
					_objectRc = RectMake(_shadowX, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVJar()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerY = _shadowY;
							_nonLiftObject = true;
							break;
						}
					}

					if (_pixelCollision) break;

				}
				else
				{
					_pixelCollision = false;
					_nonLiftObject = false;
					_pushCount = 0;
				}
			}
			else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), (int)_shadowY + 5 + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 252 && g == 123 && b == 123))
				{
					_objectRc = RectMake(_shadowX, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 풀 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerY = _shadowY;
							_nonLiftObject = true;
							break;
						}
					}

					if (_pixelCollision) break;

				}
				else
				{
					_pixelCollision = false;
					_nonLiftObject = false;
					_pushCount = 0;
				}
			}

		}

		for (int i = _shadowY + 10; i < _bodyRc.bottom; ++i)
		{
			if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
			{
				// 왼쪽 체크
				COLORREF color1 = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _bodyRc.left + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());
				// 오른쪽 체크
				COLORREF color2 = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _bodyRc.right + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());

				int r1 = GetRValue(color1);
				int g1 = GetGValue(color1);
				int b1 = GetBValue(color1);

				int r2 = GetRValue(color2);
				int g2 = GetGValue(color2);
				int b2 = GetBValue(color2);

				if ((r1 == 145 && g1 == 117 && b1 == 234) && _pushLeft)
				{
					_pixelCollision = true;
					_centerX = _shadowX + _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 248 && g1 == 146 && b1 == 0) && _pushLeft)	// 보물상자
				{
					_pixelCollision = true;
					_centerX = _shadowX + _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 252 && g1 == 123 && b1 == 123) && _pushLeft)
				{
					_objectRc = RectMake(_bodyRc.left - 30, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVJar()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _shadowX + _speed;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;

				}
				else _pixelCollision = false;

				if ((r2 == 145 && g2 == 117 && b2 == 234) && _pushRight)
				{
					_pixelCollision = true;
					_centerX = _shadowX - _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 248 && g2 == 146 && b2 == 0))	// 보물상자
				{
					_pixelCollision = true;
					_centerX = _shadowX - _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 252 && g2 == 123 && b2 == 123) && _pushRight)
				{
					_objectRc = RectMake(_bodyRc.right + 30, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVJar()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _shadowX - _speed;
							_nonLiftObject = true;
							break;
						}
					}

					if (_pixelCollision) break;

				}
				else _pixelCollision = false;

			}
			else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
			{
				// 왼쪽 체크
				COLORREF color1 = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), _bodyRc.left + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());
				// 오른쪽 체크
				COLORREF color2 = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), _bodyRc.right + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());

				int r1 = GetRValue(color1);
				int g1 = GetGValue(color1);
				int b1 = GetBValue(color1);

				int r2 = GetRValue(color2);
				int g2 = GetGValue(color2);
				int b2 = GetBValue(color2);

				if ((r1 == 145 && g1 == 117 && b1 == 234))
				{
					_pixelCollision = true;
					_centerX = _shadowX + _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 252 && g1 == 123 && b1 == 123) && _pushLeft)
				{
					_objectRc = RectMake(_bodyRc.left - 30, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _shadowX + _speed;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;
				}
				else _pixelCollision = false;

				if ((r2 == 145 && g2 == 117 && b2 == 234))
				{
					_pixelCollision = true;
					_centerX = _shadowX - _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 252 && g2 == 123 && b2 == 123) && _pushRight)
				{
					_objectRc = RectMake(_bodyRc.right + 30, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _shadowX - _speed;
							_nonLiftObject = true;
							break;
						}
					}

					if (_pixelCollision) break;

				}
				else _pixelCollision = false;
			}

		}
		break;
	case DOWN:
		// 집안 기본 픽셀 층
		for (int i = _bodyRc.left + 10; i < _bodyRc.right - 10; ++i)
		{
			if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
			{
				// 아래쪽 체크
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), _bodyRc.bottom + 5 + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 248 && g == 146 && b == 0))		// 보물상자
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					_nonLiftObject = true;
					break;
				}
				else
				{
					_pixelCollision = false;
					_nonLiftObject = false;
					_pushCount = 0;
				}
			}
			else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
			{
				// 아래쪽 체크
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), _bodyRc.bottom + 5 + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 252 && g == 123 && b == 123))
				{
					_objectRc = RectMake(_shadowX, _bodyRc.bottom + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerY = _shadowY;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;

				}
				else
				{
					_pixelCollision = false;
					_nonLiftObject = false;
					_pushCount = 0;
				}
			}

		}

		for (int i = _shadowY + 10; i < _bodyRc.bottom - 10; ++i)
		{
			if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
			{
				// 왼쪽 체크
				COLORREF color1 = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _bodyRc.left + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());
				// 오른쪽 체크
				COLORREF color2 = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _bodyRc.right + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());

				int r1 = GetRValue(color1);
				int g1 = GetGValue(color1);
				int b1 = GetBValue(color1);

				int r2 = GetRValue(color2);
				int g2 = GetGValue(color2);
				int b2 = GetBValue(color2);

				if ((r1 == 145 && g1 == 117 && b1 == 234))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.left + _bodyRcWidth / 2 + _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 248 && g1 == 146 && b1 == 0))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.left + _bodyRcWidth / 2 + _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 145 && g2 == 117 && b2 == 234))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.right - _bodyRcWidth / 2 - _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 248 && g2 == 146 && b2 == 0))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.right - _bodyRcWidth / 2 - _speed;
					break;
				}
				else _pixelCollision = false;
			}
			else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
			{
				// 왼쪽 체크
				COLORREF color1 = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), _bodyRc.left + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());
				// 오른쪽 체크
				COLORREF color2 = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), _bodyRc.right + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());

				int r1 = GetRValue(color1);
				int g1 = GetGValue(color1);
				int b1 = GetBValue(color1);

				int r2 = GetRValue(color2);
				int g2 = GetGValue(color2);
				int b2 = GetBValue(color2);

				if ((r1 == 145 && g1 == 117 && b1 == 234))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.left + _bodyRcWidth / 2 + _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 252 && g1 == 123 && b1 == 123) && _pushLeft)
				{
					_objectRc = RectMake(_bodyRc.left - 30, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _shadowX + _speed;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;
				}
				else _pixelCollision = false;

				if ((r2 == 145 && g2 == 117 && b2 == 234))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.right - _bodyRcWidth / 2 - _speed;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 252 && g2 == 123 && b2 == 123) && _pushRight)
				{
					_objectRc = RectMake(_bodyRc.right + 30, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _shadowX - _speed;
							_nonLiftObject = true;
							break;
						}
					}

					if (_pixelCollision) break;

				}
				else _pixelCollision = false;
			}

		}
		break;
	case LEFT:
		// 집안 기본 픽셀 층
		for (int i = _shadowY + 10; i < _bodyRc.bottom; ++i)
		{
			if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _bodyRc.left + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))
				{
					_centerX = _bodyRc.left + _bodyRcWidth / 2;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 248 && g == 146 && b == 0))		// 보물 상자
				{
					_centerX = _bodyRc.left + _bodyRcWidth / 2;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 252 && g == 123 && b == 123))
				{
					_objectRc = RectMake(_bodyRc.left - 30, _shadowY, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVJar().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVJar()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _bodyRc.left + _bodyRcWidth / 2;
							_nonLiftObject = true;
							break;
						}
					}

					if (_pixelCollision) break;

				}
				else
				{
					_pixelCollision = false;
					_nonLiftObject = false;
					_pushCount = 0;
				}
			}
			else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), _bodyRc.left + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.left + _bodyRcWidth / 2;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 252 && g == 123 && b == 123))
				{
					_objectRc = RectMake(_bodyRc.left - 30, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _shadowX + _speed;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;
				}
				else
				{
					_pixelCollision = false;
					_nonLiftObject = false;
					_pushCount = 0;
				}
			}
		}

		for (int i = _bodyRc.left + 10; i < _bodyRc.right - 10; ++i)
		{
			if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
			{
				// 위쪽 체크
				COLORREF color1 = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), (int)_shadowY + 5 + _linkWorld->getMapPixelLoopY());
				// 아래쪽 체크
				COLORREF color2 = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), _bodyRc.bottom + 5 + _linkWorld->getMapPixelLoopY());

				int r1 = GetRValue(color1);
				int g1 = GetGValue(color1);
				int b1 = GetBValue(color1);

				int r2 = GetRValue(color2);
				int g2 = GetGValue(color2);
				int b2 = GetBValue(color2);

				if ((r1 == 145 && g1 == 117 && b1 == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 248 && g1 == 146 && b1 == 0))	// 보물상자
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 145 && g2 == 117 && b2 == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 248 && g2 == 146 && b2 == 0))	// 보물상자
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;
			}
			else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
			{
				// 위쪽 체크
				COLORREF color1 = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), (int)_shadowY + 5 + _linkWorld->getMapPixelLoopY());
				// 아래쪽 체크
				COLORREF color2 = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), _bodyRc.bottom + 5 + _linkWorld->getMapPixelLoopY());

				int r1 = GetRValue(color1);
				int g1 = GetGValue(color1);
				int b1 = GetBValue(color1);

				int r2 = GetRValue(color2);
				int g2 = GetGValue(color2);
				int b2 = GetBValue(color2);

				if ((r1 == 145 && g1 == 117 && b1 == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 252 && g1 == 123 && b1 == 123) && _pushUp)
				{
					_objectRc = RectMake(_shadowX, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 풀 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerY = _shadowY;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;

				}
				else _pixelCollision = false;

				if ((r2 == 145 && g2 == 117 && b2 == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 252 && g2 == 123 && b2 == 123) && _pushDown)
				{
					_objectRc = RectMake(_shadowX, _bodyRc.bottom + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerY = _shadowY;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;

				}
				else _pixelCollision = false;
			}

		}
		break;
	case RIGHT:
		// 집안 기본 픽셀 층
		for (int i = _shadowY + 10; i < _bodyRc.bottom; ++i)
		{
			if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _bodyRc.right + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.right - _bodyRcWidth / 2;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 248 && g == 146 && b == 0))		// 보물 상자
				{
					_pixelCollision = true;
					_centerX = _bodyRc.right - _bodyRcWidth / 2;
					_nonLiftObject = true;
					break;
				}
				else
				{
					_pixelCollision = false;
					_nonLiftObject = false;
					_pushCount = 0;
				}
			}
			else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), _bodyRc.right + _linkWorld->getMapPixelLoopX(), i + _linkWorld->getMapPixelLoopY());

				int r = GetRValue(color);
				int g = GetGValue(color);
				int b = GetBValue(color);

				if ((r == 145 && g == 117 && b == 234))
				{
					_pixelCollision = true;
					_centerX = _bodyRc.right - _bodyRcWidth / 2;
					_nonLiftObject = true;
					break;
				}
				else if ((r == 252 && g == 123 && b == 123))
				{
					_objectRc = RectMake(_bodyRc.right + 30, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerX = _bodyRc.right - _bodyRcWidth / 2;
							_nonLiftObject = true;
							break;
						}
					}

					if (_pixelCollision) break;

				}
				else
				{
					_pixelCollision = false;
					_nonLiftObject = false;
					_pushCount = 0;
				}
			}

		}

		for (int i = _bodyRc.left + 10; i < _bodyRc.right - 10; ++i)
		{
			if (_linkWorld->getWhereIsLinnk() == INSIDE_HOUSE_AREA)
			{
				// 위쪽 체크
				COLORREF color1 = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), (int)_shadowY + 5 + _linkWorld->getMapPixelLoopY());
				// 아래쪽 체크
				COLORREF color2 = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), _bodyRc.bottom + 5 + _linkWorld->getMapPixelLoopY());

				int r1 = GetRValue(color1);
				int g1 = GetGValue(color1);
				int b1 = GetBValue(color1);

				int r2 = GetRValue(color2);
				int g2 = GetGValue(color2);
				int b2 = GetBValue(color2);

				if ((r1 == 145 && g1 == 117 && b1 == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 248 && g1 == 146 && b1 == 0))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 145 && g2 == 117 && b2 == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 248 && g2 == 146 && b2 == 0))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;
			}
			else if (_linkWorld->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
			{
				// 위쪽 체크
				COLORREF color1 = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), (int)_shadowY + 5 + _linkWorld->getMapPixelLoopY());
				// 아래쪽 체크
				COLORREF color2 = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), i + _linkWorld->getMapPixelLoopX(), _bodyRc.bottom + 5 + _linkWorld->getMapPixelLoopY());

				int r1 = GetRValue(color1);
				int g1 = GetGValue(color1);
				int b1 = GetBValue(color1);

				int r2 = GetRValue(color2);
				int g2 = GetGValue(color2);
				int b2 = GetBValue(color2);

				if ((r1 == 145 && g1 == 117 && b1 == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r1 == 252 && g1 == 123 && b1 == 123) && _pushUp)
				{
					_objectRc = RectMake(_shadowX, _bodyRc.top + 30, 10, 10);
					RECT temp;
					// 풀 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerY = _shadowY;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;

				}
				else _pixelCollision = false;

				if ((r2 == 145 && g2 == 117 && b2 == 234))
				{
					_pixelCollision = true;
					_centerY = _shadowY;
					break;
				}
				else _pixelCollision = false;

				if ((r2 == 252 && g2 == 123 && b2 == 123) && _pushDown)
				{
					_objectRc = RectMake(_shadowX, _bodyRc.bottom + 30, 10, 10);
					RECT temp;
					// 항아리 있을 때 못가게 막아주는 코드
					for (int i = 0; i < _linkWorld->getVGrass().size(); ++i)
					{
						if (IntersectRect(&temp, &_objectRc, &*(_linkWorld->getVGrass()[i]->getThrowObjectRect())))
						{
							_pixelCollision = true;
							_centerY = _shadowY;
							_nonLiftObject = true;
							break;
						}
					}
					if (_pixelCollision) break;

				}
				else _pixelCollision = false;
			}

		}

		break;
	}
	_objectRc = RectMake(0, 0, 0, 0);
}