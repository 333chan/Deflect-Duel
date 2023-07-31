#include <DxLib.h>
#include"SceneManager.h"
#include"../common/SoundManager.h"
#include"../common/ImageManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "../input/PadInput2.h"
#include "VersusScene.h"

VersusScene::VersusScene()
{
	//ƒRƒ“ƒgƒ[ƒ‰[‚Ì¶¬
	if (GetJoypadNum() >= 1)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	//‰Šú‰»
	Init();

	//‚¿‚ç‚Â‚«–hŽ~
	DrawScreen();
}

VersusScene::~VersusScene()
{
	//‰ð•ú
	Release();
}

void VersusScene::Init(void)
{
	//tmx‚Ì“Ç‚Ýž‚Ý
	tmxObj_.LoadTmx("resource/tmx/VersusScene.tmx", false);

	//tmx‚ÌCollLisetŽæ“¾
	for (auto& coll : tmxObj_.GetTitleBgimageList())
	{
		bgPos_ = coll.first;
		bgEndPos_ = coll.first + coll.second;
	}
	for (auto& coll : tmxObj_.Getp1windowList())
	{
		p1windowPos_ = coll.first;
		p1windowEndPos_ = coll.first + coll.second;
	}	
	for (auto& coll : tmxObj_.Getp2windowList())
	{
		p2windowPos_ = coll.first;
		p2windowEndPos_ = coll.first + coll.second;
	}

}

UniqueScene VersusScene::Update(UniqueScene scene)
{
	return UniqueScene();

	DrawScreen();
	return UpdateScene(scene);
}

void VersusScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	DrawExtendGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, lpImageMng.GetID("vsBg")[0], true);
}

void VersusScene::Release(void)
{
}

UniqueScene VersusScene::UpdateScene(UniqueScene& scene)
{
	if (controller_->ChaeckInputKey(KeyID::Decide))
	{
		ChangeVolumeSoundMem(255, lpSoundMng.GetID("decideSe"));
		PlaySoundMem(lpSoundMng.GetID("decideSe"), DX_PLAYTYPE_BACK);

		ChangeVolumeSoundMem(150, lpSoundMng.GetID("bgm"));
		return std::make_unique<VersusScene>();
	}
}
