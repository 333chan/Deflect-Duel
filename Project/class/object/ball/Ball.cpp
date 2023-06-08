#include <DxLib.h>
#include "../../scene/SceneManager.h"
#include "../../tmx/TmxObj.h"
#include "Ball.h"

constexpr float FALL_SPEED = 1.0f;	// 落下速度
constexpr float FALL_ACCEL = 0.1f;	// 重力加速度

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

	//座標
	pos_ = {200,500};

	//大きさ
	size_ = { 32,32 };

	rad_ = 60;

	//重力
	gravity_ = 0.0;

	//補正
	offset_ = { 0,0 };

	ballImage_ = LoadGraph("resource/image/ball/ball.png", true);

	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);

	//デバック用
	flg = false;
	movepow = {0,0};

	refPow={20,20};
}

void Ball::Update()
{
	if (!flg)
	{
		gravity_ += FALL_ACCEL;
		pos_.y += gravity_;
	}


	if (!IsStageHit())
	{

		refPow = refPow * refDir;
		pos_ += refPow;
	}
	else
	{
		gravity_ = 0;
		pos_ += offset_;
		flg = true;
	}

	SetBallform(pos_, size_);
}

void Ball::Draw()
{
	DrawExtendGraph(pos_.x, pos_.y, pos_ .x+size_.x, pos_.y + size_.y,ballImage_,true);

	DrawFormatString(600,600,0xffffff,"%f,%f", refPow.x* refDir.x, refPow.y* refDir.y);

	DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y,0xffff00, false);

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
	Vector2 refPos = { refPow * refDir };

	raycast_.setBallRay(pos_, size_, refPos);
	//tmxのCollLiset取得
	for (auto& coll : tmxObj_.GetStageCollList())
	{
		if (raycast_.StageToBallCheckColl(coll, offset_, refDir))
		{
			return true;
		}
	}

	return false;
}
