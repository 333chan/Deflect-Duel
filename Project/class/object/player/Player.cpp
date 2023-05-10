#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../../tmx/TmxObj.h"
#include"../../../_debug/_DebugDispOut.h"

constexpr int MOVE_SPEED = 10.0f;		// 移動速度
constexpr int JUMP_POW = 0.001f;		// ジャンプ力
constexpr float FALL_SPEED = 1.0f;	// 落下速度
constexpr float FALL_ACCEL = 0.1f;	// 重力加速度

Player::Player(ControllerType type)
{
	//コントローラーの生成
	if (type == ControllerType::Pad)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	Init();
}

Player::~Player()
{
	Release();
}

void Player::Init()
{
	//プレイヤー座標
	pos_ = {100,100};

	//プレイヤーサイズ
	size_ = {50,100};

	//方向
	dir_ = Dir::Max;

	//重力
	gravity_ = 0.1;

	//補正差分
	offset_ = { 0.0f ,0.0f };

	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx",false);

}

void Player::Update(void)
{
	controller_->Update();



	dir_ = Dir::Max;

	//プレイヤー移動
	if (controller_->ChaeckInputKey(KeyID::Up))
	{
		pos_.y = JUMP_POW * IpSceneMng.GetDeltaTime() +0.5*(gravity_ * (IpSceneMng.GetDeltaTime() * IpSceneMng.GetDeltaTime()));
		dir_ = Dir::Up;
	}
	if (controller_->ChaeckLongInputKey(KeyID::Down))
	{
		//pos_.y += MOVE_SPEED;
		dir_ = Dir::Down;
		
	}
	if (controller_->ChaeckLongInputKey(KeyID::Left))
	{
		pos_.x -= MOVE_SPEED;
		dir_ = Dir::Left;
	}
	else if (controller_->ChaeckLongInputKey(KeyID::Right))
	{
		pos_.x += MOVE_SPEED;
		dir_ = Dir::Right;
	}

	//簡易的な重力計算
	if (!IsHit())
	{
		//当たってなかったら
		pos_.y += (FALL_SPEED + gravity_);
		gravity_ += FALL_ACCEL;
	}

	if (IsHit())
	{
		//当たってたら補正
		pos_ -= offset_;
		gravity_ = 0;
	}

}

void Player::Draw(void)
{
	//色
	color = GetColor(0, 255, 0);

	//プレイヤー描画
	DrawBox(pos_.x, pos_.y, pos_.x+ size_.x, pos_.y+ size_.y,0xffffff,true);

	//プレイヤ
	DrawLine(playerLine_.p.x, playerLine_.p.y, playerLine_.end.x, playerLine_.end.y, color, true);

	//DrawFormatString(800, 0, 0xffffff, "%p",IpSceneMng.GetDeltaTime());

	switch (dir_)
	{
	case Dir::Up:
		DrawFormatString(200, 0, 0xffffff, "Up");
		break;
	case Dir::Down:
		DrawFormatString(200, 0, 0xffffff, "Down");
		DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y/2, 0xffff00, true);
		break;
	case Dir::Left:
		DrawFormatString(200, 0, 0xffffff, "Left");
		break;
	case Dir::Right:
		DrawFormatString(200, 0, 0xffffff, "Right");
		break;
	default:
		break;
	}

	if (IsHit())
	{
		//当たったら赤色に変更
		color = GetColor(255, 0, 0);
	}
	else
	{
		color = GetColor(000, 255, 000);
	}
	//DrawLine(stageLine_.p.x, stageLine_.p.y, stageLine_.end.x, stageLine_.end.y, color, true);
}

void Player::Release(void)
{
}

bool Player::IsHit()
{
	Vector2 stagePos;		//始点
	Vector2 stagePosEnd;	//終点

	//tmxのCollLiset取得
	for (auto& coll : tmxObj_.GetStageCollList())
	{
		stagePos = coll.first;
		stagePosEnd = stagePos + coll.second;

		//右上の終点
		Vector2 stageEnd = Vector2{ stagePosEnd.x,stagePos.y };

		//Lineの作成
		stageLine_ = { stagePos,stageEnd };
		
		if (raycast_.CheckCollision(pos_, size_, coll, dir_, offset_,color))
		{
			return true;
		}
	}

	return false;


}
