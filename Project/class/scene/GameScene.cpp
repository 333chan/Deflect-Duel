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
	player_ = std::make_unique<Player>(controller_->GetControllerType());
	stage_ = std::make_unique<Stage>();
}

UniqueScene GameScene::Update(UniqueScene scene)
{
	//�v���C���[
	player_->Update();

	//�X�e�[�W
	stage_->Update();

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

	DrawFormatString(0,0,0xffffff,"GameScene");
}

void GameScene::Release(void)
{

}

UniqueScene GameScene::UpdateScene(UniqueScene& scene)
{
	//�f�o�b�N�p
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_R))
	{
		return std::make_unique<ResultScene>();
	}

#endif

	//���̃V�[���ɖ߂�
	return std::move(scene);
}
