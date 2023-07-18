#include <DxLib.h>
#include"SceneManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "GameScene.h"
#include "TitelScene.h"
#include "SelectScene.h"

SelectScene::SelectScene(int bgm)
{

	//�R���g���[���[�̐���
	if (GetJoypadNum() >= 1)
	{
		controller_ = std::make_unique<KeyInput>();

	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	//������
	Init();

	selectBgm_ = bgm;

	//������h�~
	DrawScreen();
}

SelectScene::~SelectScene()
{
	Release();
}

void SelectScene::Init(void)
{
	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/selectScene.tmx", false);

	//�摜�ǂݍ���
	bgImageH_ = LoadGraph("resource/image/stage/selectBg.png", true);
	logoImageH_ = LoadGraph("resource/image/stage/stageselect.png", true);
	stage1ImageH_ = LoadGraph("resource/image/stage/selectStage.png",true);
	stageNullImageH_ = LoadGraph("resource/image/stage/selectStageNull.png", true);

	//�T�E���h�ǂݍ���
	decideSe_ = LoadSoundMem("resource/sound/dis.mp3");

}

UniqueScene SelectScene::Update(UniqueScene scene)
{
	controller_->Update();
	DrawScreen();

	return UpdateScene(scene);
}

void SelectScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	DrawExtendGraph(0, 0, IpSceneMng.GetScreenSize().x,IpSceneMng.GetScreenSize().y, bgImageH_, true);

	//tmx����ʒu�ƃT�C�Y�擾
	for (const auto& imagePos : tmxObj_.GetSelectStageList())
	{
		const auto& [sPos, size] = imagePos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, stageNullImageH_, true);
	}

	//tmx����ʒu�ƃT�C�Y�擾
	for (const auto& imagePos : tmxObj_.GetSelectStageList())
	{
		//�n�_�ƃT�C�Y��
		const auto& [sPos, size] = imagePos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, stage1ImageH_, true);
		break;
	}

	//tmx�̈ʒu�ƃT�C�Y�擾
	for (const auto& bgPos : tmxObj_.GetSelectLogoList())
	{
		//�n�_�ƃT�C�Y��
		const auto& [sPos, size] = bgPos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, logoImageH_, true);
	}

	//�X�e�[�W�̖��O
	DrawString(100, IpSceneMng.GetScreenSize().y - 100, "�q��/Press 1",0xffffff);
	DrawString(380, IpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);
	DrawString(700, IpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);
	DrawString(1000, IpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "Select");
#endif


}

void SelectScene::Release(void)
{
	DeleteGraph(stage1ImageH_);
	DeleteGraph(logoImageH_);

	DeleteSoundMem(decideSe_);
	DeleteSoundMem(selectBgm_);

}

UniqueScene SelectScene::UpdateScene(UniqueScene& scene)
{

	if (controller_->ChaeckInputKey(KeyID::Stage1))
	{
		ChangeVolumeSoundMem(255, decideSe_);
		PlaySoundMem(decideSe_, DX_PLAYTYPE_BACK);

		StopSoundMem(selectBgm_);
		return std::make_unique<GameScene>();
	}

	//�f�o�b�N�p
#ifdef _DEBUG



#endif

	//���̃V�[���ɖ߂�
	return std::move(scene);
}
