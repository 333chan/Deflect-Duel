#include <DxLib.h>
#include "TitelScene.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "SceneManager.h"
#include "../../_debug/_DebugDispOut.h"
#include "../../resource/image/icon/icon.h"



//画面サイズ(フルスクリーン予定)
constexpr int SCREEN_SIZE_X =1280;
constexpr int SCREEN_SIZE_Y = 720;

bool SceneManager::SystemInit(void)
{
	SetOutApplicationLogValidFlag(false);
	SetGraphMode(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 32);

	SetWindowIconID(ICON_1);

	SetWindowText("Deflect Duel");

	//ウィンドウ
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	if (GetJoypadNum() == 0)
	{
		if (MessageBox(NULL, "ゲームパッドを接続してゲームを再起動してください", "ゲームパッドが接続されていません", MB_OK))
		{
			return false;
		}
	}

	//確認メッセージ
	if (MessageBox(NULL, "フルスクリーンで起動しますか？", "画面サイズ設定", MB_YESNO) == IDYES)
	{
		//フルスクリーン
		ChangeWindowMode(false);
	}
	else
	{
		//ウィンドウ
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
	//起動時シーン設定
	scene_ = std::make_unique<TitelScene>();

	//デルタタイム系の初期化
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
			//確認メッセージ
			if (MessageBox(NULL, "ゲームを終了しますか？", "終了確認", MB_YESNO) == IDYES)
			{
				// DXLIB終了
				DxLib_End();
			}
			else
			{
				continue;
			}
		}

		//デバック開始
		_dbgStartDraw();

		// 更新
		Update();

		//描画
		Draw();

		ScreenFlip();
	}
	// DXLIB終了
	DxLib_End();

}

void SceneManager::Update()
{
	//更新
	scene_ = scene_->Update(std::move(scene_));

	//デルタタイムの作成
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

