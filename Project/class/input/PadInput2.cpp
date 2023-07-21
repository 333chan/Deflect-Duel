#include "DxLib.h"
#include "PadInput2.h"

PadInput2::PadInput2()
{
	inputTable_ = {
		{KeyID::Up,		PAD_INPUT_1},
		{KeyID::Down,	PAD_INPUT_DOWN},
		{KeyID::Left,	PAD_INPUT_LEFT},
		{KeyID::Right,	PAD_INPUT_RIGHT},
		{KeyID::Decide,	PAD_INPUT_2},
		{KeyID::Cancel,PAD_INPUT_5},
		{KeyID::Attack,	PAD_INPUT_3},
		{KeyID::Jump,	PAD_INPUT_UP},
		{KeyID::Transition,	PAD_INPUT_5},
		{KeyID::Stage1,		KEY_INPUT_1},
		{KeyID::Rematch,	KEY_INPUT_2},
		{KeyID::ReturnSelect, KEY_INPUT_5},
		{KeyID::Max , DX_INPUT_PAD1}
	};
	for (auto data : inputTable_)
	{
		controllerData_.try_emplace(data.first, AgeBool{ 0,0 });
	}
	Update();
}

PadInput2::~PadInput2()
{
}

void PadInput2::Update(void)
{
	auto PadData = GetJoypadInputState(DX_INPUT_PAD2);
	for (auto id : KeyID())
	{
		controllerData_[id][static_cast<int>(ControllerAge::Old)] =
			controllerData_[id][static_cast<int>(ControllerAge::Now)];

		controllerData_[id][static_cast<int>(ControllerAge::Now)] =
			(PadData & inputTable_[id]) > 0;
	}

}
