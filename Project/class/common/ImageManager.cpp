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
	GetID("ball","resource/image/ball/ball32.png");

	//�v���C���[
	GetID("idle", "resource/image/character/sprite/idle.png", Vector2Int(1, 8), Vector2Int(19,31));
	GetID("jumpUp", "resource/image/character/sprite/jumpUp.png", Vector2Int(1, 2), Vector2Int(20,30));
	GetID("down", "resource/image/character/sprite/down.png", Vector2Int(1, 2), Vector2Int(19, 30));
	GetID("run", "resource/image/character/sprite/run.png", Vector2Int(1, 10), Vector2Int(26, 31));
}

ImageManager::~ImageManager()
{
}
