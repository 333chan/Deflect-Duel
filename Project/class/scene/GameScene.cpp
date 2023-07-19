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
	//������
	Init();
}

GameScene::~GameScene()
{
	//���
	Release();
}

void GameScene::Init(void)
{
	//�p�b�h�̐��P�ȏ�̏ꍇ�̓p�b�h����ɂ���A���݂͖���
	if (GetJoypadNum() > 0)
	{
		controller_ = std::make_unique<KeyInput>();

	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	//�C���X�^���X�̐���
	ball_ = std::make_shared<Ball>();	//���j�[�N���Ə��L�����Ɠn���Ă��܂�����

	if (GetJoypadNum() >= 2)
	{
		player_ = std::make_unique<Player>(ControllerType::Pad1, playerType::One, ball_);
		player2_ = std::make_unique<Player>(ControllerType::Pad2, playerType::Two, ball_);
	}
	else if (GetJoypadNum() == 1)
	{
		player_ = std::make_unique<Player>(ControllerType::Key, playerType::One, ball_);
		player2_ = std::make_unique<Player>(ControllerType::Pad1, playerType::Two, ball_);
	}
	else
	{
		player_ = std::make_unique<Player>(ControllerType::Key, playerType::One, ball_);
		player2_ = std::make_unique<Player>(ControllerType::Key, playerType::Two, ball_);
	}

	stage_ = std::make_unique<Stage>();

	//�T�E���h�̃��[�h
	gameBgm_ = LoadSoundMem("resource/sound/gamebgm.mp3");

	PlaySoundMem(gameBgm_, DX_PLAYTYPE_BACK);
	ChangeVolumeSoundMem(150, gameBgm_);
	
}

UniqueScene GameScene::Update(UniqueScene scene)
{
	//�R���g���[���[
	controller_->Update();

	//1P��2P�ǂ��炩����ł�����
	if (player_->GetState()== State::Death|| player2_->GetState() == State::Death)
	{
		auto pState = player_->GetState() == State::Death ? playerType ::Two : playerType::One;

		StopSoundMem(gameBgm_);
		return std::make_unique<ResultScene>(pState);
	}

	//�X�e�[�W
	stage_->Update();

	//�v���C���[

	player_->Update();
	player2_->Update();

	//�{�[��
	ball_->Update();

	DrawScreen();
	return UpdateScene(scene);
}

void GameScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	//�X�e�[�W
	stage_->Draw();

	//�v���C���[
	player_->Draw();
	player2_->Draw();

	//�{�[��
	ball_->Draw();

	//�f�o�b�N�p
#ifdef _DEBUG
	DrawFormatString(0,0,0xffffff,"GameScene");
#endif
}

void GameScene::Release(void)
{
	DeleteSoundMem(gameBgm_);
}

UniqueScene GameScene::UpdateScene(UniqueScene& scene)
{
	//�f�o�b�N�p
#ifdef _DEBUG

	if (controller_->ChaeckInputKey(KeyID::Transition))
	{
		auto pState = player_->GetState() == State::Death ? playerType::Two : playerType::One;
		return std::make_unique<ResultScene>(pState);
	}

#endif

	//���̃V�[���ɖ߂�
	return std::move(scene);
}
