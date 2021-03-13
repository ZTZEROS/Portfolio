#include "stdafx.h"
#include "statusOverviewPanel.h"


statusOverviewPanel::statusOverviewPanel()
	:_lvldigit(0), _isUpdated(0)
{
}


statusOverviewPanel::~statusOverviewPanel()
{
}
HRESULT statusOverviewPanel::init(int x, int y)
{
	//패널 이미지 등록해주고~
	_statusPanelImage = IMAGEMANAGER->addImage(
		"전투상태",
		"UI_IMAGE/battleUI.bmp",
		606,
		161,
		true,
		MAGENTA);
	//패널이미지 위치설정~
	_statusPanelImage->setX(x);
	_statusPanelImage->setY(y);
	//패널안에서 사용할 프로그레바 등록해주고~
	_progressBar[0].overviewPanelLevelInit("현재경험치", "최대경험치", 0, 0);
	_progressBar[1].overviewPanelHPInit("현재체력", "최대체력", 0, 0);
	_progressBar[2].overviewPanelSPInit("현재마력", "최대마력", 0, 0);

	return S_OK;
}
void statusOverviewPanel::release(void)
{

}
void statusOverviewPanel::update(float currentExp, float maxExp,
	float currentHp, float maxHp,
	float currentSp, float maxSp,
	int x, int y)
{
	//레벨이미지벡터 날려주고~
	_vLvlNumberImage.clear();
	_vHpNumberImage.clear();
	_vSpNumberImage.clear();
	//레벨 자릿수 초기화~
	_lvldigit = 0;
	_hpDigit = 0;
	_spDigit = 0;
	//경험치 업데이트
	expUpdate(currentExp, maxExp);
	//체력 업데이트~
	hpUpdate(currentHp, maxHp);
	//sp 업데이트~
	spUpdate(currentSp, maxSp);
	//패널 위치 업데이트~
	_statusPanelImage->setX(x);
	_statusPanelImage->setY(y);
}
void statusOverviewPanel::expUpdate(float currentGauge, float maxGauge)
{
	_progressBar[0].update(
		_statusPanelImage->getX() + 335,
		_statusPanelImage->getY() + 77,
		currentGauge,
		maxGauge);
	int tempNum = currentGauge;
	//자리수 계산하기
	while (tempNum != 0)
	{
		tempNum = tempNum / 10;
		_lvldigit++;
	}

	//자릿수만큼 이미지 벡터에 넣기
	int lvlNumber;
	for (int i = 0; i < _lvldigit; i++)
	{
		int tempNumber2;
		tempNumber2 = pow(10, _lvldigit - (i + 1));
		lvlNumber = ((int)currentGauge / (tempNumber2)) % 10;
		sprintf(_lvlStr, "%d", lvlNumber);
		image* tempImage;
		tempImage = new image;
		tempImage = IMAGEMANAGER->findImage(_lvlStr);
		_vLvlNumberImage.push_back(tempImage);
	}
}
void statusOverviewPanel::hpUpdate(float currentGauge, float maxGauge)
{
	_progressBar[1].update(
		_statusPanelImage->getX() + 335,
		_statusPanelImage->getY() + 102,
		currentGauge,
		maxGauge);
	//자리수 계산하기
	int tempNum = currentGauge;
	while (tempNum != 0)
	{
		tempNum = tempNum / 10;
		_hpDigit++;
	}

	//자릿수만큼 이미지 벡터에 넣기
	int hpNumber;
	for (int i = 0; i < _hpDigit; i++)
	{
		int tempNumber2;
		tempNumber2 = pow(10, _hpDigit - (i + 1));
		hpNumber = ((int)currentGauge / (tempNumber2)) % 10;
		sprintf(_hpStr, "%d", hpNumber);
		image* tempImage;
		tempImage = new image;
		tempImage = IMAGEMANAGER->findImage(_hpStr);
		_vHpNumberImage.push_back(tempImage);
	}
}
void statusOverviewPanel::spUpdate(float currentGauge, float maxGauge)
{
	_progressBar[2].update(
		_statusPanelImage->getX() + 335,
		_statusPanelImage->getY() + 130,
		currentGauge,
		maxGauge);
	//자리수 계산하기
	int tempNum = currentGauge;
	while (tempNum != 0)
	{
		tempNum = tempNum / 10;
		_spDigit++;
	}

	//자릿수만큼 이미지 벡터에 넣기
	int spNumber;
	for (int i = 0; i < _spDigit; i++)
	{
		int tempNumber2;
		tempNumber2 = pow(10, _spDigit - (i + 1));
		spNumber = ((int)currentGauge / (tempNumber2)) % 10;
		sprintf(_spStr, "%d", spNumber);
		image* tempImage;
		tempImage = new image;
		tempImage = IMAGEMANAGER->findImage(_spStr);
		_vSpNumberImage.push_back(tempImage);
	}
}
void statusOverviewPanel::render(string name)
{
	//패널 렌더해주기
	_statusPanelImage->render(getMemDC());
	
	//패널안 프로그레스바 렌더해주기
	for (int i = 0; i < 3; i++)
	{
		_progressBar[i].render();
	}

	//****************************//
	//		넘버		그려주기		 //
	//****************************//
	for (int i = 0; i < _lvldigit; i++)
	{
		_vLvlNumberImage[i]->render(getMemDC(), _statusPanelImage->getX() + 225 + i * 20, _statusPanelImage->getY() + 64);
	}
	for (int i = 0; i < _hpDigit; i++)
	{
		_vHpNumberImage[i]->render(getMemDC(), _statusPanelImage->getX() + 225 + i * 20, _statusPanelImage->getY() + 88);
	}
	for (int i = 0; i < _spDigit; i++)
	{
		_vSpNumberImage[i]->render(getMemDC(), _statusPanelImage->getX() + 225 + i * 20, _statusPanelImage->getY() + 117);
	}
	//****************************//
	//		알파벳이름그려주기		 //
	//****************************//
	int tempNum;
	for (int i = 0; i < name.size(); i++)
	{ 
		string strName;
		ZeroMemory(&strName, sizeof(strName));
		strName = name;
		string chrName;
		ZeroMemory(&chrName, sizeof(strName));
		chrName = strName.at(i);
		if (chrName < "a")
		{
			tempNum = (int)strName.at(i);
			
			chrName = tempNum+32;
		}

		IMAGEMANAGER->findImage(chrName.c_str())->render(getMemDC(), (_statusPanelImage->getX()+ 180) + i * 20, _statusPanelImage->getY()+30);
	}
	//****************************//
	//		캐릭터 사진 그려주기	 //
	//****************************//
	char strFace[100];
	const char* strTemp;
	strTemp = name.c_str();
	memset(&strFace,0, sizeof(strFace));
	sprintf(strFace, "%sFace", strTemp);
	IMAGEMANAGER->findImage(strFace)->render(getMemDC(), _statusPanelImage->getX()+35, _statusPanelImage->getY()+15);
}