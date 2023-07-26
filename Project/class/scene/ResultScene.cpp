#include <DxLib.h>
#include "SceneManager.h"
#include"../common/SoundManager.h"
#include"../common/ImageManager.h"
#include "../object/player/Player.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "TitelScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "SelectScene.h"

ResultScene::ResultScene(PlayerType ptype)
{
	playertype_ = ptype;

	//パッドの数１つ以上の場合はパッド操作にする、現在は無効
	if (GetJoypadNum() > 0)
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

ResultScene::~ResultScene()
{
	//解放
	Release();
}

void ResultScene::Init(void)
{
	//現在の画面を画像で取得
	screenImage_ = MakeScreen(screenSizeX_, screenSizeY_, true);

	//取得した画像を描画
	GetDrawScreenGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, screenImage_, true);

	ChangeVolumeSoundMem(200, lpSoundMng.GetID("winSe"));
	PlaySoundMem(lpSoundMng.GetID("winSe"), DX_PLAYTYPE_BACK);
}

UniqueScene ResultScene::Update(UniqueScene scene)
{
	controller_->Update();
	DrawScreen();
	return UpdateScene(scene);
}

void ResultScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0, screenImage_,true);


	DrawBox(lpSceneMng.GetScreenSize().x / 2 - 151, lpSceneMng.GetScreenSize().y / 2 + 51, lpSceneMng.GetScreenSize().x / 2 + 101, lpSceneMng.GetScreenSize().y / 2 - 81, 0xffffff, true);
	DrawBox(lpSceneMng.GetScreenSize().x / 2-150, lpSceneMng.GetScreenSize().y / 2+50, lpSceneMng.GetScreenSize().x / 2 + 100, lpSceneMng.GetScreenSize().y / 2-80, 0x151515, true);

	if (playertype_ == PlayerType::One)
	{
		//1Pが勝ったら
		DrawString(lpSceneMng.GetScreenSize().x / 2 - 50, 300, "1P WIN", 0xffff00);
	}
	else if (playertype_ == PlayerType::Two)
	{
		//2Pが勝ったら
		DrawString(lpSceneMng.GetScreenSize().x / 2 - 50, 300, "2P WIN", 0xff0000);
	}

	//再戦
	DrawFormatString(lpSceneMng.GetScreenSize().x / 2-120,
		lpSceneMng.GetScreenSize().y / 2, 0xffffff, "B:再戦する");

	//タイトルに戻る
	DrawFormatString(lpSceneMng.GetScreenSize().x / 2-120,
		lpSceneMng.GetScreenSize().y / 2+20,0xffffff,"A:ステージセレクトに戻る");
}

void ResultScene::Release(void)
{

}

UniqueScene ResultScene::UpdateScene(UniqueScene& scene)
{


	if (controller_->ChaeckInputKey(KeyID::ReturnSelect))
	{
		StopSoundMem(lpSoundMng.GetID("winSe"));

		ChangeVolumeSoundMem(150, lpSoundMng.GetID("bgm"));
		PlaySoundMem(lpSoundMng.GetID("bgm"), DX_PLAYTYPE_BACK);
		return std::make_unique<SelectScene>();
	}
	if (controller_->ChaeckInputKey(KeyID::Rematch))
	{
		StopSoundMem(lpSoundMng.GetID("winSe"));
		return std::make_unique<GameScene>();
	}

	//デバック用
#ifdef _DEBUG



#endif
	//何もないなら返す
	return std::move(scene);
}
