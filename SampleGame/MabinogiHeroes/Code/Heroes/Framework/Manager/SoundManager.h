#pragma once
#include <map>
#include <string>

//fmod.hpp 추가
#include "../inc/fmod.hpp"

//lib 링크
#pragma comment (lib, "./Framework/lib/fmodex_vc.lib")

//여유분의 채널 갯수 설정 (버퍼)
#define EXTRA_SOUND_CHANNEL 10
#define SOUNDBUFFER 500

//총 사운드 갯수
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

	//사운드 추가
	void addSound(string keyName, string soundName, bool background = false, bool loop = false);

	//재생
	void play(string keyName, float volume);

	//종료
	void stop(string keyName);

	void StopAllSound();

	//일시 정지
	void pause(string keyName);

	//다시 재생
	void resume(string keyName);

	//멈춰있니?
	bool isPauseSound(string keyName);

	//재생중이니?
	bool isPlaySound(string keyName);

	//fmod 시스템 갱신
	void update();

	unsigned int getLength(string keyName);

	//사운드 시간 조정
	void SetSoundTime(string keyName, unsigned int time);

};

