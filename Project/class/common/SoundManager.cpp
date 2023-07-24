#include<DxLib.h>
#include "SoundManager.h"

const int& SoundManager::GetID(std::string key, std::string path)
{
	//音を読み込んでるか
	if (soundMap_.count(key)==0)
	{
		//読み込んでない場合は読み込む
		soundMap_[key] = LoadSoundMem(path.c_str());

	}
	return soundMap_[key];
}

const int& SoundManager::GetID(std::string key)
{
	//音を読み込んでるか
	if (soundMap_.count(key) == 0)
	{
		//読み込んでない場合は読み込む
		soundMap_[key] = LoadSoundMem(key.c_str());

	}
	return soundMap_[key];
}

SoundManager::SoundManager()
{
	//サウンド読み込み
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

