#pragma once
#include <map>
#include <string>

//fmod.hpp �߰�
#include "../inc/fmod.hpp"

//lib ��ũ
#pragma comment (lib, "./Framework/lib/fmodex_vc.lib")

//�������� ä�� ���� ���� (����)
#define EXTRA_SOUND_CHANNEL 10
#define SOUNDBUFFER 500

//�� ���� ����
#define TOTAL_SOUND_CHANNEL EXTRA_SOUND_CHANNEL + SOUNDBUFFER

using namespace FMOD;
using namespace std;


class SoundManager
{
private:
	SoundManager();
	SoundManager(const SoundManager& other);
	~SoundManager();
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;
	typedef map<string, Channel**> arrChannels;
	typedef arrChannels::iterator arrChannelIter;

private:
	System* _system;
	Sound** _sound;

	Channel** _channel;

	arrSounds _mTotalSounds;

public:

	static SoundManager& Get()
	{
		static SoundManager instance;
		return instance;
	}

	void Initilize();
	void Destroy();

	//���� �߰�
	void addSound(string keyName, string soundName, bool background = false, bool loop = false);

	//���
	void play(string keyName, float volume);

	//����
	void stop(string keyName);

	void StopAllSound();

	//�Ͻ� ����
	void pause(string keyName);

	//�ٽ� ���
	void resume(string keyName);

	//�����ִ�?
	bool isPauseSound(string keyName);

	//������̴�?
	bool isPlaySound(string keyName);

	//fmod �ý��� ����
	void update();

	unsigned int getLength(string keyName);

	//���� �ð� ����
	void SetSoundTime(string keyName, unsigned int time);

};

