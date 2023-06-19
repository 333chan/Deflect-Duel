#include <DxLib.h>
#include "../../scene/SceneManager.h"
#include "../../tmx/TmxObj.h"
#include "../../../_debug/_DebugDispOut.h"
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
	pos_ = {500,500};


	//大きさ
	size_ = {32,32 };

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

	attackHitFlg_ = false;

	refPow_ = { 30,10 };
	refDir_ = { 1,0 };

	//movePos_ = { refPow_ * refDir_ };

	centerPos_ = { pos_ + movePos_ + size_ / 2 };
	vecLen = 0.0f;
}

void Ball::Update()
{
	VelRay();

	//if (!flg)
	//{
	//	gravity_ += FALL_ACCEL;
	//	pos_.x += refPow_.x * refDir_.x;
	//}

	//攻撃判定ヒット時
	if (attackHitFlg_)
	{
		vec += refPow_ * -refDir_;
	}


	//ステージ判定
	if (!IsStageHit())
	{
		//攻撃判定フラグがfalseなら
		if (!attackHitFlg_)
		{
			//pos_ += refPow_ * refDir_;
			vec += refPow_ * refDir_;
		}
	}
	else
	{
		//ステージに当たったら
		gravity_ = 0;
		vec.x = 0;
		vec.y = 0;
		//pos_ -= offset_;
		vec += refPow_ * refDir_;
		//refPow_ += 5;
		flg = true;
		attackHitFlg_ = false;
	}

	if (vec.x != 0.0f || vec.y != 0.0f)
	{
		vecLen = vec.Magnitude();
		vecNom = vec/vecLen;

		pos_ += vecNom * refPow_;

	}


	SetBallform(pos_, size_);
}

void Ball::Draw()
{
	DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y,ballImage_,true);

	DrawFormatString(600,600,0xffffff,"%f,%f", refPow_.x* refDir_.x, refPow_.y* refDir_.y);
	DrawFormatString(600,620,0xffffff,"%f,%f", refPow_.x* refDir_.x, refPow_.y* refDir_.y);


	DrawFormatString(pos_.x + size_.x / 2 - 20, pos_.y - 20, 0xff0000, "ボール", true);
	DrawFormatString(48, 630, 0xff0000, "BallPosX%f,BallPosY%f", pos_.x, pos_.y);

	DrawFormatString(0, 100, 0xff0000, "%f", movePos_.x);
	DrawFormatString(0, 200, 0xff0000, "%d",attackHitFlg_);

	DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, 0xffff00, false);

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

	//ボールの判定レイを設定
	raycast_.setBallRay(vel_ , size_);

	//tmxのCollLiset取得
	for (auto& coll : tmxObj_.GetStageCollList())
	{
		if (raycast_.StageToBallCheckColl(coll, offset_, refDir_))
		{
 			return true;
		}
	}

	return false;
}

void Ball::VelRay()
{
	endPos_ = { centerPos_ + refPow_ * refDir_ };

	ballVec_ = endPos_ - pos_;

	float mog = ballVec_.Magnitude();

	Vector2 no = ballVec_.Normalized();

	endPos_ = centerPos_ + no * refPow_ * refDir_;

	Line ballLine{ centerPos_,endPos_ };
	
	_dbgDrawLine(ballLine.p.x, ballLine.p.y, ballLine.end.x, ballLine.end.y, 0xff0000);
}

void Ball::SetAttackRef(Vector2& refDir)
{
	refDir_ = refDir;
	attackHitFlg_ = true;
}
