#include "stdafx.h"
#include "HeroesSound.h"


HeroesSound::HeroesSound()
{
}


HeroesSound::~HeroesSound()
{
}

void HeroesSound::Initilize()
{
	EffectSound();
	MonsterSound();
	BackGroundSoud();
	SkillSound();

}

void HeroesSound::EffectSound()
{
	SoundManager::Get().addSound("Hit_bolt_01", "../ResourceData/Sound/EffectSound/Hit_bolt_01.wav", false, false);
	SoundManager::Get().addSound("Hit_bolt_02", "../ResourceData/Sound/EffectSound/Hit_bolt_02.wav", false, false);
	SoundManager::Get().addSound("Hit_bolt_03", "../ResourceData/Sound/EffectSound/Hit_bolt_03.wav", false, false);
	SoundManager::Get().addSound("Hit_bolt_metal_01", "../ResourceData/Sound/EffectSound/Hit_bolt_metal_01.wav", false, false);
	SoundManager::Get().addSound("Hit_bolt_metal_02", "../ResourceData/Sound/EffectSound/Hit_bolt_metal_02.wav", false, false);
	SoundManager::Get().addSound("Hit_bolt_metal_03", "../ResourceData/Sound/EffectSound/Hit_bolt_metal_03.wav", false, false);
	SoundManager::Get().addSound("walk_03", "../ResourceData/Sound/EffectSound/walk_03.wav", false, false);
}

void HeroesSound::MonsterSound()
{
	SoundManager::Get().addSound("succubus_05", "../ResourceData/Sound/MonsterSound/succubus_05.wav", false, false); //웃음
	SoundManager::Get().addSound("succubus_casting_fire_02", "../ResourceData/Sound/MonsterSound/succubus_casting_fire_02.wav", false, false); //파이어 캐스팅
	SoundManager::Get().addSound("succubus_dying_02", "../ResourceData/Sound/MonsterSound/succubus_dying_02.wav", false, false);//죽을때
	SoundManager::Get().addSound("succubus_laugh_01", "../ResourceData/Sound/MonsterSound/succubus_laugh_01.wav", false, false); //짧은 웃음
	SoundManager::Get().addSound("succubus_laugh_long_01", "../ResourceData/Sound/MonsterSound/succubus_laugh_long_01.wav", false, false); //긴웃음
	SoundManager::Get().addSound("succubus_laugh_spin_04", "../ResourceData/Sound/MonsterSound/succubus_laugh_spin_04.wav", false, false); //돌면서 공격
	SoundManager::Get().addSound("succubus_queen_appear", "../ResourceData/Sound/MonsterSound/succubus_queen_appear.wav", false, false); // 나타남
	SoundManager::Get().addSound("succubus_queen_camera3_stereo", "../ResourceData/Sound/MonsterSound/succubus_queen_camera3_stereo.wav", false, false); //화면이동 공격
	SoundManager::Get().addSound("succubus_step_01", "../ResourceData/Sound/MonsterSound/succubus_step_01.wav", false, false); //걸음소리

	//헬레이디
	SoundManager::Get().addSound("lady_dying_04", "../ResourceData/Sound/MonsterSound/lady_dying_04.wav", false, false); //죽음
	SoundManager::Get().addSound("lady_hurt_strong_03", "../ResourceData/Sound/MonsterSound/lady_hurt_strong_03.wav", false, false); //공격당함
	SoundManager::Get().addSound("lady_shout_01", "../ResourceData/Sound/MonsterSound/lady_shout_01.wav", false, false); //공격
}

void HeroesSound::BackGroundSoud()
{
	SoundManager::Get().addSound("아이디-01-Sign (Feat. Loopy)", "../ResourceData/Sound/BackGroundSound/아이디-01-Sign (Feat. Loopy).mp3", true, true);
	SoundManager::Get().addSound("bgm_colhen_01", "../ResourceData/Sound/BackGroundSound/bgm_colhen_01.mp3", true, true);
	SoundManager::Get().addSound("bgm_colhen_inn", "../ResourceData/Sound/BackGroundSound/bgm_colhen_inn.mp3", true, true);
	SoundManager::Get().addSound("bgm_ep8_succubus", "../ResourceData/Sound/BackGroundSound/bgm_ep8_succubus.wav", true, true);
	SoundManager::Get().addSound("bgm_colhen_inn_sereuha", "../ResourceData/Sound/BackGroundSound/bgm_colhen_inn_sereuha.wav", true, true);
}

void HeroesSound::SkillSound()
{
	SoundManager::Get().addSound("lethita_attack1", "../ResourceData/Sound/SkillSound/lethita_attack1.wav", false, false);
	SoundManager::Get().addSound("lethita_attack2", "../ResourceData/Sound/SkillSound/lethita_attack2.wav", false, false);
	SoundManager::Get().addSound("lethita_attack3", "../ResourceData/Sound/SkillSound/lethita_attack3.wav", false, false);
	SoundManager::Get().addSound("lethita_attack4", "../ResourceData/Sound/SkillSound/lethita_attack4.wav", false, false);
	SoundManager::Get().addSound("lethita_attack5", "../ResourceData/Sound/SkillSound/lethita_attack5.wav", false, false);
	SoundManager::Get().addSound("lethita_attack6", "../ResourceData/Sound/SkillSound/lethita_attack6.wav", false, false);
	SoundManager::Get().addSound("lethita_attack7", "../ResourceData/Sound/SkillSound/lethita_attack7.wav", false, false);
	SoundManager::Get().addSound("lethita_attack8", "../ResourceData/Sound/SkillSound/lethita_attack8.wav", false, false);
}
