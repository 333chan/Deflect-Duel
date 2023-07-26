#include "AnimController.h"

AnimController::AnimController()
{
	Init();
}

AnimController::~AnimController()
{
}

void AnimController::Init(void)
{
	animCount_ = 0.0f;
}

int AnimController::Update()
{
	switch (anim_)
	{
	case Anim::Idle:

		if (animCount_ > 5)
		{
			animCount_ = 0;
		}
		else
		{
			animCount_ += 0.10;
		}
		break;
	case Anim::Run:
		if (animCount_ > 7)
		{
			animCount_ = 0;
		}
		else
		{
			animCount_ += 0.25;
		}
		break;
	case Anim::JumpUp:
		animCount_ += 0.15;
		if (animCount_ > 4)
		{
			animCount_ = 4;
		}
		break;
	case Anim::Fall:
		animCount_ += 0.2;
		if (animCount_ > 3)
		{
			animCount_ = 2;
		}
		break;
	case Anim::Attack:
		animCount_ += 0.5;
		if (animCount_ > 15)
		{
			animCount_ = 0;
		}
		break;
	case Anim::Death:
		break;
	case Anim::Max:
		break;
	default:
		break;
	}

	return animCount_;

}

void AnimController::SetAnim(Anim animType)
{
	if (anim_!=animType)
	{
		anim_ = animType;
		animCount_ = 0.0f;
	}


}
