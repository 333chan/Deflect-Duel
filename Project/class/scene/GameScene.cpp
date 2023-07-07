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
	//�p�b�h�̐��P�ȏ�̏ꍇ�̓p�b�h����ɂ���
	if (GetJoypadNum() > 0)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	//�C���X�^���X�̐���
	ball_ = std::make_shared<Ball>();	//���j�[�N���Ə��L�����Ɠn���Ă��܂�����
	player_ = std::make_unique<Player>(controller_->GetControllerType(), ball_);
	stage_ = std::make_unique<Stage>();
	
}

UniqueScene GameScene::Update(UniqueScene scene)
{

	controller_->Update();

	//�X�e�[�W
	stage_->Update();

	//�v���C���[
	player_->Update();

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

	//�{�[��
	ball_->Draw();

	DrawFormatString(0,0,0xffffff,"GameScene");
}

void GameScene::Release(void)
{

}

UniqueScene GameScene::UpdateScene(UniqueScene& scene)
{
	//�f�o�b�N�p
#ifdef _DEBUG
	if (controller_->ChaeckInputKey(KeyID::Transition))
	{
		return std::make_unique<TitelScene>();
	}

#endif

	//���̃V�[���ɖ߂�
	return std::move(scene);
}
