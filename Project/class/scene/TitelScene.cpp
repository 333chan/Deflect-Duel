#include <DxLib.h>
#include"SceneManager.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"
#include "SelectScene.h"
#include "TitelScene.h"


constexpr float BG_MOVE_SPEED = 1;

TitelScene::TitelScene()
{

	//�R���g���[���[�̐���
	if (GetJoypadNum() >= 1)
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

TitelScene::~TitelScene()
{
	//���
	Release();
}

void TitelScene::Init(void)
{
	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/titleScene.tmx", false);

	//�摜�ǂݍ���
	bgImageH_ = LoadGraph("resource/image/stage/titleBg.png", true);
	logoImageH_ = LoadGraph("resource/image/titlelogo.png");

	//�T�E���h�ǂݍ���
	bgm_ = LoadSoundMem("resource/sound/titlebgm.mp3");
	decideSe_ = LoadSoundMem("resource/sound/dis.mp3");

	auto screen = IpSceneMng.GetScreenSize();

	//���d�X�N���[��
	bgVec_.emplace_back(
		BG(LoadGraph("resource/image/stage/backBills.png", true),
			{ { 0,0 } ,{screen} },
			{ { screen.x,0 } ,{screen.x * 2,screen.y} },
			BG_MOVE_SPEED*0.3
	));

	bgVec_.emplace_back(
		BG(LoadGraph("resource/image/stage/middleBills.png", true),
			{ { 0,0 } ,{screen} },
			{ { screen.x,0 } ,{screen.x * 2,screen.y} },
			BG_MOVE_SPEED
		));

	bgVec_.emplace_back(
		BG(LoadGraph("resource/image/stage/road.png", true),
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


	ChangeVolumeSoundMem(200, bgm_);
	PlaySoundMem(bgm_, DX_PLAYTYPE_BACK);

#ifdef _DEBUG
	//bgImage_ = LoadGraph("resource/image/stage/titleBgFull.png", true);
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
			sPos.x = IpSceneMng.GetScreenSize().x - bg.speed;
			ePos.x = IpSceneMng.GetScreenSize().x * 2 - bg.speed;
		}

		if (0 > ePos2.x)
		{
			//�񖇖ډ�ʊO
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

		DrawExtendGraphF(sPos.x, sPos.y, ePos.x, ePos.y, bg.hadle, true);	//�ꖇ��
		DrawExtendGraphF(sPos2.x, sPos2.y, ePos2.x, ePos2.y, bg.hadle, true);	//�񖇖�
	}

	DrawGraph(logoPos_.x, logoPos_.y, logoImageH_,true);
	DrawString(550,600 - 16, "Start to Press Z", 0xffffff);

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
	DeleteSoundMem(decideSe_);

}

UniqueScene TitelScene::UpdateScene(UniqueScene& scene)
{

	if (controller_->ChaeckInputKey(KeyID::Transition))
	{

		ChangeVolumeSoundMem(255, decideSe_);
		PlaySoundMem(decideSe_, DX_PLAYTYPE_BACK);

		ChangeVolumeSoundMem(150,bgm_);
		return std::make_unique<SelectScene>(bgm_);
	}
	//�f�o�b�N�p
#ifdef _DEBUG

#endif

	//���̃V�[���ɖ߂�
	return std::move(scene);
}
