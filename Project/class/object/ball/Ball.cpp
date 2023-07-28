#include <DxLib.h>
#include "../../scene/SceneManager.h"
#include "../../common/SoundManager.h"
#include"../../common/ImageManager.h"
#include "../../common/AnimController.h"
#include "../../tmx/TmxObj.h"
#include "../../../_debug/_DebugDispOut.h"
#include "Ball.h"

constexpr float FALL_SPEED = 1.0f;	// 落下速度
constexpr float FALL_ACCEL = 0.1f;	// 重力加速度

constexpr float MAX_SPIN_SPEED = 20.0f;	//最高回転速度

const Vector2 MAX_SPEED = {30,30};	//最高速度

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
	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);

	animController_ = std::make_unique<AnimController>();

	//座標
	pos_ = {500,200};

	//大きさ
	collSize_ = {32,32 };

	//補正値
	offset_ = { 0,0 };

	refPow_ = { 0.0f,0.0f };
	refDir_ = { 0.0f,0.0f };

	movePos_ = { refPow_ * refDir_ };
	speed_ = { 5,5 };

	vecLen_ = 0.0f;

	angle_ = 0.0;
	spinSpead_ = 0.2;

	attackHitFlg_ = false;

	fastHitflg_ = false;
}

void Ball::Update()
{

	//攻撃判定ヒット時
	if (attackHitFlg_)
	{
		attackHitFlg_ = false;
		vec_ = { 0,0 };
		vec_ += refPow_ + speed_ * refDir_;

		if (spinSpead_< MAX_SPIN_SPEED)
		{
			spinSpead_ += 0.025;
		}

   		if (speed_ < MAX_SPEED)
		{
			speed_ += 1.5f;
		}
	}

	//ステージ判定
	if (IsStageHit()) 
	{

		//ステージに当たったら
		//pos_ -= offset_;
		vec_ = { 0,0 };

		ChangeVolumeSoundMem(180, lpSoundMng.GetID("ballSe"));
		PlaySoundMem(lpSoundMng.GetID("ballSe"), DX_PLAYTYPE_BACK);

		attackHitFlg_ = false;
	}
	else
	{
		if (!attackHitFlg_)
		{
			vec_ += refPow_ + speed_ * refDir_;
		}
	}


	if (vec_.x != 0.0f || vec_.y != 0.0f)
	{
		vecLen_ = vec_.Magnitude();
		vecNom_ = vec_ / vecLen_;

		pos_ += vecNom_ * speed_;

	}

	centerPos_ = { (raycast_.ballRay_[0].p.x + collSize_.x / 2),(raycast_.ballRay_[0].p.y + collSize_.y / 2) };

	SetBallform(pos_, collSize_);
	VelRay();

	//回転処理

	angle_ += spinSpead_;

}

void Ball::Draw()
{
	//デバック用
#ifdef _DEBUG
	DrawCircle(centerPos_.x, centerPos_.y, 2, 0xff0000, true);

	DrawFormatString(450, 700, 0xffffff, "BallPosX%f,BallPosY%f", pos_.x, pos_.y);
	DrawFormatString(450, 600, 0xffffff, "%f",spinSpead_);

	DrawFormatString(0, 150, 0xff0000, "refDir_%d,%d", refDir_.x,refDir_.y);
	DrawFormatString(0, 200, 0xff0000, "%d",attackHitFlg_);

	DrawBox(raycast_.ballRay_[0].p.x, raycast_.ballRay_[0].p.y, raycast_.ballRay_[3].end.x, raycast_.ballRay_[3].end.y, 0xffff00, false);
#endif
	animController_->SetAnim(Anim::Spin);
	DrawRotaGraph2(
		raycast_.ballRay_[0].p.x + collSize_.x / 2, raycast_.ballRay_[0].p.y + collSize_.y / 2, 
		17, 24,
		1.5, angle_, lpImageMng.GetID("ball")[animController_->Update()], true);

	DrawFormatString(lpSceneMng.GetScreenSize().x / 2 - 50, 650, 0xffffff, "SPEED");
	DrawFormatString(lpSceneMng.GetScreenSize().x / 2 - 100, 680, 0xfffffff, "%f:%f", speed_.x, speed_.y);
	DrawFormatString(lpSceneMng.GetScreenSize().x / 2 - 110, 10, 0xfffffff, "ボールを相手に当てろ！");
}

void Ball::Release()
{

}

void Ball::SetBallform(Vector2& pos, Vector2& size)
{
	pos = pos_;
	size = collSize_;
}

bool Ball::IsStageHit()
{
	movePos_ = { refPow_ + speed_ * refDir_ };

	//ボールの判定レイを設定
	raycast_.setBallRay(pos_ + movePos_, collSize_);

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

	endPos_ = { centerPos_.x + collSize_.x * s.Normalized().x ,centerPos_.y + collSize_.y * s.Normalized().y} ;

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
