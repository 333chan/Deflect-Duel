#include<DxLib.h>
#include "SoundManager.h"

const int& SoundManager::GetID(std::string key, std::string path)
{
	//‰¹‚ğ“Ç‚İ‚ñ‚Å‚é‚©
	if (soundMap_.count(key)==0)
	{
		//“Ç‚İ‚ñ‚Å‚È‚¢ê‡‚Í“Ç‚İ‚Ş
		soundMap_[key] = LoadSoundMem(path.c_str());

	}
	return soundMap_[key];
}

const int& SoundManager::GetID(std::string key)
{
	//‰¹‚ğ“Ç‚İ‚ñ‚Å‚é‚©
	if (soundMap_.count(key) == 0)
	{
		//“Ç‚İ‚ñ‚Å‚È‚¢ê‡‚Í“Ç‚İ‚Ş
		soundMap_[key] = LoadSoundMem(key.c_str());

	}
	return soundMap_[key];
}

SoundManager::SoundManager()
{
	//ƒTƒEƒ“ƒh“Ç‚İ‚İ
	GetID("bgm", "resource/sound/titlebgm.mp3");
	GetID("decideSe", "resource/sound/decide.mp3");
	GetID("winSe", "resource/sound/win.mp3");
	GetID("gameBgm", "resource/sound/gamebgm.mp3");
	GetID("jumpSe","resource/sound/jump.mp3");
	GetID("attackSe","resource/sound/attackhit.wav");
	GetID("daethSe","resource/sound/daeth.wav");
	GetID("ballSe","resource/sound/ballSe.mp3");
}

SoundManager::~SoundManager()
{
}

