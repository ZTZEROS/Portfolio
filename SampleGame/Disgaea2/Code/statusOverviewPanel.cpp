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
	//�г� �̹��� ������ְ�~
	_statusPanelImage = IMAGEMANAGER->addImage(
		"��������",
		"UI_IMAGE/battleUI.bmp",
		606,
		161,
		true,
		MAGENTA);
	//�г��̹��� ��ġ����~
	_statusPanelImage->setX(x);
	_statusPanelImage->setY(y);
	//�гξȿ��� ����� ���α׷��� ������ְ�~
	_progressBar[0].overviewPanelLevelInit("�������ġ", "�ִ����ġ", 0, 0);
	_progressBar[1].overviewPanelHPInit("����ü��", "�ִ�ü��", 0, 0);
	_progressBar[2].overviewPanelSPInit("���縶��", "�ִ븶��", 0, 0);

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
	//�����̹������� �����ְ�~
	_vLvlNumberImage.clear();
	_vHpNumberImage.clear();
	_vSpNumberImage.clear();
	//���� �ڸ��� �ʱ�ȭ~
	_lvldigit = 0;
	_hpDigit = 0;
	_spDigit = 0;
	//����ġ ������Ʈ
	expUpdate(currentExp, maxExp);
	//ü�� ������Ʈ~
	hpUpdate(currentHp, maxHp);
	//sp ������Ʈ~
	spUpdate(currentSp, maxSp);
	//�г� ��ġ ������Ʈ~
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
	//�ڸ��� ����ϱ�
	while (tempNum != 0)
	{
		tempNum = tempNum / 10;
		_lvldigit++;
	}

	//�ڸ�����ŭ �̹��� ���Ϳ� �ֱ�
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
	//�ڸ��� ����ϱ�
	int tempNum = currentGauge;
	while (tempNum != 0)
	{
		tempNum = tempNum / 10;
		_hpDigit++;
	}

	//�ڸ�����ŭ �̹��� ���Ϳ� �ֱ�
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
	//�ڸ��� ����ϱ�
	int tempNum = currentGauge;
	while (tempNum != 0)
	{
		tempNum = tempNum / 10;
		_spDigit++;
	}

	//�ڸ�����ŭ �̹��� ���Ϳ� �ֱ�
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
	//�г� �������ֱ�
	_statusPanelImage->render(getMemDC());
	
	//�гξ� ���α׷����� �������ֱ�
	for (int i = 0; i < 3; i++)
	{
		_progressBar[i].render();
	}

	//****************************//
	//		�ѹ�		�׷��ֱ�		 //
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
	//		���ĺ��̸��׷��ֱ�		 //
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
	//		ĳ���� ���� �׷��ֱ�	 //
	//****************************//
	char strFace[100];
	const char* strTemp;
	strTemp = name.c_str();
	memset(&strFace,0, sizeof(strFace));
	sprintf(strFace, "%sFace", strTemp);
	IMAGEMANAGER->findImage(strFace)->render(getMemDC(), _statusPanelImage->getX()+35, _statusPanelImage->getY()+15);
}