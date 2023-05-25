#include <DxLib.h>
#include "../../scene/SceneManager.h"
#include "../../tmx/TmxObj.h"
#include "Ball.h"
#include "../../object/player/Player.h"

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
	pos_ = {200,200};

	//大きさ
	size_ = { 0,0 };

	//重力
	gravity_ = 0.1;

	//補正
	offset_ = { 0,0 };

	ballImage_ = LoadGraph("resource/image/ball/ball.png", true);

	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);

	GetGraphSizeF(ballImage_, &size_.x, &size_.y);


}

void Ball::Update()
{
	
	if (IsStageHit())
	{
		gravity_ = 0;
	}
	else
	{
		gravity_ += FALL_ACCEL;
		pos_.y += gravity_;

	}
}

void Ball::Draw()
{
	//DrawBox(pos_.x, pos_.y, pos_.x+size_.x, pos_.y + size_.y, 0xffffff, false);
	DrawExtendGraph(pos_.x, pos_.y, pos_ .x+size_.x, pos_.y + size_.y,ballImage_,true);
}

void Ball::Release()
{
}

bool Ball::IsStageHit()
{
	//tmxのCollLiset取得
	for (auto& coll : tmxObj_.GetStageCollList())
	{
		if (raycast_.CheckStagetoBallCollision(coll, pos_, size_, offset_))
		{
			return true;
		}
	}

	return false;
}
