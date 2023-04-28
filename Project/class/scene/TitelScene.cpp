#include <DxLib.h>
#include"SceneManager.h"

#include "GameScene.h"
#include "TitelScene.h"

TitelScene::TitelScene()
{
	//初期化
	Init();

	//ちらつき防止
	DrawScreen();
}

TitelScene::~TitelScene()
{
	//解放
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
	//デバック用
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_E))
	{
		return std::make_unique<GameScene>();
	}
#endif

	//元のシーンに戻す
	return std::move(scene);
}
