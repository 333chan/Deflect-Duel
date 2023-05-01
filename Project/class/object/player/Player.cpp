#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../../tmx/TmxObj.h"

constexpr int MOVE_SPEED = 10;	// 移動速度
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
	pos_ = {100,100};
	size_ = {100,300};
	linepos_ = {300,100};
	linepos2_ = {500,100};
	gravity_ = 0.1;
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx",false);
}

void Player::Update(void)
{
	controller_->Update();
	//pos_.y += (FALL_SPEED + gravity_);
	gravity_ += FALL_ACCEL;

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

	//線
	if (controller_->ChaeckLongInputKey(KeyID::Up))
	{
		linepos_.y -= MOVE_SPEED;
	}
	if (controller_->ChaeckLongInputKey(KeyID::Down))
	{
		linepos_.y += MOVE_SPEED;
	}
	if (controller_->ChaeckLongInputKey(KeyID::Left))
	{
		linepos_.x -= MOVE_SPEED;
	}
	if (controller_->ChaeckLongInputKey(KeyID::Right))
	{
		linepos_.x += MOVE_SPEED;
	}

}

void Player::Draw(void)
{
	int color = 0;

	DrawBox(pos_.x, pos_.y, pos_.x+100, pos_.y+300,0xffffff,true);

	if (IsHit())
	{
		color = GetColor(255,0,0);
	}
	else
	{
		color = GetColor(255, 255, 255);
	}

	//DrawLine(linepos_.x, linepos_.y, linepos_.x+100, linepos_.y + 300, color,true);		//Player
	//DrawLine(linepos2_.x, linepos2_.y, linepos2_.x-200, linepos2_.y + 300, color, true);	//Stgae
	DrawLine(pos_.x,pos_.y+size_.y, pos_.x+ size_.x, pos_.y+ size_.y,color,true);
	DrawLine(stageLine_.p.x, stageLine_.p.y, stageLine_.end.x, stageLine_.end.y, color, true);

}

void Player::Release(void)
{
}

bool Player::IsHit()
{
	tmxObj_.GetTitleCollList();

	Vector2 stagePos;
	Vector2 stagePosEnd;

	for (auto& coll : tmxObj_.GetTitleCollList())
	{
		stagePos = coll.second.first;
		stagePosEnd = stagePos + coll.second.second;
	}


	Vector2 stageEnd = Vector2{ stagePosEnd.x,stagePos.y };

	stageLine_ = { {stagePos.x,stagePos.y},stageEnd };

	//３グループのベクトルを作成
	Vector2 a_to_b = Vector2(pos_.x + size_.x - pos_.x, pos_.y + size_.y - pos_.y + size_.y);
	Vector2 a_to_c  = Vector2(linepos2_.x  - linepos_.x, linepos2_.y - linepos_.y);
	Vector2 a_to_d  = Vector2(linepos2_.x - 200 - linepos_.x, linepos2_.y + 300 - linepos_.y);

	Vector2 c_to_d = Vector2(linepos2_.x - 200 - linepos2_.x, linepos2_.y + 300 - linepos2_.y);
	Vector2 c_to_a = Vector2(linepos_.x - linepos2_.x, linepos_.y - linepos2_.y);
	Vector2 c_to_b = Vector2(linepos_.x + 100 - linepos2_.x, linepos_.y + 300 - linepos2_.y);

	float d_01 = (a_to_b.x * a_to_c.y) - (a_to_c.x * a_to_b.y);
	float d_02 = (a_to_b.x * a_to_d.y) - (a_to_d.x * a_to_b.y);

	float d_03 = (c_to_d.x * c_to_a.y) - (c_to_a.x * c_to_d.y);
	float d_04 = (c_to_d.x * c_to_b.y) - (c_to_b.x * c_to_d.y);

	if (d_01*d_02 > 0.0f)
	{
		DrawString(100, 0, "はずれ", 0xffffff);
		return false;
	}

	if (d_03 * d_04 > 0.0f)
	{
		DrawString(100, 0, "はずれ", 0xffffff);
		return false;
	}

	{
		DrawString(100, 0, "Hit", 0xffffff);
		return true;
	}
}
