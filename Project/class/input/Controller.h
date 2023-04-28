#pragma once
#include<array>
#include <unordered_map>
#include"KeyID.h"

enum  class ControllerType
{
	Key,
	Pad,
	Max
};

enum  class ControllerAge
{
	Now,
	Old,
	Max
};

using AgeBool = std::array<bool, static_cast<size_t>(ControllerAge::Max)>;
using ControllerDate = std::unordered_map<KeyID, AgeBool>;

class Controller
{
public:
	Controller();
	virtual ~Controller();
	virtual void Update() = 0;

	//�R���g���[���[�̎�ނ��擾
	virtual ControllerType GetControllerType(void) = 0;

	//�R���g���[���[�̃f�[�^���擾
	const  ControllerDate& GetControllerData(void)
	{
		return controllerData_;
	}

	//�w�肵���L�[ID�������ꂽ���ǂ����H(�������s��)
	bool ChaeckInputKey(KeyID id);

	//�������͉����Ă���Ԃ𔻒�\
	bool ChaeckLongInputKey(KeyID id);

private:

protected:
	ControllerDate controllerData_;

};

