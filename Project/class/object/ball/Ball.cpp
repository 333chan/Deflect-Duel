#include <DxLib.h>
#include "../../scene/SceneManager.h"
#include "../../tmx/TmxObj.h"
#include "../../../_debug/_DebugDispOut.h"
#include "Ball.h"

constexpr float FALL_SPEED = 1.0f;	// 落下速度
constexpr float FALL_ACCEL = 0.1f;	// 重力加速度

const Vector2 MAX_SPEED = {30,30};

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

	ballImage_ = LoadGraph("resource/image/ball/ball32.png", true);

	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);

	//デバック用
	flg = false;

	attackHitFlg_ = false;

	refPow_ = { 0.0f,0.0f };
	refDir_ = { 1.0f,-1.0f };

	movePos_ = { refPow_ * refDir_ };
	speed_ = { 5,5 };
	vecLen = 0.0f;

	fastHitflg_ = false;
}

void Ball::Update()
{

	//攻撃判定ヒット時
	if (attackHitFlg_)
	{
		attackHitFlg_ = false;
		vec = { 0,0 };
		vec += refPow_ + speed_ * refDir_;

   		if (speed_ < MAX_SPEED)
		{
			speed_ += 2;
		}
	}
	//ステージ判定


	if (IsStageHit()) 
	{

		//ステージに当たったら
		//pos_ -= offset_;

		vec = { 0,0 };

		flg = true;
		attackHitFlg_ = false;
	}
	else
	{
		if (!attackHitFlg_)
		{
			vec += refPow_ + speed_ * refDir_;
			
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
	//デバック用
#ifdef _DEBUG


	DrawCircle(centerPos_.x, centerPos_.y, 2, 0xff0000, true);

	DrawFormatString(48, 630, 0xff0000, "BallPosX%f,BallPosY%f", pos_.x, pos_.y);


	DrawFormatString(0, 150, 0xff0000, "refDir_%d,%d", refDir_.x,refDir_.y);
	DrawFormatString(0, 200, 0xff0000, "%d",attackHitFlg_);

	DrawBox(raycast_.ballRay_[0].p.x, raycast_.ballRay_[0].p.y, raycast_.ballRay_[3].end.x, raycast_.ballRay_[3].end.y, 0xffff00, false);

	//DrawCircle(pos_.x + size_.x / 2, pos_.y + size_.y / 2, rad_, 0xffff00, true);
#endif

	//DrawExtendGraph(raycast_.ballRay_[0].p.x, raycast_.ballRay_[0].p.y, raycast_.ballRay_[3].end.x, raycast_.ballRay_[3].end.y, ballImage_, true);

	static double angle= 0.0;
	angle += 0.2;

	DrawRotaGraph(raycast_.ballRay_[0].p.x + 16, raycast_.ballRay_[0].p.y + 16, 1, angle, ballImage_, true);

	DrawFormatString(IpSceneMng.GetScreenSize().x / 2 - 50, 650, 0xffffff, "SPEED");
	DrawFormatString(IpSceneMng.GetScreenSize().x / 2 - 100, 680, 0xfffffff, "%f:%f", speed_.x, speed_.y);
	DrawFormatString(IpSceneMng.GetScreenSize().x / 2 - 110, 10, 0xfffffff, "ボールを相手に当てろ！");
}

void Ball::Release()
{

	DeleteGraph(ballImage_);
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
	fastHitflg_ = true;
}
