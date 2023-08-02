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

	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/gameScene.tmx", false);

	//tmxのCollLiset取得
	for (auto& coll : tmxObj_.Getp1HowtoList())
	{
		p1windowPos_ = coll.first;
		p1windowEndPos_ = coll.first + coll.second;
	}
	for (auto& coll : tmxObj_.Getp2HowtoList())
	{
		p2windowPos_ = coll.first;
		p2windowEndPos_ = coll.first + coll.second;
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

	PlaySoundMem(lpSoundMng.GetID("gameBgm"), DX_PLAYTYPE_LOOP);
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

	DrawExtendGraph(p1windowPos_.x, p1windowPos_.y, p1windowEndPos_.x, p1windowEndPos_.y, lpImageMng.GetID("window")[0],true);
	DrawExtendGraph(p2windowPos_.x, p2windowPos_.y, p2windowEndPos_.x, p2windowEndPos_.y, lpImageMng.GetID("window")[0],true);

	DrawString(50, 640, "PLAYER1 操作\n右/左で左右移動\nBでジャンプ\nXで攻撃", 0xff0000, true);
	DrawString(1100, 640, "   PLAYER 2操作\n 右/左で左右移動\n    Bでジャンプ\n        Xで攻撃", 0x0000ff, true);

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
