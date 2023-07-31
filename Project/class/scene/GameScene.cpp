#include <DxLib.h>
#include"SceneManager.h"
#include"../common/SoundManager.h"
#include"../common/ImageManager.h"
#include "TitelScene.h"
#include "SelectScene.h"
#include "ResultScene.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "../object/player/Player.h"
#include "../object/stage/Stage.h"
#include "../object/ball/Ball.h"
#include "GameScene.h"

GameScene::GameScene()
{
	//初期化
	Init();
}

GameScene::~GameScene()
{
	//解放
	Release();
}

void GameScene::Init(void)
{
	//パッドの数１つ以上の場合はパッド操作にする、現在は無効
	if (GetJoypadNum() > 0)
	{
		controller_ = std::make_unique<KeyInput>();

	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	//インスタンスの生成
	ball_ = std::make_shared<Ball>();	//ユニークだと所有権ごと渡してしまうため

	if (GetJoypadNum() >= 2)
	{
		player_ = std::make_unique<Player>(ControllerType::Pad1, PlayerType::One, ball_);
		player2_ = std::make_unique<Player>(ControllerType::Pad2, PlayerType::Two, ball_);
	}
	else if (GetJoypadNum() == 1)
	{
		player_ = std::make_unique<Player>(ControllerType::Pad1, PlayerType::One, ball_);
		player2_ = std::make_unique<Player>(ControllerType::Key, PlayerType::Two, ball_);
	}
	else
	{
		player_ = std::make_unique<Player>(ControllerType::Key, PlayerType::One, ball_);
		player2_ = std::make_unique<Player>(ControllerType::Key, PlayerType::Two, ball_);
	}

	stage_ = std::make_unique<Stage>();

	PlaySoundMem(lpSoundMng.GetID("gameBgm"), DX_PLAYTYPE_BACK);
	ChangeVolumeSoundMem(150, lpSoundMng.GetID("gameBgm"));
	
}

UniqueScene GameScene::Update(UniqueScene scene)
{
	//コントローラー
	controller_->Update();

	//1Pか2Pどちらか死んでいたら
	if (player_->GetState()== State::Death|| player2_->GetState() == State::Death)
	{
		auto pState = player_->GetState() == State::Death ? PlayerType ::Two : PlayerType::One;

		StopSoundMem(lpSoundMng.GetID("gameBgm"));
		return std::make_unique<ResultScene>(pState);
	}

	//ステージ
	stage_->Update();

	//プレイヤー

	player_->Update();
	player2_->Update();

	//ボール
	ball_->Update();

	DrawScreen();
	return UpdateScene(scene);
}

void GameScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	//ステージ
	stage_->Draw();

	//プレイヤー
	player_->Draw();
	player2_->Draw();

	//ボール
	ball_->Draw();

	//デバック用
#ifdef _DEBUG
	DrawFormatString(0,0,0xffffff,"GameScene");
#endif
}

void GameScene::Release(void)
{

}

UniqueScene GameScene::UpdateScene(UniqueScene& scene)
{
	//デバック用
#ifdef _DEBUG

	if (controller_->ChaeckInputKey(KeyID::Transition))
	{
		StopSoundMem(lpSoundMng.GetID("gameBgm"));
		auto pState = player_->GetState() == State::Death ? PlayerType::Two : PlayerType::One;
		return std::make_unique<ResultScene>(pState);
	}

#endif

	//元のシーンに戻す
	return std::move(scene);
}
