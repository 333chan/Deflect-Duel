#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../../tmx/TmxObj.h"
#include"../../../_debug/_DebugDispOut.h"

constexpr int MOVE_SPEED = 10.0f;		// �ړ����x
constexpr int JUMP_POW = 10.0f;		// �W�����v��
constexpr float FALL_SPEED = 1.0f;	// �������x
constexpr float FALL_ACCEL = 0.1f;	// �d�͉����x

Player::Player(ControllerType type)
{
	//�R���g���[���[�̐���
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
	//�v���C���[���W
	pos_ = {100,100};

	//�v���C���[�T�C�Y
	size_ = {50,100};

	//���
	state_ = State::Idel;

	//�d��
	gravity_ = 0.1;

	//�␳����
	offset_ = { 0.0f ,0.0f };

	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx",false);

	jumpDeltaTime_ = 0.0;


}

void Player::Update(void)
{

	auto jmupVel = JUMP_POW * jumpDeltaTime_ - (0.5 * (gravity_ * jumpDeltaTime_));

	controller_->Update();
	//state_ = State::Idel;
	if (IsHit())
	{
		//�������Ă���␳
		//pos_ -= offset_;
		//gravity_ = 0;
	}

	
	switch (state_)
	{
	case State::Idel:
	{
		//auto jmupVel = JUMP_POW * jumpDeltaTime_ - (0.5 * (gravity_ * jumpDeltaTime_));
		if (jmupVel < 0)
		{
			//state_ = State::Fall;
		}

		//�v���C���[�ړ�
		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			//�W�����v
			state_ = State::JumpUp;
		}
		if (controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//���Ⴊ��
			state_ = State::Crouching;

		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//���ړ�
			state_ = State::MoveLeft;
		}
		else if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			state_ = State::MoveRight;
		}
	}
		break;
	case State::JumpUp:
	{
		jumpDeltaTime_ += IpSceneMng.GetDeltaTime();

		auto YVel = -JUMP_POW* jumpDeltaTime_ + (0.5 * gravity_* std::pow(jumpDeltaTime_, 2.0));
		pos_.y += YVel;
		gravity_ += FALL_ACCEL;

		if (YVel > 0)
		{
			DrawBox(300, 300, 600, 600, 0xf0f0f, true);
			//state_ = State::Fall;
		}
	}
	break;

	case State::Fall:

		jumpDeltaTime_ = 0.0;
		jumpDeltaTime_ += IpSceneMng.GetDeltaTime();

		pos_.y += 0.5 * (gravity_ * jumpDeltaTime_);

		pos_.y += (FALL_SPEED + gravity_);
		gravity_ += FALL_ACCEL;


		if (IsHit())
		{
			//�������Ă���␳
			pos_ -= offset_;
			gravity_ = 0;
			state_ = State::Idel;
		}

		break;
	case State::MoveLeft:

		//��
		pos_.x -= MOVE_SPEED;
		if (!controller_->ChaeckLongInputKey(KeyID::Left))
		{
			state_ = State::Idel;
		}

		if (jmupVel < 0)
		{
			state_ = State::Fall;
		}
		else 
		{

		}

		break;
	case State::MoveRight:
		//�E
		pos_.x += MOVE_SPEED;
		if (!controller_->ChaeckLongInputKey(KeyID::Right))
		{
			state_ = State::Idel;
		}
		break;
	case State::Crouching:
		break;
	case State::Attack:
		break;
	case State::Max:
		break;
	[[likery]]default:
		break;
	}

	//�ȈՓI�ȏd�͌v�Z
	if (!IsHit())
	{
		//�������ĂȂ�������
		pos_.y += (FALL_SPEED + gravity_);
		gravity_ += FALL_ACCEL;
	}



}

void Player::Draw(void)
{
	//�F
	color = GetColor(0, 255, 0);

	//�v���C���[�`��
	DrawBox(pos_.x, pos_.y, pos_.x+ size_.x, pos_.y+ size_.y,0xffffff,true);

	//�v���C��
	DrawLine(playerLine_.p.x, playerLine_.p.y, playerLine_.end.x, playerLine_.end.y, color, true);

	DrawFormatString(100,20,0xffffff,"",state_, true);



	switch (state_)
	{
	case State::Idel:
		DrawFormatString(300, 30, 0xffffff, "State:Idel");
		break;
	case State::JumpUp:
		DrawFormatString(300, 30, 0xffffff, "State:JumpUp");
		break;
	case State::Fall:
		DrawFormatString(300, 30, 0xffffff, "State:Fall");
		break;
	case State::MoveLeft:
		DrawFormatString(300, 30, 0xffffff, "State:Move");
		break;
	case State::MoveRight:
		DrawFormatString(300, 30, 0xffffff, "State:Move");
		break;
	case State::Crouching:
		DrawFormatString(300, 30, 0xffffff, "State:Crouching");
		break;
	case State::Attack:
		DrawFormatString(300, 30, 0xffffff, "State:Attack");
		break;
	case State::Max:
		break;
	default:
		break;
	}

	if (IsHit())
	{
		//����������ԐF�ɕύX
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
	Vector2 stagePos;		//�n�_
	Vector2 stagePosEnd;	//�I�_

	//tmx��CollLiset�擾
	for (auto& coll : tmxObj_.GetStageCollList())
	{
		stagePos = coll.first;
		stagePosEnd = stagePos + coll.second;

		//�E��̏I�_
		Vector2 stageEnd = Vector2{ stagePosEnd.x,stagePos.y };

		//Line�̍쐬
		stageLine_ = { stagePos,stageEnd };
		
		if (raycast_.CheckCollision(pos_, size_, coll, state_, offset_,color))
		{
			return true;
		}
	}

	return false;


}
