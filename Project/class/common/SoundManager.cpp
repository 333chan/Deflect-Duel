#include<DxLib.h>
#include "SoundManager.h"

const int& SoundManager::GetID(std::string key, std::string path)
{
	//����ǂݍ���ł邩
	if (soundMap_.count(key)==0)
	{
		//�ǂݍ���łȂ��ꍇ�͓ǂݍ���
		soundMap_[key] = LoadSoundMem(path.c_str());

	}
	return soundMap_[key];
}

const int& SoundManager::GetID(std::string key)
{
	//����ǂݍ���ł邩
	if (soundMap_.count(key) == 0)
	{
		//�ǂݍ���łȂ��ꍇ�͓ǂݍ���
		soundMap_[key] = LoadSoundMem(key.c_str());

	}
	return soundMap_[key];
}

SoundManager::SoundManager()
{
	//�T�E���h�ǂݍ���
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

