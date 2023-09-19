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

constexpr int MOVE_SPEED = 15.0f;		// 移動速度
constexpr int JUMP_POW = 16.0f;		// ジャンプ力
constexpr float FALL_ACCEL = 2.0f;	// 重力加速度

constexpr int DRAW_OFFSET = 12;	//描画補正
constexpr float DRAW_EXRATE = 2.25f;//拡大率

Player::Player(ControllerType type, PlayerType pType, std::shared_ptr<Ball>& ball)
{

	controller_ = SetControllerType(type);


	//プレイヤーの種類情報
	playertype_ = pType;

	//ボール情報
	ball_ = ball;

	//初期化
	Init();

}

Player::~Player()
{
	Release();
}

void Player::Init()
{

	animController_ = std::make_unique<AnimController>();

	//プレイヤー座標
	if (playertype_ == PlayerType::One)
	{
		//1P
		pos_ = { 100,450 };

		//プレイヤーサイズ
		collSize_ = { 64,96 };
		DrawSize_ = { 18,66 };

		reverse_ = 0;
	}
	else if (playertype_ == PlayerType::Two)
	{
		//2P
		pos_ = { 1100,450 };

		//プレイヤーサイズ
		collSize_ = { 64,96 };
		DrawSize_ = { 18,66 };

		//左初手
		reverse_ = -1;
	} 

	//攻撃時のサイズ
	attacksize_ = {48,96};

	GetGraphSizeF(lpImageMng.GetID("knight_attack")[0],&imageSize_.x, &imageSize_.y);

	//ボール情報
	ballpos_ = {0,0};
	ballsize_ = { 0,0 };

	//状態
	state_ = State::Idle;
	animController_->SetAnim(Anim::Fall);

	//方向
	dir_ = Dir::Max;

	//重力
	gravity_ = 0.1;

	//補正差分
	offset_ = { 0.0f ,0.0f };

	//反射方向
	refDir_ = { 0.0f ,0.0f };

	//経過時間
	jumpDeltaTime_ = 0.0;

	//tmxの読み込み
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
			//ステージに当たっていないなら
			jumpDeltaTime_ = 1.3;
			gravity_ = 7.8;
			state_ = State::Fall;
			break;
		}

		//プレイヤー移動
		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//ジャンプ
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;


			PlaySoundMem(lpSoundMng.GetID("jumpSe"), DX_PLAYTYPE_BACK);
			state_ = State::JumpUp;
			break;
		}
		if (controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//しゃがみ
			state_ = State::Crouching;
			//dir_ = Dir::Down;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//左
			state_ = State::MoveLeft;
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("walkSe"));
			PlaySoundMem(lpSoundMng.GetID("walkSe"), DX_PLAYTYPE_LOOP);
		}
		else if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//右
			state_ = State::MoveRight;
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("walkSe"));
			PlaySoundMem(lpSoundMng.GetID("walkSe"), DX_PLAYTYPE_LOOP);
		}
		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			////攻撃
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
			//当たってたら補正
			pos_ -= offset_;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//右移動
			MovePosition(Dir::Right);
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//左移動
			MovePosition(Dir::Left);
		}

 		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			////攻撃
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
			//当たってたら補正
			pos_ -= offset_;
			state_ = State::Idle;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//右移動
			MovePosition(Dir::Right);
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//左移動
			MovePosition(Dir::Left);
		}

		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			////攻撃
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
		//左移動
		MovePosition(Dir::Left);

		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//ジャンプ
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			StopSoundMem(lpSoundMng.GetID("walkSe"));
			PlaySoundMem(lpSoundMng.GetID("jumpSe"), DX_PLAYTYPE_BACK);
			state_ = State::JumpUp;
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Left))
		{
			StopSoundMem(lpSoundMng.GetID("walkSe"));
			//キーを放したらIdel
			state_ = State::Idle;
		}

		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			StopSoundMem(lpSoundMng.GetID("walkSe"));

			////攻撃
			lpSoundMng.ResetCount("attackSe");
			lpSoundMng.PlaySoundOneTime("attackSe");
			
			state_ = State::Attack;
		}
	}
		break;
	case State::MoveRight:
	{

		//右移動
		MovePosition(Dir::Right);

		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//ジャンプ
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			StopSoundMem(lpSoundMng.GetID("walkSe"));
			PlaySoundMem(lpSoundMng.GetID("jumpSe"), DX_PLAYTYPE_BACK);
			state_ = State::JumpUp;
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Right))
		{
			StopSoundMem(lpSoundMng.GetID("walkSe"));
			//キーを放したらIdel
			state_ = State::Idle;
		}

		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			//攻撃
			StopSoundMem(lpSoundMng.GetID("walkSe"));

			lpSoundMng.ResetCount("attackSe");
			lpSoundMng.PlaySoundOneTime("attackSe");

			state_ = State::Attack;
		}
	}
		break;

	case State::Crouching:

		pos_={pos_.x,pos_.y+ collSize_.y /2-20};

		//しゃがみ
		if (!controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//キーを離したら
			state_ = State::Idle;
		}
		break;

	case State::Attack:




		//アニメーションが終わったら
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
				//アニメーションが終了したら
				StopSoundMem(lpSoundMng.GetID("airAttackSe"));

				//当たってたら補正
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

		//アニメーションが終わったら
		if (animController_->SetAnimEnd(animEnd_) == true)
		{
			if (IsStageHit(Line({ pos_.x + collSize_.x / 2, pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x / 2,pos_.y + collSize_.y })))
			{
				//当たってたら補正
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
	{	//ボールとの衝突判定
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
		//左向いてたら
		reverse_ = -1;
		attackpos_ = { pos_.x ,pos_.y };
	}
	else if(dir_ == Dir::Right)
	{
		//右向いてたら
		reverse_ = 1;
		attackpos_ = { pos_.x + collSize_.x,pos_.y };
	}

	//地面にいるか
	if (IsStageHit(Line({ pos_.x + collSize_.x / 2, pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x / 2,pos_.y + collSize_.y })))
	{
		//当たってたら補正
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

	//プレイヤーの描画
	switch (state_)
	{
	case State::Idle:	//立ち
		
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
	case State::JumpUp:	//ジャンプ上昇
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
	case State::Fall:	//落下
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
	case State::MoveLeft://左移動
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
	case State::MoveRight://右移動
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
	case State::Crouching://しゃがみ
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

	case State::Attack://攻撃
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

	case State::AirAttack://空中攻撃
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
	case State::Death://死
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

	//プレイヤーの名前
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


	

#ifdef _DEBUG	//デバック時のみ

	//プレイヤー
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

//ステージとのあたり判定
bool Player::IsStageHit(Line collRay)
{
	//レイのデバック表示
	_dbgDrawLine(collRay.p.x, collRay.p.y, collRay.end.x, collRay.end.y, 0xff0000);

	//プレイヤーのレイをセット
	raycast_.setPlayerRay(collRay);

	//tmxのCollLiset取得
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
	//矩形レイのセット
	raycast_.setPlayerSquareRay(pos_, collSize_, movePos_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->collSize_);

	//プレイヤーとボールの接触判定
	if (raycast_.PlayerToBallChackColl(offset_))
	{
		return true;
	}

	return false;
}

bool Player::IsAttackHit()
{
	//矩形レイのセット
	raycast_.setPlayerAttackRay(attackpos_, attacksize_,reverse_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->collSize_);

	//攻撃とボールの接触判定
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
		//右移動
		dir_ = dir;
		pos_.x += MOVE_SPEED;
		if (IsStageHit(Line({ pos_.x + collSize_.x / 2,pos_.y + collSize_.y / 2 }, { pos_.x + collSize_.x ,pos_.y + collSize_.y / 2 })))
		{
			//当たってたら補正
			pos_ -= offset_;
		}
	}
	else if(dir == Dir::Left)
	{
		//左移動
		dir_ = dir;
		pos_.x -= MOVE_SPEED;
		if (IsStageHit(Line({ {pos_.x + collSize_.x / 2,pos_.y + collSize_.y / 2},{pos_.x,pos_.y + collSize_.y / 2} })))
		{
			//当たってたら補正
			pos_ -= offset_;
		}
	}

}

std::unique_ptr<Controller> Player::SetControllerType(ControllerType type)
{
	//コントローラーの生成
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




