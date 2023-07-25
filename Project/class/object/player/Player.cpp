#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../common/SoundManager.h"
#include "../../common/ImageManager.h"
#include "../../common/AnimController.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../../input/PadInput2.h"
#include "../../tmx/TmxObj.h"
#include"../../../_debug/_DebugDispOut.h"

constexpr int MOVE_SPEED = 5.0f;		// �ړ����x
constexpr int JUMP_POW = 15.0f;		// �W�����v��
constexpr float FALL_ACCEL = 1.0f;	// �d�͉����x

Player::Player(ControllerType type, playerType pType, std::shared_ptr<Ball>& ball)
{
	//�R���g���[���[�̐���
	if (type == ControllerType::Pad1)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else if(type == ControllerType::Pad2)
	{
		
		controller_ = std::make_unique<PadInput2>();
	}
	else if (type == ControllerType::Key)
	{
		controller_ = std::make_unique<KeyInput>();
	}


	//�v���C���[�̎�ޏ��
	playertype_ = pType;

	//�{�[�����
	ball_ = ball;

	//������
	Init();

}

Player::~Player()
{
	Release();
}

void Player::Init()
{

	animController_ = std::make_unique<AnimController>();

	//�v���C���[���W
	if (playertype_ == playerType::One)
	{
		//1P
		pos_ = { 100,450 };
	}
	else if (playertype_ == playerType::Two)
	{
		//2P
		pos_ = { 900,450 };
	}

	//�v���C���[�T�C�Y
	size_ = {48,96};

	//�U�����̉摜�T�C�Y
	attacksize_ = {96,96};

	//�{�[�����
	ballpos_ = {0,0};
	ballsize_ = { 0,0 };

	//���
	state_ = State::Idle;

	//����
	dir_ = Dir::Max;

	//�d��
	gravity_ = 0.1;

	//�␳����
	offset_ = { 0.0f ,0.0f };

	//���˕���
	refDir_ = { 0.0f ,0.0f };

	//�o�ߎ���
	jumpDeltaTime_ = 0.0;

	//�v���C���[�摜(�������)
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

	animController_->SetAnim(Anim::Idle);

	isGround = false;
}

void Player::Update(void)
{
	controller_->Update();

	switch (state_)
	{
	case State::Idle:
	{
		gravity_ = 0;

		if (!IsStageHit(Line({ pos_.x + size_.x / 2, pos_.y + size_.y / 2 }, { pos_.x + size_.x / 2,pos_.y + size_.y })))
		{
			//�X�e�[�W�ɓ������Ă��Ȃ��Ȃ�
			jumpDeltaTime_ = 1.3;
			gravity_ = 7.8;
			state_ = State::Fall;
			break;
		}
		else
		{
			isGround = true;
		}

		//�v���C���[�ړ�
		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;

			PlaySoundMem(lpSoundMng.GetID("jumpSe"), DX_PLAYTYPE_BACK);
			state_ = State::JumpUp;
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
			dir_ = Dir::Left;
			state_ = State::MoveLeft;
		}
		else if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			dir_ = Dir::Right;
			state_ = State::MoveRight;
		}
		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			//�U��
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("attackSe"));
			PlaySoundMem(lpSoundMng.GetID("attackSe"), DX_PLAYTYPE_BACK);
			state_ = State::Attack;
		}

	}
	break;
	case State::JumpUp:
	{

		auto YVel = -JUMP_POW + (2.0f * gravity_ * std::pow(jumpDeltaTime_, 2.0));

		if (YVel > 0)
		{
			state_ = State::Idle;
		}

		gravity_ += FALL_ACCEL;
		jumpDeltaTime_ += lpSceneMng.GetDeltaTime();
	
		pos_.y += YVel;

		if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x / 2,pos_.y })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			dir_ = Dir::Right;
			pos_.x += MOVE_SPEED;
			if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
			

		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//���ړ�
			dir_ = Dir::Left;
			pos_.x -= MOVE_SPEED;
			if (IsStageHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
		}

 		if (controller_->ChaeckLongInputKey(KeyID::Attack))
		{
			//�U��
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("attackSe"));
			PlaySoundMem(lpSoundMng.GetID("attackSe"), DX_PLAYTYPE_BACK);
			state_ = State::Attack;
		}

	}
	break;
	case State::Fall:
	{
		jumpDeltaTime_ += lpSceneMng.GetDeltaTime();
		gravity_ += FALL_ACCEL;

		auto YVel = -JUMP_POW + (gravity_ * std::pow(jumpDeltaTime_, 2.0));
		pos_.y += YVel;

		if (IsStageHit(Line({ pos_.x + size_.x / 2, pos_.y + size_.y / 2 }, { pos_.x + size_.x / 2,pos_.y + size_.y })))
		{
			//�������Ă���␳
			pos_ -= offset_;
			
			state_ = State::Idle;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			dir_ = Dir::Right;
			pos_.x += MOVE_SPEED;
			if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//���ړ�
			dir_ = Dir::Left;
			pos_.x -= MOVE_SPEED;
			if (IsStageHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
			{
				//�������Ă���␳
				pos_ -= offset_;
			}
		}

		if (controller_->ChaeckLongInputKey(KeyID::Attack))
		{
			//�U��
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("attackSe"));
			PlaySoundMem(lpSoundMng.GetID("attackSe"), DX_PLAYTYPE_BACK);
			state_ = State::Attack;
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
			//�L�[���������Idel
			state_ = State::Idle;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Attack))
		{
			//�U��
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("attackSe"));
			PlaySoundMem(lpSoundMng.GetID("attackSe"), DX_PLAYTYPE_BACK);
			state_ = State::Attack;
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
			//�L�[���������Idel
			state_ = State::Idle;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Attack))
		{

			//�U��
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("attackSe"));
			PlaySoundMem(lpSoundMng.GetID("attackSe"), DX_PLAYTYPE_BACK);
			state_ = State::Attack;
		}
	}
		break;

	case State::Crouching:

		//���Ⴊ��
		if (!controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//�L�[�𗣂�����
			//state_ = State::Idel;
		}
		break;

	case State::Attack:
		if(IsAttackHit())
		{

			ball_->SetAttackRef(refDir_);
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Attack))
		{
			//�L�[���������
			state_ = State::Idle;
		}

		break;
	case State::Death:

		break;
	case State::Max:
		break;
	[[likery]]default:
		break;
	}

	if (IsBallHit())
	{
		ChangeVolumeSoundMem(180, lpSoundMng.GetID("daethSe"));
		PlaySoundMem(lpSoundMng.GetID("daethSe"), DX_PLAYTYPE_BACK);
		state_ = State::Death;
	}

	if (dir_ == Dir::Left)
	{
		//�������Ă���
		attackpos_ = { pos_.x - size_.x,pos_.y };
	}
	else if(dir_ == Dir::Right)
	{
		//�E�����Ă���
		attackpos_ = { pos_.x + size_.x,pos_.y };
	}


}

void Player::Draw(void)
{
	//�v���C���[�̕`��
	switch (state_)
	{
	case State::Idle:	//����
		
		animController_->SetAnim(Anim::Idle);

		if (dir_ == Dir::Left)
		{
			DrawExtendGraph(
				pos_.x + size_.x, pos_.y,
				pos_.x, pos_.y + size_.y,
				lpImageMng.GetID("idle")[animController_->Update()], true);
			break;
		}
		DrawExtendGraph(
			pos_.x ,pos_.y,
			pos_.x + size_.x, pos_.y + size_.y,
			lpImageMng.GetID("idle")[animController_->Update()], true);

		_dbgDrawFormatString(pos_.x, pos_.y-40, 0xffffff, "Idel", true);
		break;
	case State::JumpUp:	//�W�����v�㏸
		animController_->SetAnim(Anim::JumpUp);

		if (dir_ == Dir::Left)
		{
			DrawExtendGraph(pos_.x + size_.x, pos_.y, pos_.x, pos_.y + size_.y, lpImageMng.GetID("jumpUp")[animController_->Update()], true);
			break;
		}
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, lpImageMng.GetID("jumpUp")[animController_->Update()], true);

		if (dir_ == Dir::AirAttackLeft)
		{
			DrawExtendGraph(pos_.x + size_.x, pos_.y, pos_.x, pos_.y + size_.y, playerImage4_, true);
			break;
		}
		else if(dir_ == Dir::AirAttackRight)
		{
			DrawExtendGraph(pos_.x + size_.x, pos_.y, pos_.x, pos_.y + size_.y, playerImage4_, true);
			break;
		}
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "JumpUp", true);
		break;
	case State::Fall:	//����
		animController_->SetAnim(Anim::Fall);
		if (dir_ == Dir::Left)
		{
			DrawExtendGraph(pos_.x + size_.x, pos_.y, pos_.x, pos_.y + size_.y, lpImageMng.GetID("down")[animController_->Update()], true);
			break;
		}
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, lpImageMng.GetID("down")[animController_->Update()], true);
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Fall", true);
		break;
	case State::MoveLeft://���ړ�
		animController_->SetAnim(Anim::Run);
		DrawExtendGraph(pos_.x + size_.x+5, pos_.y, pos_.x, pos_.y + size_.y, lpImageMng.GetID("run")[animController_->Update()], true);
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Left", true);
		break;
	case State::MoveRight://�E�ړ�
		animController_->SetAnim(Anim::Run);
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x+5, pos_.y + size_.y, lpImageMng.GetID("run")[animController_->Update()], true);
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Right", true);
		break;
	case State::Crouching:
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Crouching", true);
		break;
	case State::Attack://�U��
		if (dir_ == Dir::Left)
		{
			DrawExtendGraph(pos_.x + size_.x, pos_.y, pos_.x- size_.x, pos_.y + attacksize_.y, playerImage6_, true);
			break;
		}
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + attacksize_.x, pos_.y + attacksize_.y, playerImage6_, true);

		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Attack", true);
		break;
	case State::Death://��
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + attacksize_.x+30, pos_.y + attacksize_.y, playerImage7_, true);

		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Death", true);
		break;
	case State::Max:
		break;
	default:
		break;
	}

	//�v���C���[�̖��O
	if (playertype_ == playerType::One)
	{
		DrawFormatString(pos_.x + size_.x / 2-10, pos_.y - 20, 0xffff00, "1P", true);
	}
	else if (playertype_ == playerType::Two)
	{
		DrawFormatString(pos_.x + size_.x / 2-10, pos_.y - 20, 0xff0000, "2P", true);
	}

	//�������
	DrawString(50, 625, "Player1\n����\nA/D�ō��E�ړ�\nW�ŃW�����v\nSPACE�ōU��", 0xfff00f, true);
	DrawString(1100, 625, "Player2\n����\n�E/���ō��E�ړ�\nB�ŃW�����v\nX�ōU��", 0xff0000, true);

#ifdef _DEBUG	//�f�o�b�N���̂�

	//�v���C���[
	if (playertype_ == playerType::One)
	{
		DrawFormatString(48, 600, 0xffff00, "player1PosX%f,player1PosY%f", pos_.x, pos_.y);
	}
	else if (playertype_ == playerType::Two)
	{
		DrawFormatString(800, 600, 0xff0000, "player2PosX%f,player2PosY%f", pos_.x, pos_.y);
	}
#endif //_DEBUG

}

void Player::Release(void)
{

}

State Player::GetState(void)
{
	return state_;
}

playerType Player::GetPlayerType(void)
{
	return playertype_;
}

//�X�e�[�W�Ƃ̂����蔻��
bool Player::IsStageHit(Line collRay)
{
	//���C�̃f�o�b�N�\��
	_dbgDrawLine(collRay.p.x, collRay.p.y, collRay.end.x, collRay.end.y, 0xff0000);

	//�v���C���[�̃��C���Z�b�g
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
	//��`���C�̃Z�b�g
	raycast_.setPlayerSquareRay(pos_, size_, movePos_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->size_);

	//�v���C���[�ƃ{�[���̐ڐG����
	if (raycast_.PlayerToBallChackColl(offset_))
	{
		return true;
	}

	return false;
}

bool Player::IsAttackHit()
{
	//��`���C�̃Z�b�g
	raycast_.setPlayerAttackRay(attackpos_, size_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->size_);

	//�U���ƃ{�[���̐ڐG����
	if (raycast_.AttackToBallCheckColl(refDir_))
	{
    		return true;
	}

	return false;
}




