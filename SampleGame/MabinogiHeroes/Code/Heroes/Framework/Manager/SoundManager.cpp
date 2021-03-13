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
	//���� �ý��� ����
	System_Create(&_system);

	//ä�� �� ���� �� �ʱ�ȭ
	_system->init(TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);

	//ä�ΰ� ���带 �����Ҵ�
	//�޸� ���� �� ���� ����
	_sound = new Sound*[TOTAL_SOUND_CHANNEL];
	_channel = new Channel*[TOTAL_SOUND_CHANNEL];

	//�ʱ�ȭ
	ZeroMemory(_sound, sizeof(Sound*) * (TOTAL_SOUND_CHANNEL));
	ZeroMemory(_channel, sizeof(Channel*) * (TOTAL_SOUND_CHANNEL));

}

void SoundManager::Destroy()
{
	//���� ����
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

	//�޸� �����
	if (_channel) { delete[](_channel);	(_channel) = NULL; }
	if (_sound) { delete[](_sound);	(_sound) = NULL; }

	//�ý��� �ݱ�
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
		//��������̴�?
		if (background)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	//���� �ƴϸ�
	else
	{
		//�ѹ��� �÷����ϸ� �ǰ���
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� ��������
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
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);

			//���� ����
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
			//���� ����
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
		//���� ����
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
			//���� �Ͻ� ����
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
			//���� ����~~~
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
			//���� �������ִ�
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
			//���� ����~~~
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}

void SoundManager::update()
{//���� System ��������� ������Ʈ
	_system->update();

	//������ �ٲ�ų�
	//����� ���� ���带 ä�ο��� �����µ��� �پ���
	//�۾��� �ڵ����� ���ش�
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
