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
	pos_ = {500,250};


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

	refPow_ = { 0.0f,0.0f };
	refDir_ = { -1.0f,-1.0f };

	movePos_ = { refPow_ * refDir_ };
	speed_ = { 10,10 };
	vecLen = 0.0f;

	fastHitflg_ = false;
}

void Ball::Update()
{

	//攻撃判定ヒット時
	if (attackHitFlg_)
	{
		//vec += refPow_ * -refDir_;
	}
	//ステージ判定


	if (IsStageHit()) 
	{

		//ステージに当たったら

		//pos_ -= offset_;

		//speed_ = { 0,0 };
		vec = { 0,0 };
		//vec += refPow_ + speed_ * refDir_;
		//speed_ += 1;
		flg = true;
		attackHitFlg_ = false;
		//pos_ -= offset_;

	}
	else
	{

		vec += refPow_+ speed_* refDir_;
		//攻撃判定フラグがfalseなら
		if (!attackHitFlg_)
		{
			//pos_ += refPow_ * refDir_;

		}
	}


	if (vec.x != 0.0f || vec.y != 0.0f)
	{
		vecLen = vec.Magnitude();
		vecNom = vec / vecLen;

		pos_ += vecNom * speed_;

	}

	centerPos_ = { (raycast_.ballRay_[0].p.x + size_.x / 2),(raycast_.ballRay_[0].p.y + size_.y / 2) };

	SetBallform(pos_, size_);
	VelRay();
}

void Ball::Draw()
{
	DrawExtendGraph(raycast_.ballRay_[0].p.x, raycast_.ballRay_[0].p.y, raycast_.ballRay_[3].end.x, raycast_.ballRay_[3].end.y ,ballImage_,true);

	DrawCircle(centerPos_.x, centerPos_.y, 2, 0xff0000, true);

	DrawFormatString(600,600,0xffffff,"%f,%f", refPow_.x* refDir_.x, refPow_.y* refDir_.y);
	DrawFormatString(600,620,0xffffff,"%f,%f", refPow_.x* refDir_.x, refPow_.y* refDir_.y);

	//DrawFormatString(pos_.x + size_.x / 2 - 20, pos_.y - 20, 0xff0000, "ボール", true);
	DrawFormatString(48, 630, 0xff0000, "BallPosX%f,BallPosY%f", pos_.x, pos_.y);

	DrawFormatString(0, 100, 0xff0000, "speed_%f,%f", speed_.x, speed_.y);
	DrawFormatString(0, 150, 0xff0000, "refDir_%f,%f", refDir_.x,refDir_.y);
	DrawFormatString(0, 200, 0xff0000, "%d",attackHitFlg_);

	DrawBox(raycast_.ballRay_[0].p.x, raycast_.ballRay_[0].p.y, raycast_.ballRay_[3].end.x, raycast_.ballRay_[3].end.y, 0xffff00, false);

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
	movePos_ = { refPow_ + speed_ * refDir_ };

	//ボールの判定レイを設定
	raycast_.setBallRay(pos_ + movePos_, size_);

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

	//endPos_ = { (centerPos_ + size_+refPow_) * refDir_};

	Vector2 s = refPow_ + speed_ * refDir_;

	endPos_ = { centerPos_.x + size_.x * s.Normalized().x ,centerPos_.y + size_.y * s.Normalized().y} ;

	ballVec_ = endPos_ - centerPos_;

	//float mog = ballVec_.Magnitude();

	//Vector2 no = ballVec_.Normalized();

	//endPos_ = centerPos_ + (no * refPow_ * refDir_);

	Line ballLine{ {centerPos_.x,centerPos_.y }, {endPos_.x,endPos_.y} };

	_dbgDrawLine(ballLine.p.x, ballLine.p.y, ballLine.end.x, ballLine.end.y, 0xff0000);

	_dbgDrawFormatString(0, 300, 0xff0000, "Ray始点,%f,%f", ballLine.p.x, ballLine.p.y);
	_dbgDrawFormatString(0, 340, 0xff0000, "Ray終点,%f,%f", ballLine.end.x, ballLine.end.y);
}

void Ball::SetAttackRef(Vector2& refDir)
{
	refDir_ = refDir;
	attackHitFlg_ = true;
}
