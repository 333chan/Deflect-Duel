#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"

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
	gravity_ = 0.1;
}

void Player::Update(void)
{
	controller_->Update();
	pos_.y += (FALL_SPEED + gravity_);
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
}

void Player::Draw(void)
{
	DrawCircle(pos_.x, pos_.y,50,0xffffff,true);
}

void Player::Release(void)
{
}
