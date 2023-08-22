#include <DxLib.h>
#include"SceneManager.h"
#include"../common/SoundManager.h"
#include"../common/ImageManager.h"
#include "../common/AnimController.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "../input/PadInput2.h"
#include "GameScene.h"
#include "VersusScene.h"

VersusScene::VersusScene()
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

VersusScene::~VersusScene()
{
	//解放
	Release();
}

void VersusScene::Init(void)
{
	animController_ = std::make_unique<AnimController>();

	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/VersusScene.tmx", false);

	//tmxのCollLiset取得
	for (auto& coll : tmxObj_.GetTitleBgimageList())
	{
		bgPos_ = coll.first;
		bgEndPos_ = coll.first + coll.second;
	}
	for (auto& coll : tmxObj_.Getp1windowList())
	{
		p1windowPos_ = coll.first;
		p1windowEndPos_ = coll.first + coll.second;
	}	
	for (auto& coll : tmxObj_.Getp2windowList())
	{
		p2windowPos_ = coll.first;
		p2windowEndPos_ = coll.first + coll.second;
	}
	animController_ = std::make_unique<AnimController>();

}

UniqueScene VersusScene::Update(UniqueScene scene)
{
	controller_->Update();


	DrawScreen();
	return UpdateScene(scene);
}

void VersusScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "VersusScene");
#endif

	animController_->SetAnim(Anim::Idle);

	//背景
	DrawExtendGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, lpImageMng.GetID("vsBg")[0], true);

	//1Pプレイヤー
	DrawExtendGraph(p1windowPos_.x, p1windowPos_.y, p1windowEndPos_.x, p1windowEndPos_.y, lpImageMng.GetID("playerWindow")[0], true);
	DrawExtendGraph(p1windowPos_.x+50, p1windowPos_.y+90, p1windowEndPos_.x-50, p1windowEndPos_.y-555, lpImageMng.GetID("vsPlayer1Logo")[0], true);

	DrawRotaGraph(
		p1windowPos_.x + 264, p1windowPos_.y + 250,
		3.00f,
		0,
		lpImageMng.GetID("knight_idle")[animController_->Update()],
		true,-1);

	//コントローラー
	DrawRotaGraph(
		p1windowPos_.x + 300, p1windowPos_.y + 450,
		2.50f,
		0,
		lpImageMng.GetID("conImage")[0],
		true, -1);



	DrawExtendGraph(p2windowPos_.x, p2windowPos_.y, p2windowEndPos_.x, p2windowEndPos_.y, lpImageMng.GetID("playerWindow")[0], true);
	DrawExtendGraph(p2windowPos_.x + 50, p2windowPos_.y + 90, p2windowEndPos_.x - 50, p2windowEndPos_.y - 555, lpImageMng.GetID("vsPlayer2Logo")[0], true);

	DrawRotaGraph(
		p2windowPos_.x + 264, p2windowPos_.y + 250,
		3.00f,
		0,
		lpImageMng.GetID("rogue_idle")[animController_->Update()],
	    true, 1);

	//コントローラー
	DrawRotaGraph(
		p2windowPos_.x + 300, p2windowPos_.y + 450,
		2.50f,
		0,
		lpImageMng.GetID("conImage")[0],
		true, -1);


	DrawExtendGraph(
		lpSceneMng.GetScreenSize().x / 2 - 80, lpSceneMng.GetScreenSize().y / 2 + 80,
		lpSceneMng.GetScreenSize().x / 2 + 80, lpSceneMng.GetScreenSize().y / 2 + 150,
		lpImageMng.GetID("window")[0], true);
	DrawFormatString(lpSceneMng.GetScreenSize().x/2-60, lpSceneMng.GetScreenSize().y / 2+100,0xffffff,"PRESS B BUTTON\n Start Battle");
}

void VersusScene::Release(void)
{
}

UniqueScene VersusScene::UpdateScene(UniqueScene& scene)
{
	if (controller_->ChaeckInputKey(KeyID::Decide))
	{
		ChangeVolumeSoundMem(255, lpSoundMng.GetID("decideSe"));
		PlaySoundMem(lpSoundMng.GetID("decideSe"), DX_PLAYTYPE_BACK);

		StopSoundMem(lpSoundMng.GetID("bgm"));
		return std::make_unique<GameScene>();
	}

	//元のシーンに戻す
	return std::move(scene);
}
