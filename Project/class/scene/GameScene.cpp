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
	//パッドの数１つ以上の場合はパッド操作にする
	if (GetJoypadNum() > 0)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	//インスタンスの生成
	ball_ = std::make_shared<Ball>();	//ユニークだと所有権ごと渡してしまうため
	player_ = std::make_unique<Player>(controller_->GetControllerType(), ball_);
	stage_ = std::make_unique<Stage>();
	
}

UniqueScene GameScene::Update(UniqueScene scene)
{

	controller_->Update();

	//ステージ
	stage_->Update();

	//プレイヤー
	player_->Update();

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

	//ボール
	ball_->Draw();

	DrawFormatString(0,0,0xffffff,"GameScene");
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
		return std::make_unique<TitelScene>();
	}

#endif

	//元のシーンに戻す
	return std::move(scene);
}
