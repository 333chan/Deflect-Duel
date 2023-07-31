#include <DxLib.h>
#include"SceneManager.h"
#include"../common/SoundManager.h"
#include"../common/ImageManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "TitelScene.h"
#include "SelectScene.h"
#include "VersusScene.h"

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

	DrawExtendGraph(0, 0, lpSceneMng.GetScreenSize().x,lpSceneMng.GetScreenSize().y, lpImageMng.GetID("selectBg")[0], true);


	//tmxから位置とサイズ取得
	for (const auto& imagePos : tmxObj_.GetSelectStageList())
	{
		const auto& [sPos, size] = imagePos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, lpImageMng.GetID("selectStageNull")[0], true);
	}

	//tmxから位置とサイズ取得
	for (const auto& imagePos : tmxObj_.GetSelectStageList())
	{
		//始点とサイズに
		const auto& [sPos, size] = imagePos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, lpImageMng.GetID("selectStage")[0], true);
		break;
	}

	//tmxの位置とサイズ取得
	for (const auto& bgPos : tmxObj_.GetSelectLogoList())
	{
		//始点とサイズに
		const auto& [sPos, size] = bgPos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, lpImageMng.GetID("selectLogo")[0], true);
	}

	//ステージの名前
	if (GetJoypadNum() >= 1)
	{
		DrawString(100, lpSceneMng.GetScreenSize().y - 100, "倉庫/Push LB", 0xffffff);
	}
	else
	{
		DrawString(100, lpSceneMng.GetScreenSize().y - 100, "倉庫/Press 1", 0xffffff);
	}

	DrawString(380, lpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);
	DrawString(700, lpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);
	DrawString(1000, lpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, lpSceneMng.GetScreenSize().y, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y-30, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	DrawString(10, lpSceneMng.GetScreenSize().y - 20, "A:タイトルに戻る", 0xffffff, true);


#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "Select");
#endif


}

void SelectScene::Release(void)
{
}

UniqueScene SelectScene::UpdateScene(UniqueScene& scene)
{

	if (controller_->ChaeckInputKey(KeyID::Stage1))
	{
		ChangeVolumeSoundMem(255, lpSoundMng.GetID("decideSe"));
		PlaySoundMem(lpSoundMng.GetID("decideSe"), DX_PLAYTYPE_BACK);

		return std::make_unique<VersusScene>();
	}
	if (controller_->ChaeckInputKey(KeyID::Cancel))
	{
		ChangeVolumeSoundMem(255, lpSoundMng.GetID("decideSe"));
		PlaySoundMem(lpSoundMng.GetID("decideSe"), DX_PLAYTYPE_BACK);

		StopSoundMem(lpSoundMng.GetID("bgm"));
		return std::make_unique<TitelScene>();
	}

	//デバック用
#ifdef _DEBUG



#endif

	//元のシーンに戻す
	return std::move(scene);
}
