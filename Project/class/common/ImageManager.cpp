#include<DxLib.h>
#include<algorithm>
#include<cassert>
#include "ImageManager.h"
#include"../../_debug/_DebugConOut.h"

const std::vector<int>& ImageManager::GetID(std::string key)
{
	//�摜��ǂݍ���ł邩
	if (imageMap_.count(key) == 0)
	{
		//�ǂݍ���łȂ��ꍇ�̓��T�C�Y���ĉ摜��ǂݍ���
		imageMap_[key].resize(1);
		imageMap_[key][0] = LoadGraph(key.c_str());
	}
	return imageMap_[key];
}

const std::vector<int>& ImageManager::GetID(std::string key, std::string path)
{
	//�摜��ǂݍ���ł邩
	if (imageMap_.count(key) == 0)
	{
		//�ǂݍ���łȂ��ꍇ�̓��T�C�Y���ĉ摜��ǂݍ���
		imageMap_[key].resize(1);
		imageMap_[key][0] = LoadGraph(path.c_str());
	}
	return imageMap_[key];
}

const std::vector<int>& ImageManager::GetID(std::string key, std::string path, Vector2Int divCount, Vector2Int divSize)
{
	if (imageMap_.count(key) == 0)
	{
		imageMap_[key].resize(static_cast<UINT>(divCount.x*divCount.y));

		if (LoadDivGraph(path.c_str(),
			divCount.x * divCount.y,
			divCount.x, divCount.y,
			divSize.x, divSize.y,
			&imageMap_[key][0],
			true) == -1)
		{
			TRACE("���s");
		}
	}
	return imageMap_[key];
}

ImageManager::ImageManager()
{
	//�^�C�g��
	GetID("titlelogo", "resource/image/titlelogo.png");
	GetID("titleBg","resource/image/stage/titleBg.png");
	GetID("backBills", "resource/image/stage/backBills.png");
	GetID("middleBills", "resource/image/stage/middleBills.png");
	GetID("road", "resource/image/stage/road.png");

	//�Z���N�g
	GetID("selectBg","resource/image/stage/selectBg.png");
	GetID("selectLogo","resource/image/stage/selectLogo.png");
	GetID("selectStage","resource/image/stage/selectStage.png");
	GetID("selectStageNull","resource/image/stage/selectStageNull.png");

	//�X�e�[�W
	GetID("stage","resource/image/stage/stage.png");

	//�{�[��
	GetID("ball","resource/image/ball/ball.png");

	//�v���C���[1
	//GetID("idle", "resource/image/character/sprite/idle2.png", Vector2Int(8, 1), Vector2Int(40, 38));

	GetID("knight_idle", "resource/image/character/sprite/knight/idle.png", Vector2Int(6, 1), Vector2Int(98, 66));
	GetID("knight_jumpUp", "resource/image/character/sprite/knight/jumpUp.png", Vector2Int(7, 1), Vector2Int(98,66));
	GetID("knight_fall", "resource/image/character/sprite/knight/down.png", Vector2Int(3, 1), Vector2Int(98, 66));
	GetID("knight_run", "resource/image/character/sprite/knight/run.png", Vector2Int(8, 1), Vector2Int(98, 66));
	GetID("knight_attack", "resource/image/character/sprite/knight/attack.png", Vector2Int(17, 1), Vector2Int(98,66));

	

	//�v���C���[2
	GetID("rogue_idle", "resource/image/character/sprite/rogue/idle.png", Vector2Int(8, 1), Vector2Int(40, 38));

}

ImageManager::~ImageManager()
{
}
