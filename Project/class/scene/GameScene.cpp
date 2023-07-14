#include <DxLib.h>
#include"SceneManager.h"
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
	player_ = std::make_unique<Player>(ControllerType::Key , playerType::One, ball_);
	player2_ = std::make_unique<Player>(ControllerType::Pad, playerType::Two, ball_);
	stage_ = std::make_unique<Stage>();

	gamebgm_ = LoadSoundMem("resource/sound/gamebgm.mp3");
	PlaySoundMem(gamebgm_, DX_PLAYTYPE_BACK);
	ChangeVolumeSoundMem(150, gamebgm_);
	
}

UniqueScene GameScene::Update(UniqueScene scene)
{

	controller_->Update();

	if (player_->GetState()== State::Death|| player2_->GetState() == State::Death)
	{
		auto t = player_->GetState() == State::Death ? playerType ::Two : playerType::One;


		StopSoundMem(gamebgm_);
		return std::make_unique<ResultScene>(t);
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
		auto t = player_->GetState() == State::Death ? playerType::Two : playerType::One;
		return std::make_unique<ResultScene>(t);
	}

#endif

	//元のシーンに戻す
	return std::move(scene);
}
