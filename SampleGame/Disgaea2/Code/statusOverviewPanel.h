#pragma once
#include "gameNode.h"
#include "progressBar.h"
class statusOverviewPanel : public gameNode
{
private:
	//*******************************//
	//			�̹��� ����			 //
	//*******************************//
	image* _statusPanelImage;
	vector<image*> _vLvlNumberImage;
	vector<image*> _vHpNumberImage;
	vector<image*> _vSpNumberImage;
private:
	progressBar _progressBar[3];
private:
	//�� �ɷ�ġ�� ���ڸ������� ���� ����
	int _lvldigit, _hpDigit, _spDigit;
	//�� �ɷ�ġ�� ���ڸ� ���������� �ٲ㼭 ���� ����
	char _lvlStr[2] ,_hpStr[2], _spStr[2];
	//������Ʈ�� �ѹ������ϱ����� ���� ���������ʴ� �������� 
	bool _isUpdated;
public:
	HRESULT init(int x, int y);
	void release(void);
	void update(float currentExp, float maxExp, 
				float currentHp, float maxHp,
				float currentSp, float maxSp,
				int x, int y);
	void expUpdate(float currentGauge, float maxGauge);
	void spUpdate (float currentGauge, float maxGauge);
	void hpUpdate (float currentGauge, float maxGauge);
	void render(string name);
public:
	statusOverviewPanel();
	~statusOverviewPanel();
};

