#include <DxLib.h>
#include "KeyInput.h"

constexpr int KEY_INPUT_SINGLE_MAX = 32;


KeyInput::KeyInput()
{
	inputTable_ = {
		{KeyID::Up,			KEY_INPUT_W},
		{KeyID::Down,		KEY_INPUT_S},
		{KeyID::Left,		KEY_INPUT_A},
		{KeyID::Right,		KEY_INPUT_D},

		{KeyID::Decide,		KEY_INPUT_RETURN},
		{KeyID::Canacel,	KEY_INPUT_ESCAPE},
		{KeyID::Attack,		KEY_INPUT_RSHIFT},
		{KeyID::Jump,		KEY_INPUT_SPACE}
	};

	for (const auto&data:inputTable_)
	{
		controllerData_.try_emplace(data.first, AgeBool{ 0,0 });
	}
	Update();
}

KeyInput::~KeyInput()
{
}

void KeyInput::Update(void)
{
	GetHitKeyStateAll(keyData_.data());
	for (auto id : KeyID())
	{
		controllerData_[id][static_cast<int>(ControllerAge::Old)] =
			controllerData_[id][static_cast<int>(ControllerAge::Now)];

		controllerData_[id][static_cast<int>(ControllerAge::Now)] =
			keyData_[inputTable_[id]];

	}

}
