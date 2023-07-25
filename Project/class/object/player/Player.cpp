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

constexpr int MOVE_SPEED = 5.0f;		// 移動速度
constexpr int JUMP_POW = 15.0f;		// ジャンプ力
constexpr float FALL_ACCEL = 1.0f;	// 重力加速度

Player::Player(ControllerType type, playerType pType, std::shared_ptr<Ball>& ball)
{
	//コントローラーの生成
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

	//プレイヤーサイズ
	size_ = {48,96};

	//攻撃時の画像サイズ
	attacksize_ = {96,96};

	//ボール情報
	ballpos_ = {0,0};
	ballsize_ = { 0,0 };

	//状態
	state_ = State::Idle;

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

	//プレイヤー画像(無理やり)
	playerImage_ = LoadGraph("resource/image/character/player_idle.png", true);
	playerImage2_ = LoadGraph("resource/image/character/player_move.png", true);
	playerImage3_ = LoadGraph("resource/image/character/Player_Crouch.png", true);
	playerImage4_ = LoadGraph("resource/image/character/player_up.png", true);
	playerImage5_ = LoadGraph("resource/image/character/player_down.png", true);
	playerImage6_ = LoadGraph("resource/image/character/player_attack.png", true);
	playerImage7_ = LoadGraph("resource/image/character/player_death.png", true);

	//tmxの読み込み
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
			//ステージに当たっていないなら
			jumpDeltaTime_ = 1.3;
			gravity_ = 7.8;
			state_ = State::Fall;
			break;
		}
		else
		{
			isGround = true;
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
			//state_ = State::Crouching;
			dir_ = Dir::Down;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//左移動
			dir_ = Dir::Left;
			state_ = State::MoveLeft;
		}
		else if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//右移動
			dir_ = Dir::Right;
			state_ = State::MoveRight;
		}
		if (controller_->ChaeckInputKey(KeyID::Attack))
		{
			//攻撃
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
			//当たってたら補正
			pos_ -= offset_;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//右移動
			dir_ = Dir::Right;
			pos_.x += MOVE_SPEED;
			if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
			{
				//当たってたら補正
				pos_ -= offset_;
			}
			

		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//左移動
			dir_ = Dir::Left;
			pos_.x -= MOVE_SPEED;
			if (IsStageHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
			{
				//当たってたら補正
				pos_ -= offset_;
			}
		}

 		if (controller_->ChaeckLongInputKey(KeyID::Attack))
		{
			//攻撃
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
			//当たってたら補正
			pos_ -= offset_;
			
			state_ = State::Idle;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//右移動
			dir_ = Dir::Right;
			pos_.x += MOVE_SPEED;
			if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
			{
				//当たってたら補正
				pos_ -= offset_;
			}
		}
		if (controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//左移動
			dir_ = Dir::Left;
			pos_.x -= MOVE_SPEED;
			if (IsStageHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
			{
				//当たってたら補正
				pos_ -= offset_;
			}
		}

		if (controller_->ChaeckLongInputKey(KeyID::Attack))
		{
			//攻撃
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("attackSe"));
			PlaySoundMem(lpSoundMng.GetID("attackSe"), DX_PLAYTYPE_BACK);
			state_ = State::Attack;
		}


	}
	break;
	case State::MoveLeft:
	{
		//左移動
		pos_.x -= MOVE_SPEED;

		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//ジャンプ
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			state_ = State::JumpUp;
		}

		if (IsStageHit(Line({ {pos_.x + size_.x / 2,pos_.y + size_.y / 2},{pos_.x,pos_.y + size_.y / 2} })))
		{
			//当たってたら補正
			pos_ -= offset_;
		}


		if (!controller_->ChaeckLongInputKey(KeyID::Left))
		{
			//キーを放したらIdel
			state_ = State::Idle;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Attack))
		{
			//攻撃
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("attackSe"));
			PlaySoundMem(lpSoundMng.GetID("attackSe"), DX_PLAYTYPE_BACK);
			state_ = State::Attack;
		}
	}
		break;
	case State::MoveRight:
	{
		//右移動
		pos_.x += MOVE_SPEED;

		if (controller_->ChaeckInputKey(KeyID::Up))
		{
			//ジャンプ
			gravity_ = 0;
			jumpDeltaTime_ = 0.0;
			state_ = State::JumpUp;
		}

		if (IsStageHit(Line({ pos_.x + size_.x / 2,pos_.y + size_.y / 2 }, { pos_.x + size_.x ,pos_.y + size_.y / 2 })))
		{
			//当たってたら補正
			pos_ -= offset_;
		}

		if (!controller_->ChaeckLongInputKey(KeyID::Right))
		{
			//キーを放したらIdel
			state_ = State::Idle;
		}

		if (controller_->ChaeckLongInputKey(KeyID::Attack))
		{

			//攻撃
			ChangeVolumeSoundMem(150, lpSoundMng.GetID("attackSe"));
			PlaySoundMem(lpSoundMng.GetID("attackSe"), DX_PLAYTYPE_BACK);
			state_ = State::Attack;
		}
	}
		break;

	case State::Crouching:

		//しゃがみ
		if (!controller_->ChaeckLongInputKey(KeyID::Down))
		{
			//キーを離したら
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
			//キーを放したら
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
		//左向いてたら
		attackpos_ = { pos_.x - size_.x,pos_.y };
	}
	else if(dir_ == Dir::Right)
	{
		//右向いてたら
		attackpos_ = { pos_.x + size_.x,pos_.y };
	}


}

void Player::Draw(void)
{
	//プレイヤーの描画
	switch (state_)
	{
	case State::Idle:	//立ち
		
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
	case State::JumpUp:	//ジャンプ上昇
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
	case State::Fall:	//落下
		animController_->SetAnim(Anim::Fall);
		if (dir_ == Dir::Left)
		{
			DrawExtendGraph(pos_.x + size_.x, pos_.y, pos_.x, pos_.y + size_.y, lpImageMng.GetID("down")[animController_->Update()], true);
			break;
		}
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, lpImageMng.GetID("down")[animController_->Update()], true);
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Fall", true);
		break;
	case State::MoveLeft://左移動
		animController_->SetAnim(Anim::Run);
		DrawExtendGraph(pos_.x + size_.x+5, pos_.y, pos_.x, pos_.y + size_.y, lpImageMng.GetID("run")[animController_->Update()], true);
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Left", true);
		break;
	case State::MoveRight://右移動
		animController_->SetAnim(Anim::Run);
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + size_.x+5, pos_.y + size_.y, lpImageMng.GetID("run")[animController_->Update()], true);
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Right", true);
		break;
	case State::Crouching:
		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Crouching", true);
		break;
	case State::Attack://攻撃
		if (dir_ == Dir::Left)
		{
			DrawExtendGraph(pos_.x + size_.x, pos_.y, pos_.x- size_.x, pos_.y + attacksize_.y, playerImage6_, true);
			break;
		}
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + attacksize_.x, pos_.y + attacksize_.y, playerImage6_, true);

		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Attack", true);
		break;
	case State::Death://死
		DrawExtendGraph(pos_.x, pos_.y, pos_.x + attacksize_.x+30, pos_.y + attacksize_.y, playerImage7_, true);

		_dbgDrawFormatString(pos_.x, pos_.y - 40, 0xffffff, "Death", true);
		break;
	case State::Max:
		break;
	default:
		break;
	}

	//プレイヤーの名前
	if (playertype_ == playerType::One)
	{
		DrawFormatString(pos_.x + size_.x / 2-10, pos_.y - 20, 0xffff00, "1P", true);
	}
	else if (playertype_ == playerType::Two)
	{
		DrawFormatString(pos_.x + size_.x / 2-10, pos_.y - 20, 0xff0000, "2P", true);
	}

	//操作説明
	DrawString(50, 625, "Player1\n操作\nA/Dで左右移動\nWでジャンプ\nSPACEで攻撃", 0xfff00f, true);
	DrawString(1100, 625, "Player2\n操作\n右/左で左右移動\nBでジャンプ\nXで攻撃", 0xff0000, true);

#ifdef _DEBUG	//デバック時のみ

	//プレイヤー
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
	raycast_.setPlayerSquareRay(pos_, size_, movePos_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->size_);

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
	raycast_.setPlayerAttackRay(attackpos_, size_);
	raycast_.setBallRay(ball_->pos_+ ball_->movePos_, ball_->size_);

	//攻撃とボールの接触判定
	if (raycast_.AttackToBallCheckColl(refDir_))
	{
    		return true;
	}

	return false;
}




