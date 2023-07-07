#pragma once


//入力キーの種類
enum class KeyID
{
	Up,		//上
	Down,	//下
	Left,	//左
	Right,	//右
	Decide,	//決定
	Cancel,//キャンセル
	Attack, //攻撃
	Jump,	//ジャンプ

	Transition,	//シーン遷移

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