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

constexpr int MOVE_SPEED = 15.0f;		// �ړ����x
constexpr int JUMP_POW = 16.0f;		// �W�����v��
constexpr float FALL_ACCEL = 2.0f;	// �d�͉����x

constexpr int DRAW_OFFSET = 12;	//�`��␳
constexpr float DRAW_EXRATE = 2.25f;//�g�嗦

Player::Player(ControllerType type, PlayerType pType, std::shared_ptr<Ball>& ball)
{

	controller_ = SetControllerType(type);


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
	if (playertype_ == PlayerType::One)
	{
		//1P
		pos_ = { 100,450 };

		//�v���C���[�T�C�Y
		collSize_ = { 64,96 };
		DrawSize_ = { 18,66 };

		reverse_ = 0;
	}
	else if (playertype_ == PlayerType::Two)
	{
		//2P
		pos_ = { 1100,450 };

		//�v���C���[�T�C�Y
		collSize_ = { 64,96 };
		DrawSize_ = { 18,66 };

		//������
		reverse_ = -1;
	} 

	//�U�����̃T�C�Y
	attacksize_ = {48,96};

	GetGraphSizeF(lpImageMng.GetID("knight_attack")[0],&imageSize_.x, &imageSize_.y);

	//�{�[�����
	ballpos_ = {0,0};
	ballsize_ = { 0,0 };

	//���
	state_ = State::Idle;
	animController_->SetAnim(Anim::Fall);

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

	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx", false);
	movePos_ = { MOVE_SPEED , MOVE_SPEED };



	animEnd_ = false;

	isGround = false;

}

void Player::Update(void)
{
	imagePos_ = { pos_.x + (imageSize_.x - collSize_.x) / 2 - 23,pos_.y };
	
	controller_->Update();

	switch (state_)
	{
	case State::Idle:
	{
		gravity_ = 0;

		if (!IsStageHit(Line({ pos_.x + collSize_.x / 2, pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x / 2,pos_.y + collSize_.y })))
		{
			//�X�e�[�W�ɓ������Ă��Ȃ��Ȃ�
			jumpDeltaTime_ = 1.3;
			gravity_ = 7.8;
			state_ = State::Fall;
			break;
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
			state_ = State::Crouching;
			//dir_ = Dir::Down;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//��
			state_ = State::MoveLeft;
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("walkSe"));
			PlaySoundMem(lpSoundMng.GetID("walkSe"), DX_PLAYTYPE_LOOP);
		}
		else if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E
			state_ = State::MoveRight;
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("walkSe"));
			PlaySoundMem(lpSoundMng.GetID("walkSe"), DX_PLAYTYPE_LOOP);
		}
		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			////�U��
			lpSoundMng.ResetCount("attackSe");
			lpSoundMng.PlaySoundOneTime("attackSe");  

			state_ = State::Attack;
		}

	}
	break;
	case State::JumpUp:
	{
		jumpDeltaTime_ += lpSceneMng.GetDeltaTime();
		gravity_ += FALL_ACCEL;

		yVel_ = -JUMP_POW + (gravity_ * std::pow(jumpDeltaTime_, 2.0));
		pos_.y += yVel_;

		if (yVel_ > 0&&state_==State::JumpUp)
		{
			//jumpDeltaTime_ = 1.3;
			//gravity_ = 7.8;
			state_ = State::Fall;
			break;
		}

		if (IsStageHit(Line({ pos_.x + collSize_.x / 2,pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x / 2,pos_.y })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			MovePosition(Dir::Right);
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//���ړ�
			MovePosition(Dir::Left);
		}

 		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			////�U��
			if (playertype_ == PlayerType::One)
			{
			}

			lpSoundMng.ResetCount("airAttackSe");
			lpSoundMng.PlaySoundOneTime("airAttackSe");

			animEnd_ = false;

			state_ = State::AirAttack;
		}

	}
	break;
	case State::Fall:
	{
		jumpDeltaTime_ += lpSceneMng.GetDeltaTime();
		gravity_ += FALL_ACCEL;

		yVel_ = -JUMP_POW + (gravity_ * std::pow(jumpDeltaTime_, 2.0));
		pos_.y += yVel_;

		if (IsStageHit(Line({ pos_.x + collSize_.x / 2, pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x / 2,pos_.y + collSize_.y })))
		{
			//�������Ă���␳
			pos_ -= offset_;
			state_ = State::Idle;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//�E�ړ�
			MovePosition(Dir::Right);
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//���ړ�
			MovePosition(Dir::Left);
		}

		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			////�U��
			lpSoundMng.ResetCount("airAttackSe");
			lpSoundMng.PlaySoundOneTime("airAttackSe");

			if (IsAttackHit())
			{
				ball_->SetAttackRef(refDir_);
			}

			animEnd_ = false;

			state_ = State::AirAttack;

		}


	}
	break;
	case State::MoveLeft:
	{
		//���ړ�
		MovePosition(Dir::Left);

		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			StopSoundMem(lpSoundMng.GetID("walkSe"));
			PlaySoundMem(lpSoundMng.GetID("jumpSe"), DX_PLAYTYPE_BACK);
			state_ = State::JumpUp;
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Left))
		{
			StopSoundMem(lpSoundMng.GetID("walkSe"));
			//�L�[���������Idel
			state_ = State::Idle;
		}

		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			StopSoundMem(lpSoundMng.GetID("walkSe"));

			////�U��
			lpSoundMng.ResetCount("attackSe");
			lpSoundMng.PlaySoundOneTime("attackSe");
			
			state_ = State::Attack;
		}
	}
		break;
	case State::MoveRight:
	{

		//�E�ړ�
		MovePosition(Dir::Right);

		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//�W�����v
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			StopSoundMem(lpSoundMng.GetID("walkSe"));
			PlaySoundMem(lpSoundMng.GetID("jumpSe"), DX_PLAYTYPE_BACK);
			state_ = State::JumpUp;
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Right))
		{
			StopSoundMem(lpSoundMng.GetID("walkSe"));
			//�L�[���������Idel
			state_ = State::Idle;
		}

		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			//�U��
			StopSoundMem(lpSoundMng.GetID("walkSe"));

			lpSoundMng.ResetCount("attackSe");
			lpSoundMng.PlaySoundOneTime("attackSe");

			state_ = State::Attack;
		}
	}
		break;

	case State::Crouching:

		pos_={pos_.x,pos_.y+ collSize_.y /2-20};

		//���Ⴊ��
		if (!controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//�L�[�𗣂�����
			state_ = State::Idle;
		}
		break;

	case State::Attack:




		//�A�j���[�V�������I�������
		if (animController_->SetAnimEnd(animEnd_) == true)
		{
			state_ = State::Idle;
		}
		else
		{
			if (IsAttackHit())
			{
				ball_->SetBallOwn(playertype_);
				ball_->SetAttackRef(refDir_);


				lpSoundMng.ResetCount("attackHitSe");
				ChangeVolumeSoundMem(200, lpSoundMng.GetID("attackHitSe"));
				lpSoundMng.PlaySoundOneTime("attackHitSe");
			}
		}

		break;

	case State::AirAttack:

		if (!isGround)
		{
			jumpDeltaTime_ += lpSceneMng.GetDeltaTime();
			gravity_ += FALL_ACCEL;

			yVel_ = -JUMP_POW + (gravity_ * std::pow(jumpDeltaTime_, 2.0));
			pos_.y += yVel_;
			if (IsStageHit(Line({ pos_.x + collSize_.x / 2, pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x / 2,pos_.y + collSize_.y })))
			{
				//�A�j���[�V�������I��������
				StopSoundMem(lpSoundMng.GetID("airAttackSe"));

				//�������Ă���␳
				pos_ -= offset_;
				state_ = State::Idle;
			}

		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			MovePosition(Dir::Right);
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			MovePosition(Dir::Left);
		}

		//�A�j���[�V�������I�������
		if (animController_->SetAnimEnd(animEnd_) == true)
		{
			if (IsStageHit(Line({ pos_.x + collSize_.x / 2, pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x / 2,pos_.y + collSize_.y })))
			{
				//�������Ă���␳
				pos_ -= offset_;
				state_ = State::Idle;
			}
			else
			{
				state_ = State::Fall;
			}

		}
		else
		{
			if (IsAttackHit())
			{

				lpSoundMng.ResetCount("attackHitSe");
				ChangeVolumeSoundMem(200, lpSoundMng.GetID("attackHitSe"));
				lpSoundMng.PlaySoundOneTime("attackHitSe");

				ball_->SetBallOwn(playertype_);
				ball_->SetAttackRef(refDir_);
				ball_->fastHitflg_ = true;
			}
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
	{	//�{�[���Ƃ̏Փ˔���
		if (ball_->fastHitflg_ == true)
		{
			if (playertype_ == PlayerType::Two)
			{
				if (ball_->GetBallOwn() == PlayerType::One)
				{
					ChangeVolumeSoundMem(250, lpSoundMng.GetID("daethSe"));
					PlaySoundMem(lpSoundMng.GetID("daethSe"), DX_PLAYTYPE_BACK);
					state_ = State::Death;
				}

			}

			if (playertype_ == PlayerType::One)
			{
				if (ball_->GetBallOwn() == PlayerType::Two)
				{
					ChangeVolumeSoundMem(250, lpSoundMng.GetID("daethSe"));
					PlaySoundMem(lpSoundMng.GetID("daethSe"), DX_PLAYTYPE_BACK);
					state_ = State::Death;
				}

			}


		}
	}


	if (dir_ == Dir::Left)
	{
		//�������Ă���
		reverse_ = -1;
		attackpos_ = { pos_.x ,pos_.y };
	}
	else if(dir_ == Dir::Right)
	{
		//�E�����Ă���
		reverse_ = 1;
		attackpos_ = { pos_.x + collSize_.x,pos_.y };
	}

	//�n�ʂɂ��邩
	if (IsStageHit(Line({ pos_.x + collSize_.x / 2, pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x / 2,pos_.y + collSize_.y })))
	{
		//�������Ă���␳
		pos_ -= offset_;
		isGround = true;
	}
	else
	{
		isGround = false;
	}

	if (lpSceneMng.GetScreenSize().y < pos_.y)
	{
		pos_ = { pos_.x,450 };
	}

}

void Player::Draw(void)
{

	//�v���C���[�̕`��
	switch (state_)
	{
	case State::Idle:	//����
		
		animController_->SetAnim(Anim::Idle);

		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET, 
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_idle")[animController_->Update()],
				true, -1 * reverse_);
		}
		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 -(DRAW_OFFSET - 12) * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_idle")[animController_->Update()],
				true, -1 * reverse_);
		}
		_dbgDrawFormatString(pos_.x, pos_.y-40, 0xffffff, "Idle", true);
		break;
	case State::JumpUp:	//�W�����v�㏸
		animController_->SetAnim(Anim::JumpUp);

		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_jumpUp")[animController_->Update()],
				true, -1 * reverse_);
		}

		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - (DRAW_OFFSET - 12) * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_jumpUp")[animController_->Update()],
				true, -1 * reverse_);
		}

		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "JumpUp", true);
		break;
	case State::Fall:	//����
		animController_->SetAnim(Anim::Fall);
		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_fall")[animController_->Update()],
				true, -1 * reverse_);
		}
		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - (DRAW_OFFSET - 12) * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_fall")[animController_->Update()],
				true, -1 * reverse_);
		}
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Fall", true);
		break;
	case State::MoveLeft://���ړ�
		animController_->SetAnim(Anim::Run);
		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_run")[animController_->Update()],
				true, -1 * reverse_);
		}
		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - (DRAW_OFFSET - 12) * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_run")[animController_->Update()],
				true, -1 * reverse_);
		}
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Left", true);
		break;
	case State::MoveRight://�E�ړ�
		animController_->SetAnim(Anim::Run);
		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_run")[animController_->Update()],
				true, -1 * reverse_);
		}
		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - (DRAW_OFFSET - 12) * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_run")[animController_->Update()],
				true, -1 * reverse_);
		}
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Right", true);
		break;
	case State::Crouching://���Ⴊ��
		animController_->SetAnim(Anim::Crouch);
		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET/4 * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_crouch")[animController_->Update()],
				true, -1 * reverse_);
		}
		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - (DRAW_OFFSET - 12) * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_crouch")[animController_->Update()],
				true, -1 * reverse_);
		}
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Crouching", true);
		break;

	case State::Attack://�U��
		animController_->SetAnim(Anim::Attack);

		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_attack")[animController_->Update()],
				true, -1 * reverse_);
		}
		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_attack")[animController_->Update()],
				true, -1 * reverse_);
		}
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Attack", true);
		break;

	case State::AirAttack://�󒆍U��
		animController_->SetAnim(Anim::AirAttack);
		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_airAttack")[animController_->Update()],
				true, -1 * reverse_);
		}
		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 + DRAW_OFFSET/2 * reverse_, pos_.y + collSize_.y / 2- DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_airAttack")[animController_->Update()],
				true, -1 * reverse_);
		}

		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "AirAttack", true);
		break;
	case State::Death://��
		animController_->SetAnim(Anim::Death);
		if (playertype_ == PlayerType::One)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - DRAW_OFFSET * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("knight_death")[animController_->Update()],
				true, -1 * reverse_);
		}
		if (playertype_ == PlayerType::Two)
		{
			DrawRotaGraph(
				pos_.x + collSize_.x / 2 - (DRAW_OFFSET - 12) * reverse_, pos_.y + collSize_.y / 2 + DRAW_OFFSET,
				DRAW_EXRATE,
				0,
				lpImageMng.GetID("rogue_death")[animController_->Update()],
				true, -1 * reverse_);
		}

		DrawRotaGraph(
			ball_ -> pos_.x + ball_->collSize_.x / 2, ball_->pos_.y + ball_->collSize_.y / 2,
			DRAW_EXRATE,
			0,
			lpImageMng.GetID("explosion")[0],
			true, -1 * reverse_);
		ball_->SetPlayerHit(true);

		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Death", true);
		break;
	case State::Max:
		break;
	default:
		break;
	}

	//�v���C���[�̖��O
	if (playertype_ == PlayerType::One)
	{

		DrawExtendGraph(pos_.x + collSize_.x / 2 - 10, pos_.y - 20, pos_.x + collSize_.x / 2 + 10, pos_.y-5,lpImageMng.GetID("p1Logo")[0], true);
		//DrawFormatString(pos_.x + collSize_.x / 2-10, pos_.y - 20, 0xff0000, "1P", true);
	}
	else if (playertype_ == PlayerType::Two)
	{
		DrawExtendGraph(pos_.x + collSize_.x / 2 - 10, pos_.y - 20, pos_.x + collSize_.x / 2 + 10, pos_.y - 5, lpImageMng.GetID("p2Logo")[0], true);
		//DrawFormatString(pos_.x + collSize_.x / 2-10, pos_.y - 20, 0x0000ff, "2P", true);
	}


	

#ifdef _DEBUG	//�f�o�b�N���̂�

	//�v���C���[
	if (playertype_ == PlayerType::One)
	{
		DrawFormatString(48, 600, 0xffff00, "player1PosX%f,player1PosY%f", pos_.x, pos_.y);
		DrawFormatString(50, 625 + 16 * 5, 0xff0000, "yvel:%f", yVel_);
	}
	else if (playertype_ == PlayerType::Two)
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

PlayerType Player::GetPlayerType(void)
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
	raycast_.setPlayerSquareRay(pos_, collSize_, movePos_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->collSize_);

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
	raycast_.setPlayerAttackRay(attackpos_, attacksize_,reverse_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->collSize_);

	//�U���ƃ{�[���̐ڐG����
	if (raycast_.AttackToBallCheckColl(refDir_,reverse_))
	{
    		return true;
	}

	return false;
}

void Player::MovePosition(Dir dir)
{


	if (dir == Dir::Right)
	{
		//�E�ړ�
		dir_ = dir;
		pos_.x += MOVE_SPEED;
		if (IsStageHit(Line({ pos_.x + collSize_.x / 2,pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x ,pos_.y + collSize_.y / 2 })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}
	}
	else if(dir == Dir::Left)
	{
		//���ړ�
		dir_ = dir;
		pos_.x -= MOVE_SPEED;
		if (IsStageHit(Line({ {pos_.x + collSize_.x / 2,pos_.y + collSize_.y / 2},{pos_.x,pos_.y + collSize_.y / 2} })))
		{
			//�������Ă���␳
			pos_ -= offset_;
		}
	}

}

std::unique_ptr<Controller> Player::SetControllerType(ControllerType type)
{
	//�R���g���[���[�̐���
	if (type == ControllerType::Pad1)
	{
		return std::make_unique<PadInput>(type);
	}
	else if (type == ControllerType::Pad2)
	{
		return std::make_unique<PadInput>(type);
	}

	else if (type == ControllerType::Pad3)
	{
		return std::make_unique<PadInput>(type);
	}
	else if (type == ControllerType::Pad4)
	{
		return std::make_unique<PadInput>(type);
	}
	else if (type == ControllerType::Key)
	{
		return std::make_unique<KeyInput>();
	}

	return nullptr;
}




