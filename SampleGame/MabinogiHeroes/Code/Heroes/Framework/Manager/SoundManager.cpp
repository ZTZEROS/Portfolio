#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager() : _system(NULL), _channel(NULL), _sound(NULL)
{
}


SoundManager::~SoundManager()
{
}

void SoundManager::Initilize()
{
	//사운드 시스템 생성
	System_Create(&_system);

	//채널 수 설정 및 초기화
	_system->init(TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);

	//채널과 사운드를 동적할당
	//메모리 버퍼 및 사운드 생성
	_sound = new Sound*[TOTAL_SOUND_CHANNEL];
	_channel = new Channel*[TOTAL_SOUND_CHANNEL];

	//초기화
	ZeroMemory(_sound, sizeof(Sound*) * (TOTAL_SOUND_CHANNEL));
	ZeroMemory(_channel, sizeof(Channel*) * (TOTAL_SOUND_CHANNEL));

}

void SoundManager::Destroy()
{
	//사운드 삭제
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTAL_SOUND_CHANNEL; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}

			if (_sound != NULL)
			{
				if (_sound != NULL) _sound[i]->release();
			}
		}
	}

	//메모리 지우기
	if (_channel) { delete[](_channel);	(_channel) = NULL; }
	if (_sound) { delete[](_sound);	(_sound) = NULL; }

	//시스템 닫기
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void SoundManager::addSound(string keyName, string soundName, bool background, bool loop)
{
	if (loop)
	{
		//배경음악이니?
		if (background)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	//루프 아니면
	else
	{
		//한번만 플레이하면 되겠지
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//맵에 사운드를 키값과 함께 저장하자
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void SoundManager::play(string keyName, float volume)
{
	arrSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);

			//볼륨 설정
			_channel[count]->setVolume(volume);
			break;
		}
	}
}

void SoundManager::stop(string keyName)
{
	arrSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 정지
			_channel[count]->stop();
			break;
		}
	}
}

void SoundManager::StopAllSound()
{
	arrSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (; iter != _mTotalSounds.end(); ++iter, count++)
	{
		//사운드 정지
		_channel[count]->stop();
	}
}

void SoundManager::pause(string keyName)
{
	arrSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 일시 정지
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::resume(string keyName)
{
	arrSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 정지~~~
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::isPauseSound(string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 정지해있니
			_channel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}

bool SoundManager::isPlaySound(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 정지~~~
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}

void SoundManager::update()
{//사운드 System 계속적으로 업데이트
	_system->update();

	//볼륨이 바뀌거나
	//재생이 끝난 사운드를 채널에서 빼내는등의 다양한
	//작업을 자동으로 해준다
}

unsigned int SoundManager::getLength(string keyName)
{
	arrSoundIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			(*iter->second)->getLength(&length, FMOD_TIMEUNIT_MS);
			return length;
		}
	}

	return 0;
}

void SoundManager::SetSoundTime(string keyName, unsigned int time)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPosition(time*1000, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}
