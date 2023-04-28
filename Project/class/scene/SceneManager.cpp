#include <DxLib.h>
#include "TitelScene.h"
#include "GameScene.h"
#include "SceneManager.h"



//��ʃT�C�Y(�t���X�N���[���\��)
constexpr int SCREENSIZE_X = 1500;
constexpr int SCREENSIZE_Y = 780;

bool SceneManager::SystemInit(void)
{
	SetOutApplicationLogValidFlag(false);
	SetGraphMode(static_cast<int>(SCREENSIZE_X), static_cast<int>(SCREENSIZE_Y), 32);
	ChangeWindowMode(true);
	// SetWindowIconID(ICON_1); // �A�C�R���ύX
	SetWindowText("2016019_�c����");

	if (DxLib_Init() == -1)
	{
		return false;
	}
	return true;
}

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Init(void)
{
	scene_ = std::make_unique<GameScene>();
}

void SceneManager::Run(void)
{
	if (!SystemInit())
	{
		return;
	}

	Init();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		
		
		// �X�V
		Update();
		Draw();

		ScreenFlip();
	}
	// DXLIB�I��
	DxLib_End();

}

void SceneManager::Update()
{
	//�X�V
	scene_ = scene_->Update(std::move(scene_));
}

void SceneManager::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	scene_->Draw();
}

void SceneManager::Relese(void)
{
}

