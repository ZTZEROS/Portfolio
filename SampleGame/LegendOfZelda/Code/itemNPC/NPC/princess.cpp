#include "stdafx.h"
#include "../../stdafx.h"
#include "princess.h"


princess::princess()
{
}

princess::~princess()
{
}

HRESULT princess::init(void)
{
	npc::init();
	_nState = STATE_ECO;
	speed = 0;
	pause = true;
	procedure = 0;
	rt = { 165, 600, 860, 700 };
	//ECO1
	_mList.insert(make_pair("1_01", "도와주세요. 저는 성의 지하감옥에 붙잡혀 있습니다."));
	_mList.insert(make_pair("1_02", "제 이름은 젤다...."));
	_mList.insert(make_pair("1_03", "이미 여섯이 산제물로 바쳐지고, 제가 마지막 하나입니다."));
	_mList.insert(make_pair("1_04", "성에 쳐들어온 사제 아그님은 산제물을 이용해, 일곱 현자의 봉인을 깨려고 합니다. 도와주세요."));
	//ECO2
	_mList.insert(make_pair("1_05", "도와주세요. 전 성 안의 던전 안에 갇혀있습니다. 정원에 성으로 들어갈 수 있는 비밀 통로가 있다고 알고 있습니다."));
	//NONE_MASTER_KEY
	_mList.insert(make_pair("1_06", "이 문은 잠겼어요! 커다란 열쇠를 가지고 있으면 열 수 있어요!"));
	//CASTEL ZELDA
	_mList.insert(make_pair("1_07", "고마워요 링크. 당신이 가까이 오고 있는 것을 느꼈어요."));
	_mList.insert(make_pair("1_08", "잘 들으세요 링크. 마법사가 성 안의 군인들을 조종하고 있어요. 제 아버지에게 좋지 않은 일이 있음을 느껴요. "));
	_mList.insert(make_pair("1_09", "사제 아그님은 강력한 마력을 가진 사람의 탈을 쓴 악마입니다!"));
	_mList.insert(make_pair("1_10", "제 말을 이해하셨나요?"));
	_mList.insert(make_pair("1_11", "좋아요. 아그님이 눈치채기 전에 이곳을 빠져나가요. 제가 비밀 통로를 알고 있어요. 먼저 일 층으로 가야해요."));

	return S_OK;
}
void princess::release(void)
{
	npc::release();
}
void princess::update(void)
{
	npc::update();
	//chat();
	speed++;

}
void princess::render(void)
{
	npc::render();
	HFONT normal = CreateFont(25, 0, 0, 0, 600, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, TEXT("HY엽서L"));
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SelectObject(getMemDC(), normal);
	if(CurrentMenu == 0) chat();
}

void princess::framePlay()
{
	npc::framePlay();
}

void princess::move(void)
{
	switch (_nState)
	{
		case STATE_ECO:
		break;
		case STATE_UP:
		break;
		case STATE_DOWN:
		break;
		case STATE_LEFT:
		break;
		case STATE_RIGHT:
		break;
	}
}
void princess::chat(void)
{
	switch (_nState)
	{
	case STATE_ECO:
			
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			procedure++;
			textframe = 0;// strlen(_mIter->second.c_str());
		}

		if (pause && procedure==0)
		{
			Talking = 1;
			_mIter = _mList.find("1_01");
		//	DrawText(getMemDC(), 160, WINSIZEY - 150, _mIter->second.c_str(), textframe);		
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (pause && procedure == 1) //*&& KEYMANAGER->isOnceKeyDown('Z')*/&& textframe > strlen(_mIter->second.c_str()))
		{
			_mIter = _mList.find("1_02");
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (pause &&procedure == 2) // _mIter == _mList.find("1_02") /*&& KEYMANAGER->isOnceKeyDown('Z')*/ && textframe > strlen(_mIter->second.c_str()))
		{
		//	textframe = 0;
			_mIter = _mList.find("1_03");
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (pause && procedure == 3) //_mIter == _mList.find("1_03") /*&& KEYMANAGER->isOnceKeyDown('Z')*/ && textframe > strlen(_mIter->second.c_str()))
		{
		//	textframe = 0;
			_mIter = _mList.find("1_04");
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (pause &&procedure == 4) // _mIter == _mList.find("1_04") /*&& KEYMANAGER->isOnceKeyDown('Z')*/ && textframe > strlen(_mIter->second.c_str()))
		{
		//	textframe = 0;
			pause = false;
			_nState = STATE_NONE;
			procedure = 0;
			Talking = 0;
		}
		if (speed >= 5 && textframe < strlen(_mIter->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
		break;
	case STATE_NONE:
		break;
	case STATE_ECO2:

		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			procedure++;
			textframe = 0;// strlen(_mIter->second.c_str());
		}
		if (procedure == 0)
		{
			Talking = 1;
			_mIter = _mList.find("1_05");
			//	DrawText(getMemDC(), 160, WINSIZEY - 150, _mIter->second.c_str(), textframe);		
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (procedure == 1)
		{
			Talking = 0;
			_nState = STATE_NONE;
			procedure = 0;
		}
		if (speed >= 5 && textframe < strlen(_mIter->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
		break;
	case STATE_UP:
		break;
	case STATE_DOWN:
		break;
	case STATE_LEFT:
		break;
	case STATE_RIGHT:
		break;
	}
}