#include "Player.h"
#include "../../scene/SceneManager.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../../tmx/TmxObj.h"
#include"../../../_debug/_DebugDispOut.h"

constexpr int MOVE_SPEED = 10;		// �ړ����x
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

	//����
	dir_ = Dir::Max;

	//�d��
	gravity_ = 0.1;

	//�␳����
	offset_ = { 0.0f ,0.0f };

	//tmx�̓ǂݍ���
	tmxObj_.LoadTmx("resource/tmx/Stage.tmx",false);

}

void Player::Update(void)
{
	controller_->Update();

	//�ȈՓI�ȏd�͌v�Z
	if (!IsHit())
	{
		//�������ĂȂ�������
		pos_.y += (FALL_SPEED + gravity_);
		gravity_ += FALL_ACCEL;
	}

	if(IsHit())
	{
		//�������Ă���␳
		pos_ -= offset_;	
		gravity_ = 0;
	}

	dir_ = Dir::Max;

	//�v���C���[�ړ�
	if (controller_->ChaeckLongInputKey(KeyID::Up))
	{
		pos_.y -= MOVE_SPEED;
		dir_ = Dir::Up;
	}
	if (controller_->ChaeckLongInputKey(KeyID::Down))
	{
		pos_.y += MOVE_SPEED;
		dir_ = Dir::Down;
		
	}
	if (controller_->ChaeckLongInputKey(KeyID::Left))
	{
		pos_.x -= MOVE_SPEED;
		dir_ = Dir::Left;
	}
	if (controller_->ChaeckLongInputKey(KeyID::Right))
	{
		pos_.x += MOVE_SPEED;
		dir_ = Dir::Right;
	}
	

}

void Player::Draw(void)
{
	//�F
	color = GetColor(0, 255, 0);

	//�v���C���[�`��
	DrawBox(pos_.x, pos_.y, pos_.x+ size_.x, pos_.y+ size_.y,0xffffff,true);

	
	if (IsHit())
	{
		//����������ԐF�ɕύX
		color = GetColor(255,0,0);
	}
	else
	{
		color = GetColor(000, 255, 000);
	}

	//�v���C��
	DrawLine(playerLine_.p.x, playerLine_.p.y, playerLine_.end.x, playerLine_.end.y, color, true);

	switch (dir_)
	{
	case Dir::Up:
		DrawFormatString(200, 0, 0xffffff, "Up");
		break;
	case Dir::Down:
		DrawFormatString(200, 0, 0xffffff, "Down");
		DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y/2, 0xffff00, true);
		break;
	case Dir::Left:
		DrawFormatString(200, 0, 0xffffff, "Left");
		break;
	case Dir::Right:
		DrawFormatString(200, 0, 0xffffff, "Right");
		break;
	default:
		break;
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
		
		if (raycast_.CheckCollision(pos_, size_, coll, dir_, offset_,color))
		{
			return true;
		}
	}

	return false;


}
