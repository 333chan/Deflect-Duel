#include <DxLib.h>
#include"SceneManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "GameScene.h"
#include "SelectScene.h"

SelectScene::SelectScene()
{

	//コントローラーの生成
	if (GetJoypadNum() >= 1)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}


	//初期化
	Init();

	//ちらつき防止
	DrawScreen();
}

SelectScene::~SelectScene()
{
	Release();
}

void SelectScene::Init(void)
{
	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/selectScene.tmx", false);

	bgImageH_ = LoadGraph("resource/image/stage/titleBg.png", true);
	stage1ImageH_ = LoadGraph("resource/image/stage/stage.png",true);
}

UniqueScene SelectScene::Update(UniqueScene scene)
{
	controller_->Update();
	DrawScreen();

	return UpdateScene(scene);
}

void SelectScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	//tmxのCollLiset取得
	for (auto& coll : tmxObj_.GetTitleBgimageList())
	{
		bgPos = coll.first;
		bgPosEnd = coll.first + coll.second;
	}
	for (auto& coll : tmxObj_.GetSelectStageList())
	{
		stage1Pos = coll.first;
		stage1PosEnd = coll.first + coll.second;
	}

	DrawExtendGraph(bgPos.x, bgPos.y, bgPosEnd.x, bgPosEnd.y, bgImageH_, true);
	DrawExtendGraph(stage1Pos.x, stage1Pos.y, stage1PosEnd.x, stage1PosEnd.y, stage1ImageH_, true);
	DrawString(stage1Pos.x, stage1Pos.y-16, "倉庫/Press X",0xffffff);
	//DrawGraph(logoPos.x, logoPos.y, logoImageH_, true);

	DrawFormatString(0, 0, 0xffffff, "Select");


}

void SelectScene::Release(void)
{
	DeleteGraph(bgImageH_);

}

UniqueScene SelectScene::UpdateScene(UniqueScene& scene)
{
	//デバック用
#ifdef _DEBUG
	if (controller_->ChaeckInputKey(KeyID::Transition))
	{
		return std::make_unique<GameScene>();
	}


#endif

	//元のシーンに戻す
	return std::move(scene);
}
