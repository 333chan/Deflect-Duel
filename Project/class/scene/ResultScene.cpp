#include <DxLib.h>
#include "SceneManager.h"
#include "../object/player/Player.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "TitelScene.h"
#include "GameScene.h"
#include "ResultScene.h"

ResultScene::ResultScene(playerType ptype)
{
	playertype_ = ptype;

	//パッドの数１つ以上の場合はパッド操作にする、現在は無効
	if (GetJoypadNum() > 0)
	{
		controller_ = std::make_unique<KeyInput>();

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
	GetDrawScreenGraph(0, 0, IpSceneMng.GetScreenSize().x, IpSceneMng.GetScreenSize().y, screenImage_, true);

	//サウンドの読み込み
	winSe_ = LoadSoundMem("resource/sound/win.mp3");

	ChangeVolumeSoundMem(220, winSe_);
	PlaySoundMem(winSe_, DX_PLAYTYPE_BACK);
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


	DrawBox(IpSceneMng.GetScreenSize().x / 2 - 151, IpSceneMng.GetScreenSize().y / 2 + 51, IpSceneMng.GetScreenSize().x / 2 + 101, IpSceneMng.GetScreenSize().y / 2 - 81, 0xffffff, true);
	DrawBox(IpSceneMng.GetScreenSize().x / 2-150, IpSceneMng.GetScreenSize().y / 2+50, IpSceneMng.GetScreenSize().x / 2 + 100, IpSceneMng.GetScreenSize().y / 2-80, 0x151515, true);

	if (playertype_ == playerType::One)
	{
		//1Pが勝ったら
		DrawString(IpSceneMng.GetScreenSize().x / 2 - 50, 300, "1P勝ち", 0xffff00);
	}
	else if (playertype_ == playerType::Two)
	{
		//2Pが勝ったら
		DrawString(IpSceneMng.GetScreenSize().x / 2 - 50, 300, "2P勝ち", 0xff0000);
	}

	//再戦
	DrawFormatString(IpSceneMng.GetScreenSize().x / 2-120,
		IpSceneMng.GetScreenSize().y / 2, 0xffffff, "Press R key to Rematch");

	//タイトルに戻る
	DrawFormatString(IpSceneMng.GetScreenSize().x / 2-120,
		IpSceneMng.GetScreenSize().y / 2+20,0xffffff,"Press T key for Title");
}

void ResultScene::Release(void)
{

	DeleteSoundMem(winSe_);

}

UniqueScene ResultScene::UpdateScene(UniqueScene& scene)
{


	if (controller_->ChaeckInputKey(KeyID::ReturnTitle))
	{
		StopSoundMem(winSe_);
		return std::make_unique<TitelScene>();
	}
	if (controller_->ChaeckInputKey(KeyID::Rematch))
	{
		StopSoundMem(winSe_);
		return std::make_unique<GameScene>();
	}

	//デバック用
#ifdef _DEBUG



#endif
	//何もないなら返す
	return std::move(scene);
}
