#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../../tmx/TmxObj.h"
#include"../../../_debug/_DebugDispOut.h"

constexpr int MOVE_SPEED = 20.0f;		// �ړ����x
constexpr int JUMP_POW = 14.0f;		// �W�����v��
//constexpr float FALL_SPEED = 1.0f;	// �������x
constexpr float FALL_ACCEL = 1.0f;	// �d�͉����x

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
	pos_ = {100,300};

	//�v���C���[�T�C�Y
	size_ = {50,100};

	//���
	state_ = State::Idel;

	//����
	dir_ = Dir::Max;

	//�d��
	gravity_ = 0.1;

	//�␳����
	offset_ = { 0.0f ,0.0f };

	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx",false);

	jumpDeltaTime_ = 0.0;

	fallFlg_ = false;

}

void Player::Update(void)
{

	auto jmupVel = JUMP_POW * jumpDeltaTime_ - (0.5 * (gravity_ * jumpDeltaTime_));

	controller_->Update();

	switch (state_)
	{
	case State::Idel:
	{
		gravity_ = 0;

		//if (!CheckHitKeyAll(DX_CHECKINPUT_KEY))
		//{
		//	dir_ = Dir::Max;
		//}

		//�v���C���[�ړ�
		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			//�W�����v
			state_ = State::JumpUp;
			dir_ = Dir::Up;
			break;
		}
		if (controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//���Ⴊ��
			//state_ = State::Crouching;
			dir_ = Dir::Down;
		}
		if (!IsHit(Line({ pos_.x + size_.x / 2, pos_.y + size_.y / 2 }, { pos_.x + size_.x / 2,pos_.y + size_.y })))
		{
			jumpDeltaTime_ = 1.29959;
			gravity_ = 7.79999;
			state_ = State::Fall;
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//���ړ�
			state_ = State::MoveLeft;
			dir_ = Dir::Left;
		}
		else if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			state_ = State::MoveRight;
			dir_ = Dir::Right;
		}
	}
	break;
	case State::JumpUp:
	{

		gravity_ += FALL_ACCEL;
		jumpDeltaTime_ += IpSceneMng.GetDeltaTime();
		
		auto YVel = -JUMP_POW + (2.0f * gravity_ * std::pow(jumpDeltaTime_, 2.0));
		pos_.y += YVel;

		if (YVel > 0)
		{
			fallFlg_ = false;
			//DrawBox(300, 300, 600, 600, 0xf0f0f, true);
			state_ = State::Fall;
		}



		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			dir_ = Dir::Right;

			//�E�ړ�
			pos_.x += MOVE_SPEED;
			//state_ = State::MoveRight;
			if (IsHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
			

		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			dir_ = Dir::Left;
			//�E�ړ�
			pos_.x -= MOVE_SPEED;
			//state_ = State::MoveLeft;
			if (IsHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
		}
	}
	break;
	case State::Fall:
	{
		dir_ = Dir::Down;

		//jumpDeltaTime_ = 0.0;
		jumpDeltaTime_ += IpSceneMng.GetDeltaTime();
		gravity_ += FALL_ACCEL;

		auto YVel = -JUMP_POW + (gravity_ * std::pow(jumpDeltaTime_, 2.0));
		pos_.y += YVel;

		//pos_.y += (FALL_SPEED + gravity_);

		if (IsHit(Line({ pos_.x + size_.x / 2, pos_.y + size_.y / 2 }, { pos_.x + size_.x / 2,pos_.y + size_.y })))
		{
			//�������Ă���␳
			pos_ -= offset_;
			
			state_ = State::Idel;
			fallFlg_ = false;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			pos_.x += MOVE_SPEED;
			dir_ = Dir::Right;
			//state_ = State::MoveRight;
			if (IsHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//���ړ�
			pos_.x -= MOVE_SPEED;
			dir_ = Dir::Left;

			//state_ = State::MoveLeft;
			if (IsHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
		}


	}
	break;
	case State::MoveLeft:
	{


		//��
		pos_.x -= MOVE_SPEED;



		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			state_ = State::JumpUp;
		}
		if (IsHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}


		if (!controller_->ChaeckLongInputKey(KeyID::Left))
		{
			state_ = State::Idel;
		}



	}
		break;
	case State::MoveRight:
	{
		//�E
		pos_.x += MOVE_SPEED;
		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			state_ = State::JumpUp;
		}

		if (IsHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Right))
		{
			state_ = State::Idel;
		}
	}
		break;
	case State::Crouching:
		if (!controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//state_ = State::Idel;
		}
		break;
	case State::Attack:
		break;
	case State::Max:
		//state_ = State::Idel;
		break;
	[[likery]]default:
		break;
	}

	//if (IsHit())
	//{
	//	//�������Ă���␳
	//	pos_ -= offset_;
	//	//gravity_ = 0;
	//}

}

void Player::Draw(void)
{
	//�F
	color = GetColor(0, 255, 0);

	//�v���C���[�`��
	DrawBox(pos_.x, pos_.y, pos_.x+ size_.x, pos_.y+ size_.y,0xffffff,true);

	//�v���C��
	DrawLine(playerLine_.p.x, playerLine_.p.y, playerLine_.end.x, playerLine_.end.y, color, true);



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

	switch (dir_)
	{
	case Dir::Up:
		DrawFormatString(450, 30, 0xffffff, "Dir:Up");
		break;
	case Dir::Down:
		DrawFormatString(450, 30, 0xffffff, "Dir:Down");
		break;
	case Dir::Right:
		DrawFormatString(450, 30, 0xffffff, "Dir:Right");
		break;
	case Dir::Left:
		DrawFormatString(450, 30, 0xffffff, "Dir:Left");
		break;
	case Dir::Max:
		DrawFormatString(450, 30, 0xffffff, "Dir:MAX");
		break;
	default:
		break;
	}
	//DrawLine(stageLine_.p.x, stageLine_.p.y, stageLine_.end.x, stageLine_.end.y, color, true);
}

void Player::Release(void)
{
}

bool Player::IsHit(Line collRay)
{
	Vector2 stagePos;		//�n�_
	Vector2 stagePosEnd;	//�I�_
	_dbgDrawLine(collRay.p.x, collRay.p.y, collRay.end.x, collRay.end.y, 0xff0000);

	//tmx��CollLiset�擾
	for (auto& coll : tmxObj_.GetStageCollList())
	{
		stagePos = coll.first;
		stagePosEnd = stagePos + coll.second;

		//�E��̏I�_
		Vector2 stageEnd = Vector2{ stagePosEnd.x,stagePos.y };

		//Line�̍쐬
		stageLine_ = { stagePos,stageEnd };
		
		if (raycast_.CheckCollision(pos_, size_, coll, dir_, collRay, offset_,color))
		{
			return true;
		}
	}

	return false;
}


