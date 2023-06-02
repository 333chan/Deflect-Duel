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
	pos_ = {200,400};

	//�傫��
	size_ = { 128,128 };

	rad_ = 60;

	//�d��
	gravity_ = 0.0;

	//�␳
	offset_ = { 0,0 };

	ballImage_ = LoadGraph("resource/image/ball/ball.png", true);

	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);
}

void Ball::Update()
{
	SetBallform(pos_, size_);

	if (!IsStageHit())
	{
		gravity_ += FALL_ACCEL;
		pos_.y += gravity_;
	
	}
	else
	{
		gravity_ = 0;
		pos_ -= offset_;
	}

}

void Ball::Draw()
{
	DrawExtendGraph(pos_.x, pos_.y, pos_ .x+size_.x, pos_.y + size_.y,ballImage_,true);

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
	raycast_.setBallRay(pos_, size_);
	//tmx��CollLiset�擾
	for (auto& coll : tmxObj_.GetStageCollList())
	{
		if (raycast_.StageToBallColl(coll, offset_))
		{
			return true;
		}
	}

	return false;
}
