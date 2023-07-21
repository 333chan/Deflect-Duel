#include <DxLib.h>
#include"SceneManager.h"
#include"../common/SoundManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "../input/PadInput2.h"
#include "SelectScene.h"
#include "TitelScene.h"


constexpr float BG_MOVE_SPEED = 1;

TitelScene::TitelScene()
{

	//コントローラーの生成
	if (GetJoypadNum() >= 1)
	{
		controllerFlg = true;
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controllerFlg = false;
		controller_ = std::make_unique<KeyInput>();
	}

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
	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/titleScene.tmx", false);

	//画像読み込み
	bgImageH_ = LoadGraph("resource/image/stage/titleBg.png", true);
	logoImageH_ = LoadGraph("resource/image/titlelogo.png");

	auto screen = IpSceneMng.GetScreenSize();

	//多重スクロール
	//最奥
	bgVec_.emplace_back(
		BG(LoadGraph("resource/image/stage/backBills.png", true),
			{ { 0,0 } ,{screen} },
			{ { screen.x,0 } ,{screen.x * 2,screen.y} },
			BG_MOVE_SPEED*0.3
	));
	//中間
	bgVec_.emplace_back(
		BG(LoadGraph("resource/image/stage/middleBills.png", true),
			{ { 0,0 } ,{screen} },
			{ { screen.x,0 } ,{screen.x * 2,screen.y} },
			BG_MOVE_SPEED
		));
	//手前
	bgVec_.emplace_back(
		BG(LoadGraph("resource/image/stage/road.png", true),
			{ { 0,0 } ,{screen} },
			{ { screen.x,0 } ,{screen.x * 2,screen.y} },
			BG_MOVE_SPEED*3.5
		));

	//tmxのCollLiset取得
	for (auto& coll : tmxObj_.GetTitleBgimageList())
	{
		bgPos_ = coll.first;
		bgEndPos_ = coll.first + coll.second;
	}
	for (auto& coll : tmxObj_.GetTitleLogoimageList())
	{
		logoPos_ = coll.first;
		logoPosEnd_ = coll.first + coll.second;
	}



	ChangeVolumeSoundMem(200, lpSoundMng.GetID("bgm"));
	PlaySoundMem(lpSoundMng.GetID("bgm"), DX_PLAYTYPE_LOOP);



#ifdef _DEBUG
	//bgImage_ = LoadGraph("resource/image/stage/titleBgFull.png", true);
	DrawFormatString(0, 0, 0xffffff, "TitleScene");
#endif

}

UniqueScene TitelScene::Update(UniqueScene scene)
{
	controller_->Update();

	//多重スクロール
	for (auto& bg:bgVec_)
	{
		auto& [sPos, ePos] = bg.pos1;
		auto& [sPos2, ePos2] = bg.pos2;

		//一枚目
		sPos.x -= bg.speed;
		ePos.x -= bg.speed;

		//二枚目
		sPos2.x -= bg.speed;
		ePos2.x -= bg.speed;

		
		if (0 > ePos.x)
		{
			//一枚目画面外
			sPos.x = IpSceneMng.GetScreenSize().x - bg.speed;
			ePos.x = IpSceneMng.GetScreenSize().x * 2 - bg.speed;
		}

		if (0 > ePos2.x)
		{
			//二枚目画面外
			sPos2.x = IpSceneMng.GetScreenSize().x - bg.speed;
			ePos2.x = IpSceneMng.GetScreenSize().x * 2 - bg.speed;
		}
	}

	DrawScreen();
	return UpdateScene(scene);
}

void TitelScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	DrawExtendGraph(0, 0, IpSceneMng.GetScreenSize().x, IpSceneMng.GetScreenSize().y, bgImageH_, true);

	for (const auto& bg : bgVec_)
	{
		const auto& [sPos, ePos] = bg.pos1;
		const auto& [sPos2, ePos2] = bg.pos2;

		DrawExtendGraphF(sPos.x, sPos.y, ePos.x, ePos.y, bg.hadle, true);	//一枚目
		DrawExtendGraphF(sPos2.x, sPos2.y, ePos2.x, ePos2.y, bg.hadle, true);	//二枚目
	}

	if (controllerFlg = true)
	{

	}


	DrawGraph(logoPos_.x, logoPos_.y, logoImageH_,true);
	DrawString(550,600 - 16, "PRESS B BUTTON", 0xffffff);

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "TitleScene");
#endif

}

void TitelScene::Release(void)
{

	for (auto& bg : bgVec_)
	{
		DeleteGraph(bg.hadle);
	}

	DeleteGraph(logoImageH_);

}

UniqueScene TitelScene::UpdateScene(UniqueScene& scene)
{

	if (controller_->ChaeckInputKey(KeyID::Decide))
	{
		ChangeVolumeSoundMem(255, lpSoundMng.GetID("decideSe"));
		PlaySoundMem(lpSoundMng.GetID("decideSe"), DX_PLAYTYPE_BACK);

		ChangeVolumeSoundMem(150, lpSoundMng.GetID("bgm"));
		return std::make_unique<SelectScene>();
	}

	if (CheckHitKeyAll(DX_CHECKINPUT_PAD))
	{

	}
	//デバック用
#ifdef _DEBUG

#endif

	//元のシーンに戻す
	return std::move(scene);
}
