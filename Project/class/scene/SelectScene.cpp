#include <DxLib.h>
#include"SceneManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "GameScene.h"
#include "TitelScene.h"
#include "SelectScene.h"

SelectScene::SelectScene(int bgm)
{

	//コントローラーの生成
	if (GetJoypadNum() >= 1)
	{
		controller_ = std::make_unique<KeyInput>();

	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	//初期化
	Init();

	selectBgm = bgm;

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

	bgImageH_ = LoadGraph("resource/image/stage/selectBg.png", true);
	logoImageH_ = LoadGraph("resource/image/stage/stageselect.png", true);
	stage1ImageH_ = LoadGraph("resource/image/stage/selectStage.png",true);
	stageNullImageH_ = LoadGraph("resource/image/stage/selectStageNull.png", true);

	disSe = LoadSoundMem("resource/sound/dis.mp3");

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

	DrawExtendGraph(0, 0, IpSceneMng.GetScreenSize().x,IpSceneMng.GetScreenSize().y, bgImageH_, true);

	for (const auto& imagePos : tmxObj_.GetSelectStageList())
	{
		const auto& [sPos, size] = imagePos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, stageNullImageH_, true);
	}

	//tmxの位置とサイズ取得
	for (const auto& imagePos : tmxObj_.GetSelectStageList())
	{
		//始点とサイズに
		const auto& [sPos, size] = imagePos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, stage1ImageH_, true);
		break;
	}

	//tmxの位置とサイズ取得
	for (const auto& bgPos : tmxObj_.GetSelectLogoList())
	{
		//始点とサイズに
		const auto& [sPos, size] = bgPos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, logoImageH_, true);
	}

	DrawString(100, IpSceneMng.GetScreenSize().y - 100, "倉庫/Press 1",0xffffff);
	DrawString(380, IpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);
	DrawString(700, IpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);
	DrawString(1000, IpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);

	//DrawGraph(logoPos.x, logoPos.y, logoImageH_, true);

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "Select");
#endif


}

void SelectScene::Release(void)
{
	DeleteGraph(stage1ImageH_);
	DeleteGraph(logoImageH_);

}

UniqueScene SelectScene::UpdateScene(UniqueScene& scene)
{

	if (controller_->ChaeckInputKey(KeyID::Stage1))
	{
		ChangeVolumeSoundMem(255, disSe);
		PlaySoundMem(disSe, DX_PLAYTYPE_BACK);

		StopSoundMem(selectBgm);
		return std::make_unique<GameScene>();
	}

	//デバック用
#ifdef _DEBUG



#endif

	//元のシーンに戻す
	return std::move(scene);
}
