#include <DxLib.h>
#include "../../scene/SceneManager.h"
#include "../../tmx/TmxObj.h"
#include "Ball.h"

constexpr float FALL_SPEED = 1.0f;	// �������x
constexpr float FALL_ACCEL = 0.1f;	// �d�͉����x

Ball::Ball()
{
	Init();
}

Ball::~Ball()
{
	Release();
}

void Ball::Init()
{

	//���W
	pos_ = {200,500};

	//�傫��
	size_ = { 32,32 };

	rad_ = 60;

	//�d��
	gravity_ = 0.0;

	//�␳
	offset_ = { 0,0 };

	ballImage_ = LoadGraph("resource/image/ball/ball.png", true);

	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);

	//�f�o�b�N�p
	flg = false;

	attackHitFlg_ = false;

	refPow_={20,20};

	attackRefPow_ ={20,20};
}

void Ball::Update()
{
	if (!flg)
	{
		gravity_ += FALL_ACCEL;
		pos_.x += gravity_;
	}

	//�U������q�b�g��
	if (attackHitFlg_)
	{
		pos_ += attackRefPow_ * attackRefDir_;
	}


	//�X�e�[�W����
	if (!IsStageHit())
	{
		//�U������t���O��false�Ȃ�
		if (!attackHitFlg_)
		{
			pos_ += refPow_ * refDir_;
		}
	}
	else
	{
		//�X�e�[�W�ɓ���������
		gravity_ = 0;
		pos_ += refPow_ * refDir_;
		flg = true;
		attackHitFlg_ = false;
	}

	attackPos_ = movePos_;

	SetBallform(pos_, size_);
}

void Ball::Draw()
{
	DrawExtendGraph(pos_.x, pos_.y, pos_ .x+size_.x, pos_.y + size_.y,ballImage_,true);

	DrawFormatString(600,600,0xffffff,"%f,%f", refPow_.x* refDir_.x, refPow_.y* refDir_.y);
	DrawFormatString(600,620,0xffffff,"%f,%f", refPow_.x* attackRefDir_.x, refPow_.y* attackRefDir_.y);

	DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y,0xffff00, false);

	DrawFormatString(pos_.x + size_.x / 2 - 20, pos_.y - 20, 0xff0000, "�{�[��", true);
	DrawFormatString(48, 630, 0xff0000, "BallPosX%f,BallPosY%f", pos_.x, pos_.y);

	DrawFormatString(0, 100, 0xff0000, "%f", movePos_.x);
	DrawFormatString(0, 200, 0xff0000, "%d",attackHitFlg_);


	//DrawCircle(pos_.x + size_.x / 2, pos_.y + size_.y / 2, rad_, 0xffff00, true);
}

void Ball::Release()
{
}

void Ball::SetBallform(Vector2& pos, Vector2& size)
{
	pos = pos_;
	size = size_;
}

bool Ball::IsStageHit()
{
	movePos_ = { refPow_ * refDir_ };

	if (attackHitFlg_)
	{
		
	}
	
	//�{�[���̔��背�C��ݒ�
   	raycast_.setBallRay(pos_, size_, movePos_, attackPos_*0);

	//tmx��CollLiset�擾
	for (auto& coll : tmxObj_.GetStageCollList())
	{
		if (raycast_.StageToBallCheckColl(coll, offset_, refDir_))
		{
			return true;
		}
	}

	return false;
}

void Ball::SetAttackRef(Vector2& refDir)
{
	attackRefDir_ = refDir;
	attackHitFlg_ = true;
}
