#include <DxLib.h>
#include "Stage.h"
#include "../../scene/SceneManager.h"
#include"../../common/ImageManager.h"
#include "../../tmx/TmxObj.h"

Stage::Stage()
{
	Init();
}

Stage::~Stage()
{
	Release();
}

void Stage::Init()
{
	//tmx‚Ì“Ç‚İ‚İ
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);
}

void Stage::Update()
{

}

void Stage::Draw()
{
	

	//tmx‚ÌCollLisetæ“¾
	for (auto& coll : tmxObj_.GetStageimageList())
	{
		stagePos = coll.first;
		stagePosEnd = coll.second;
	}

	DrawExtendGraph(stagePos.x, stagePos.y,stagePosEnd.x, stagePosEnd.y , lpImageMng.GetID("stage")[0], true);
}

void Stage::Release()
{

}
