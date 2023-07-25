#include <DxLib.h>
#include "TitelScene.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "SceneManager.h"
#include "../../_debug/_DebugDispOut.h"
#include "../../resource/image/icon/icon.h"



//��ʃT�C�Y(�t���X�N���[���\��)
constexpr int SCREEN_SIZE_X =1280;
constexpr int SCREEN_SIZE_Y = 720;

bool SceneManager::SystemInit(void)
{
	SetOutApplicationLogValidFlag(false);
	SetGraphMode(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 32);

	SetWindowIconID(ICON_1);

	SetWindowText("Deflect Duel");

	//�E�B���h�E
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	if (GetJoypadNum() == 0)
	{
		if (MessageBox(NULL, "�Q�[���p�b�h��ڑ����ăQ�[�����ċN�����Ă�������", "�Q�[���p�b�h���ڑ�����Ă��܂���", MB_OK))
		{
			return false;
		}
	}

	//�m�F���b�Z�[�W
	if (MessageBox(NULL, "�t���X�N���[���ŋN�����܂����H", "��ʃT�C�Y�ݒ�", MB_YESNO) == IDYES)
	{
		//�t���X�N���[��
		ChangeWindowMode(false);
	}
	else
	{
		//�E�B���h�E
		ChangeWindowMode(true);
	}


	_dbgSetup(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 255);

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
	//�N�����V�[���ݒ�
	scene_ = std::make_unique<TitelScene>();

	//�f���^�^�C���n�̏�����
	deltaTime_ = 0.0f;
	tickCount_ = std::chrono::system_clock::now();
}

void SceneManager::Run(void)
{
	if (!SystemInit())
	{
		return;
	}

	Init();

	while (ProcessMessage() == 0 ) {
		
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			//�m�F���b�Z�[�W
			if (MessageBox(NULL, "�Q�[�����I�����܂����H", "�I���m�F", MB_YESNO) == IDYES)
			{
				// DXLIB�I��
				DxLib_End();
			}
			else
			{
				continue;
			}
		}

		//�f�o�b�N�J�n
		_dbgStartDraw();

		// �X�V
		Update();

		//�`��
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

	//�f���^�^�C���̍쐬
	auto tick = std::chrono::system_clock::now();
	deltaTime_ = std::chrono::duration_cast<std::chrono::microseconds>(tick - tickCount_).count() / 1000000.0f;
	tickCount_ = tick;
}

double SceneManager::GetDeltaTime(void)
{
	return deltaTime_;
}

void SceneManager::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	scene_->Draw();
	_dbgDraw();
}

void SceneManager::Relese(void)
{
}

Vector2 SceneManager::GetScreenSize(void)
{
	return Vector2(SCREEN_SIZE_X,SCREEN_SIZE_Y);
}

