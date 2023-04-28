#include <DxLib.h>
#include "SceneManager.h"
#include "TitelScene.h"
#include "GameScene.h"
#include "ResultScene.h"

ResultScene::ResultScene()
{
	//初期化
	Init();
}

ResultScene::~ResultScene()
{
	//解放
	Release();
}

void ResultScene::Init(void)
{

}

UniqueScene ResultScene::Update(UniqueScene scene)
{
	DrawScreen();
	return UpdateScene(scene);
}

void ResultScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawFormatString(0,0,0xffffff,"ResultScene");
}

void ResultScene::Release(void)
{
}

UniqueScene ResultScene::UpdateScene(UniqueScene& scene)
{
	//デバック用
#ifdef _DEBUG

	if (CheckHitKey(KEY_INPUT_T))
	{
		return std::make_unique<TitelScene>();
	}

#endif
	//何もないなら返す
	return std::move(scene);
}
