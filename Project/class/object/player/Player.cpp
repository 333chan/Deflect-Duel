#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../../tmx/TmxObj.h"
#include"../../../_debug/_DebugDispOut.h"

constexpr int MOVE_SPEED = 20.0f;		// �ړ����x
constexpr int JUMP_POW = 15.0f;		// �W�����v��
//constexpr float FALL_SPEED = 1.0f;	// �������x
constexpr float FALL_ACCEL = 1.0f;	// �d�͉����x

Player::Player(ControllerType type, std::shared_ptr<Ball>& ball)
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
	
	ball_ = ball;

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
	size_ = {48,96};
	attacksize_ = {96,96};

	ballpos_ = {0,0};
	ballsize_ = { 0,0 };

	//���
	state_ = State::Idel;

	//����
	dir_ = Dir::Max;

	//�d��
	gravity_ = 0.1;

	//�␳����
	offset_ = { 0.0f ,0.0f };

	refDir_ = { 0.0f ,0.0f };

	jumpDeltaTime_ = 0.0;

	//Idel
	playerImage_ = LoadGraph("resource/image/character/player_idle.png", true);
	playerImage2_ = LoadGraph("resource/image/character/player_move.png", true);
	playerImage3_ = LoadGraph("resource/image/character/Player_Crouch.png", true);
	playerImage4_ = LoadGraph("resource/image/character/player_up.png", true);
	playerImage5_ = LoadGraph("resource/image/character/player_down.png", true);
	playerImage6_ = LoadGraph("resource/image/character/player_attack.png", true);
	playerImage7_ = LoadGraph("resource/image/character/player_death.png", true);

	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);
	movePos_ = { MOVE_SPEED , MOVE_SPEED };

}

void Player::Update(void)
{


	controller_->Update();

	switch (state_)
	{
	case State::Idel:
	{
		gravity_ = 0;

		if (!CheckHitKeyAll(DX_CHECKINPUT_KEY))
		{
			dir_ = Dir::Max;
		}

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
		if (!IsStageHit(Line({ pos_.x + size_.x / 2, pos_.y + size_.y / 2 }, { pos_.x + size_.x / 2,pos_.y + size_.y })))
		{
			jumpDeltaTime_ = 1.3;
			gravity_ = 7.8;
			state_ = State::Fall;
			break;
		}
		if (controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//���Ⴊ��
			//state_ = State::Crouching;
			dir_ = Dir::Down;
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
		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			state_ = State::Attack;
		}
	}
	break;
	case State::JumpUp:
	{
		auto YVel = -JUMP_POW + (2.0f * gravity_ * std::pow(jumpDeltaTime_, 2.0));

		if (YVel > 0)
		{
			state_ = State::Idel;

		}

		gravity_ += FALL_ACCEL;
		jumpDeltaTime_ += IpSceneMng.GetDeltaTime();
		

		pos_.y += YVel;

		if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x / 2,pos_.y })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			dir_ = Dir::Right;

			//�E�ړ�
			pos_.x += MOVE_SPEED;
			//state_ = State::MoveRight;
			if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
			

		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			dir_ = Dir::Left;
			//���ړ�
			pos_.x -= MOVE_SPEED;
			//state_ = State::MoveLeft;
			if (IsStageHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
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

		jumpDeltaTime_ += IpSceneMng.GetDeltaTime();
		gravity_ += FALL_ACCEL;

		auto YVel = -JUMP_POW + (gravity_ * std::pow(jumpDeltaTime_, 2.0));
		pos_.y += YVel;

		if (IsStageHit(Line({ pos_.x + size_.x / 2, pos_.y + size_.y / 2 }, { pos_.x + size_.x / 2,pos_.y + size_.y })))
		{
			//�������Ă���␳
			pos_ -= offset_;
			
			state_ = State::Idel;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			pos_.x += MOVE_SPEED;
			dir_ = Dir::Right;
			if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
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
			if (IsStageHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
		}


	}
	break;
	case State::MoveLeft:
	{
		//���ړ�
		pos_.x -= MOVE_SPEED;

		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			state_ = State::JumpUp;
		}

		if (IsStageHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}


		if (!controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//�ړ��L�[���������Idel
			state_ = State::Idel;
		}
	}
		break;
	case State::MoveRight:
	{
		//�E�ړ�
		pos_.x += MOVE_SPEED;

		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			state_ = State::JumpUp;
		}

		if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�ړ��L�[���������Idel
			state_ = State::Idel;
		}
	}
		break;

	case State::Crouching:

		if (!controller_->ChaeckLongInputKey(KeyID::Down))
		{
			state_ = State::Idel;
		}
		break;

	case State::Attack:
		

		if(IsAttackHit())
		{
			ball_->SetAttackRef(refDir_);
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Attack))
		{
			state_ = State::Idel;
		}

		break;
	case State::Death:

		_dbgDrawFormatString(500, 300, 0xffffff, "���ɂ܂����[", true);

		break;
	case State::Max:
		break;
	[[likery]]default:
		break;
	}
	if (IsBallHit())
	{
		state_ = State::Death;
		
	}


	attackpos_ = { pos_.x + size_.x,pos_.y };

}

void Player::Draw(void)
{
	//�v���C���[�`��
#ifdef _DEBUG	//�f�o�b�N���̂�
	switch (state_)
	{
	case State::Idel:
		DrawFormatString(300, 30, 0xffffff, "State:Idel");
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, playerImage_, true);
		break;
	case State::JumpUp:
		DrawFormatString(300, 30, 0xffffff, "State:JumpUp");
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, playerImage4_, true);
		break;
	case State::Fall:
		DrawFormatString(300, 30, 0xffffff, "State:Fall");
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, playerImage5_, true);
		break;
	case State::MoveLeft:
		DrawFormatString(300, 30, 0xffffff, "State:Left");
		DrawExtendGraph(pos_.x +size_.x, pos_.y, pos_.x, pos_.y + size_.y, playerImage2_, true);
		break;
	case State::MoveRight:
		DrawFormatString(300, 30, 0xffffff, "State:Right");
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, playerImage2_, true);
		break;
	case State::Crouching:
		DrawFormatString(300, 30, 0xffffff, "State:Crouching");
		break;
	case State::Attack:
		DrawFormatString(300, 30, 0xffffff, "State:Attack");
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + attacksize_.x, pos_.y + attacksize_.y, playerImage6_, true);
		break;
	case State::Death:
		DrawFormatString(300, 30, 0xffffff, "State:Attack");
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + attacksize_.x, pos_.y + attacksize_.y, playerImage7_, true);
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

	DrawFormatString(48, 600, 0xffff00, "playerPosX%f,playerPosY%f", pos_.x, pos_.y);
	DrawFormatString(pos_.x + size_.x / 2 - 40, pos_.y - 20, 0xffff00, "�v���C���[", true);


	//����
	//DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, 0xffffff, false);

	//DrawBox(ball_.pos_.x, ball_.pos_.y, ball_.pos_.x + ball_.size_.x, ball_.pos_.y + ball_.size_.y, 0xffff00, false);

#endif //_DEBUG

}

void Player::Release(void)
{
}

//�X�e�[�W�Ƃ̂����蔻��
bool Player::IsStageHit(Line collRay)
{
	//���C�̃f�o�b�N�\��
	//_dbgDrawLine(collRay.p.x, collRay.p.y, collRay.end.x, collRay.end.y, 0xff0000);
	raycast_.setPlayerRay(collRay);

	//tmx��CollLiset�擾
	for (auto& coll : tmxObj_.GetStageCollList())
	{	
		if (raycast_.StageToPlayerCheckColl(coll, offset_))
		{
			return true;
		}	
	}
	return false;
}

bool Player::IsBallHit()
{
	raycast_.setPlayerSquareRay(pos_, size_, movePos_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->size_);

	if (raycast_.PlayerToBallChackColl(offset_))
	{
		return true;
	}

	return false;
}

bool Player::IsAttackHit()
{
	raycast_.setPlayerAttackRay(attackpos_, size_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->size_);

	if (raycast_.AttackToBallCheckColl(refDir_))
	{
    		return true;
	}

	return false;
}




