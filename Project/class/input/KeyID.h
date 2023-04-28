#pragma once


//���̓L�[�̎��
enum class KeyID
{
	Up,		//��
	Down,	//��
	Left,	//��
	Right,	//�E
	Decide,	//����
	Canacel,//�L�����Z��
	Attack, //�U��
	Jump,	//�W�����v
	Max
};

static KeyID begin(KeyID) {
	return KeyID::Up;
};

static KeyID end(KeyID) {
	return KeyID::Max;
};

static KeyID operator++(KeyID& state) {
	return state = static_cast<KeyID>(std::underlying_type<KeyID>::type(state) + 1);
};

static KeyID operator*(const KeyID& state) {
	return state;
};