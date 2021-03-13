#pragma once
#include "gameNode.h"
#include "progressBar.h"
class statusOverviewPanel : public gameNode
{
private:
	//*******************************//
	//			이미지 변수			 //
	//*******************************//
	image* _statusPanelImage;
	vector<image*> _vLvlNumberImage;
	vector<image*> _vHpNumberImage;
	vector<image*> _vSpNumberImage;
private:
	progressBar _progressBar[3];
private:
	//각 능력치가 몇자리수인지 담을 변수
	int _lvldigit, _hpDigit, _spDigit;
	//각 능력치의 숫자를 문자형으로 바꿔서 담을 변수
	char _lvlStr[2] ,_hpStr[2], _spStr[2];
	//업데이트를 한번씩만하기위해 만든 맘에들지않는 급조변수 
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

