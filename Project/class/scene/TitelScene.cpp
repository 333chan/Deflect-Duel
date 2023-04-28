#include <DxLib.h>
#include"SceneManager.h"

#include "GameScene.h"
#include "TitelScene.h"

TitelScene::TitelScene()
{
	//������
	Init();

	//������h�~
	DrawScreen();
}

TitelScene::~TitelScene()
{
	//���
	Release();
}

void TitelScene::Init(void)
{

}

UniqueScene TitelScene::Update(UniqueScene scene)
{
	DrawScreen();
	return UpdateScene(scene);
}

void TitelScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	DrawFormatString(0, 0, 0xffffff, "TitleScene");
}

void TitelScene::Release(void)
{
}

UniqueScene TitelScene::UpdateScene(UniqueScene& scene)
{
	//�f�o�b�N�p
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_E))
	{
		return std::make_unique<GameScene>();
	}
#endif

	//���̃V�[���ɖ߂�
	return std::move(scene);
}
