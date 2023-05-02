#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../../tmx/TmxObj.h"
#include"../../../_debug/_DebugDispOut.h"

constexpr int MOVE_SPEED = 10;		// 移動速度
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
	size_ = {100,300};

	//重力
	gravity_ = 0.1;

	//補正差分
	offset_ = 0.0f;

	//tmxの読み込み
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx",false);

	Dir::None;
}

void Player::Update(void)
{
	controller_->Update();

	//簡易的な重力計算
	if (!IsHit())
	{
		//当たってなかったら
		pos_.y += (FALL_SPEED + gravity_);
		//gravity_ += FALL_ACCEL;
	}

	if(IsHit())
	{
		//当たってたら
		pos_.y -= offset_;
		gravity_ = 0;
	}


	//プレイヤー移動
	if (controller_->ChaeckLongInputKey(KeyID::Up))
	{
		pos_.y -= MOVE_SPEED;
		
	}
	if (controller_->ChaeckLongInputKey(KeyID::Down))
	{
		pos_.y += MOVE_SPEED;
		
	}
	if (controller_->ChaeckLongInputKey(KeyID::Left))
	{
		pos_.x -= MOVE_SPEED;
	}
	if (controller_->ChaeckLongInputKey(KeyID::Right))
	{
		pos_.x += MOVE_SPEED;
	}
	
}

void Player::Draw(void)
{
	//色
	color = GetColor(0, 255, 0);

	//プレイヤー描画
	DrawBox(pos_.x, pos_.y, pos_.x+100, pos_.y+300,0xffffff,true);

	
	if (IsHit())
	{
		//当たったら赤色に変更
		color = GetColor(255,0,0);
	}
	else
	{
		color = GetColor(000, 255, 000);
	}

	//プレイヤ
	DrawLine(playerLine_.p.x, playerLine_.p.y, playerLine_.end.x, playerLine_.end.y, color, true);
	DrawBox(stageLine_.p.x, stageLine_.p.y, stageLine_.end.x, stageLine_.end.y, color, true);

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
		
		if (raycast_.CheckCollision(pos_, size_, coll, offset_, color))
		{
			return true;
		}
	}

	return false;


}
