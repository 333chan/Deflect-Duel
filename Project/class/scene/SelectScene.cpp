#include <DxLib.h>
#include"SceneManager.h"
#include"../common/SoundManager.h"
#include"../common/ImageManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "TitelScene.h"
#include "SelectScene.h"
#include "VersusScene.h"

SelectScene::SelectScene()
{

	//�R���g���[���[�̐���
	if (GetJoypadNum() >= 1)
	{
		controller_ = std::make_unique<PadInput>();

	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	//������
	Init();

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

	DrawExtendGraph(0, 0, lpSceneMng.GetScreenSize().x,lpSceneMng.GetScreenSize().y, lpImageMng.GetID("selectBg")[0], true);


	//tmx����ʒu�ƃT�C�Y�擾
	for (const auto& imagePos : tmxObj_.GetSelectStageList())
	{
		const auto& [sPos, size] = imagePos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, lpImageMng.GetID("selectStageNull")[0], true);
	}

	//tmx����ʒu�ƃT�C�Y�擾
	for (const auto& imagePos : tmxObj_.GetSelectStageList())
	{
		//�n�_�ƃT�C�Y��
		const auto& [sPos, size] = imagePos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, lpImageMng.GetID("selectStage")[0], true);
		break;
	}

	//tmx�̈ʒu�ƃT�C�Y�擾
	for (const auto& bgPos : tmxObj_.GetSelectLogoList())
	{
		//�n�_�ƃT�C�Y��
		const auto& [sPos, size] = bgPos;
		DrawExtendGraph(sPos.x, sPos.y, sPos.x + size.x, sPos.y + size.y, lpImageMng.GetID("selectLogo")[0], true);
	}

	//�X�e�[�W�̖��O
	if (GetJoypadNum() >= 1)
	{
		DrawString(100, lpSceneMng.GetScreenSize().y - 100, "�q��/Push LB", 0xffffff);
	}
	else
	{
		DrawString(100, lpSceneMng.GetScreenSize().y - 100, "�q��/Press 1", 0xffffff);
	}

	DrawString(380, lpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);
	DrawString(700, lpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);
	DrawString(1000, lpSceneMng.GetScreenSize().y - 100, "Coming soon...",0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, lpSceneMng.GetScreenSize().y, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y-30, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	DrawString(10, lpSceneMng.GetScreenSize().y - 20, "A:�^�C�g���ɖ߂�", 0xffffff, true);


#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "Select");
#endif


}

void SelectScene::Release(void)
{
}

UniqueScene SelectScene::UpdateScene(UniqueScene& scene)
{

	if (controller_->ChaeckInputKey(KeyID::Stage1))
	{
		ChangeVolumeSoundMem(255, lpSoundMng.GetID("decideSe"));
		PlaySoundMem(lpSoundMng.GetID("decideSe"), DX_PLAYTYPE_BACK);

		return std::make_unique<VersusScene>();
	}
	if (controller_->ChaeckInputKey(KeyID::Cancel))
	{
		ChangeVolumeSoundMem(255, lpSoundMng.GetID("decideSe"));
		PlaySoundMem(lpSoundMng.GetID("decideSe"), DX_PLAYTYPE_BACK);

		StopSoundMem(lpSoundMng.GetID("bgm"));
		return std::make_unique<TitelScene>();
	}

	//�f�o�b�N�p
#ifdef _DEBUG



#endif

	//���̃V�[���ɖ߂�
	return std::move(scene);
}
