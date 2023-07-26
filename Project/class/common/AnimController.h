#pragma once

enum class PlayerType;

enum class Anim{
	Idle,
	Run,
	JumpUp,
	Fall,
	Attack,
	Death,
	Max
};

class AnimController
{
public:
	AnimController();
	~AnimController();

	void Init(void);
	int Update();

	void SetAnim(Anim animType);

private:

	//�A�j���[�V�����̍Đ�����
	float animCount_;

	Anim anim_;

	PlayerType pType_;

};

