#include <DxLib.h>
#include"SceneManager.h"
#include"../common/SoundManager.h"
#include"../common/ImageManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "../input/PadInput2.h"
#include "SelectScene.h"
#include "TitelScene.h"


constexpr float BG_MOVE_SPEED = 1;

TitelScene::TitelScene()
{

	//�R���g���[���[�̐���
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
	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/titleScene.tmx", false);

	auto screen = lpSceneMng.GetScreenSize();

	//���d�X�N���[��
	//�ŉ�
	bgVec_.emplace_back(
		BG(lpImageMng.GetID("backBills")[0],
			{ { 0,0 } ,{screen} },
			{ { screen.x,0 } ,{screen.x * 2,screen.y} },
			BG_MOVE_SPEED*0.3
	));
	//����
	bgVec_.emplace_back(
		BG(lpImageMng.GetID("middleBills")[0],
			{ { 0,0 } ,{screen} },
			{ { screen.x,0 } ,{screen.x * 2,screen.y} },
			BG_MOVE_SPEED
		));
	//��O
	bgVec_.emplace_back(
		BG(lpImageMng.GetID("road")[0],
			{ { 0,0 } ,{screen} },
			{ { screen.x,0 } ,{screen.x * 2,screen.y} },
			BG_MOVE_SPEED*3.5
		));

	//tmx��CollLiset�擾
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
	DrawFormatString(0, 0, 0xffffff, "TitleScene");
#endif

}

UniqueScene TitelScene::Update(UniqueScene scene)
{
	controller_->Update();

	//���d�X�N���[��
	for (auto& bg:bgVec_)
	{
		auto& [sPos, ePos] = bg.pos1;
		auto& [sPos2, ePos2] = bg.pos2;

		//�ꖇ��
		sPos.x -= bg.speed;
		ePos.x -= bg.speed;

		//�񖇖�
		sPos2.x -= bg.speed;
		ePos2.x -= bg.speed;

		
		if (0 > ePos.x)
		{
			//�ꖇ�ډ�ʊO
			sPos.x = lpSceneMng.GetScreenSize().x - bg.speed;
			ePos.x = lpSceneMng.GetScreenSize().x * 2 - bg.speed;
		}

		if (0 > ePos2.x)
		{
			//�񖇖ډ�ʊO
			sPos2.x = lpSceneMng.GetScreenSize().x - bg.speed;
			ePos2.x = lpSceneMng.GetScreenSize().x * 2 - bg.speed;
		}
	}

	DrawScreen();
	return UpdateScene(scene);
}

void TitelScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	DrawExtendGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, lpImageMng.GetID("titleBg")[0], true);

	for (const auto& bg : bgVec_)
	{
		const auto& [sPos, ePos] = bg.pos1;
		const auto& [sPos2, ePos2] = bg.pos2;

		DrawExtendGraphF(sPos.x, sPos.y, ePos.x, ePos.y, bg.hadle, true);	//�ꖇ��
		DrawExtendGraphF(sPos2.x, sPos2.y, ePos2.x, ePos2.y, bg.hadle, true);	//�񖇖�
	}

	DrawGraph(logoPos_.x, logoPos_.y, lpImageMng.GetID("titlelogo")[0], true);
	DrawString(550,600 - 16, "PRESS Player1 B BUTTON", 0xffffff);

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "TitleScene");
#endif

}

void TitelScene::Release(void)
{
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
	//�f�o�b�N�p
#ifdef _DEBUG



#endif

	//���̃V�[���ɖ߂�
	return std::move(scene);
}
