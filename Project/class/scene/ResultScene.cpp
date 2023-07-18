#include <DxLib.h>
#include "SceneManager.h"
#include "../object/player/Player.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "TitelScene.h"
#include "GameScene.h"
#include "ResultScene.h"

ResultScene::ResultScene(playerType ptype)
{
	playertype_ = ptype;

	//�p�b�h�̐��P�ȏ�̏ꍇ�̓p�b�h����ɂ���A���݂͖���
	if (GetJoypadNum() > 0)
	{
		controller_ = std::make_unique<KeyInput>();

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

ResultScene::~ResultScene()
{
	//���
	Release();
}

void ResultScene::Init(void)
{
	//���݂̉�ʂ��摜�Ŏ擾
	screenImage_ = MakeScreen(screenSizeX_, screenSizeY_, true);

	//�擾�����摜��`��
	GetDrawScreenGraph(0, 0, IpSceneMng.GetScreenSize().x, IpSceneMng.GetScreenSize().y, screenImage_, true);

	//�T�E���h�̓ǂݍ���
	winSe_ = LoadSoundMem("resource/sound/win.mp3");

	ChangeVolumeSoundMem(220, winSe_);
	PlaySoundMem(winSe_, DX_PLAYTYPE_BACK);
}

UniqueScene ResultScene::Update(UniqueScene scene)
{
	controller_->Update();
	DrawScreen();
	return UpdateScene(scene);
}

void ResultScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0, screenImage_,true);


	DrawBox(IpSceneMng.GetScreenSize().x / 2 - 151, IpSceneMng.GetScreenSize().y / 2 + 51, IpSceneMng.GetScreenSize().x / 2 + 101, IpSceneMng.GetScreenSize().y / 2 - 81, 0xffffff, true);
	DrawBox(IpSceneMng.GetScreenSize().x / 2-150, IpSceneMng.GetScreenSize().y / 2+50, IpSceneMng.GetScreenSize().x / 2 + 100, IpSceneMng.GetScreenSize().y / 2-80, 0x151515, true);

	if (playertype_ == playerType::One)
	{
		//1P����������
		DrawString(IpSceneMng.GetScreenSize().x / 2 - 50, 300, "1P����", 0xffff00);
	}
	else if (playertype_ == playerType::Two)
	{
		//2P����������
		DrawString(IpSceneMng.GetScreenSize().x / 2 - 50, 300, "2P����", 0xff0000);
	}

	//�Đ�
	DrawFormatString(IpSceneMng.GetScreenSize().x / 2-120,
		IpSceneMng.GetScreenSize().y / 2, 0xffffff, "Press R key to Rematch");

	//�^�C�g���ɖ߂�
	DrawFormatString(IpSceneMng.GetScreenSize().x / 2-120,
		IpSceneMng.GetScreenSize().y / 2+20,0xffffff,"Press T key for Title");
}

void ResultScene::Release(void)
{

	DeleteSoundMem(winSe_);

}

UniqueScene ResultScene::UpdateScene(UniqueScene& scene)
{


	if (controller_->ChaeckInputKey(KeyID::ReturnTitle))
	{
		StopSoundMem(winSe_);
		return std::make_unique<TitelScene>();
	}
	if (controller_->ChaeckInputKey(KeyID::Rematch))
	{
		StopSoundMem(winSe_);
		return std::make_unique<GameScene>();
	}

	//�f�o�b�N�p
#ifdef _DEBUG



#endif
	//�����Ȃ��Ȃ�Ԃ�
	return std::move(scene);
}
