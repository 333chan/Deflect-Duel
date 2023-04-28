#include <DxLib.h>
#include"SceneManager.h"
#include "TitelScene.h"
#include "ResultScene.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "../object/player/Player.h"
#include "../object/stage/Stage.h"
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
	player_ = std::make_unique<Player>(controller_->GetControllerType());
	stage_ = std::make_unique<Stage>();
}

UniqueScene GameScene::Update(UniqueScene scene)
{
	//プレイヤー
	player_->Update();

	//ステージ
	stage_->Update();

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

	DrawFormatString(0,0,0xffffff,"GameScene");
}

void GameScene::Release(void)
{

}

UniqueScene GameScene::UpdateScene(UniqueScene& scene)
{
	//デバック用
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_R))
	{
		return std::make_unique<ResultScene>();
	}

#endif

	//元のシーンに戻す
	return std::move(scene);
}
