#include "DxLib.h"
#include "PadInput.h"

PadInput::PadInput()
{
	Init(ControllerType::Pad1);
}

PadInput::PadInput(ControllerType type)
{
	Init(type);
}

PadInput::~PadInput()
{
}

void PadInput::Update(void)
{
	auto PadData = GetControllerData(cType_);//GetJoypadInputState(DX_INPUT_PAD1);
	for (auto id : KeyID())
	{
			controllerData_[id][static_cast<int>(ControllerAge::Old)] =
				controllerData_[id][static_cast<int>(ControllerAge::Now)];

			controllerData_[id][static_cast<int>(ControllerAge::Now)] =
				(PadData & inputTable_[id]) > 0;
	}

}

void PadInput::Init(ControllerType type)
{
	inputTable_ = {
	{KeyID::Up,		PAD_INPUT_1},
	{KeyID::Down,	PAD_INPUT_DOWN},
	{KeyID::Left,	PAD_INPUT_LEFT},
	{KeyID::Right,	PAD_INPUT_RIGHT},
	{KeyID::Decide,	PAD_INPUT_2},
	{KeyID::Cancel,PAD_INPUT_1},
	{KeyID::Attack,	PAD_INPUT_3},
	{KeyID::Jump,	PAD_INPUT_UP},
	{KeyID::Transition,	PAD_INPUT_4},
	{KeyID::Stage1,		PAD_INPUT_5},
	{KeyID::Rematch,	PAD_INPUT_2},
	{KeyID::ReturnSelect, PAD_INPUT_4},
	{KeyID::All ,PAD_INPUT_2 }
	};
	for (auto data : inputTable_)
	{
		controllerData_.try_emplace(data.first, AgeBool{ 0,0 });
	}

	cType_ = type;
	Update();
}

int PadInput::GetControllerData(ControllerType type)
{
	if (type == ControllerType::Pad1)
	{
		return GetJoypadInputState(DX_INPUT_PAD1);
	}
	else if (type == ControllerType::Pad2)
	{
		return GetJoypadInputState(DX_INPUT_PAD2);
	}
	else if (type == ControllerType::Pad3)
	{
		return GetJoypadInputState(DX_INPUT_PAD3);
	}
	else if (type == ControllerType::Pad4)
	{
		return GetJoypadInputState(DX_INPUT_PAD4);
	}

	return -1;
}
