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

	//コントローラーの種類を取得
	virtual ControllerType GetControllerType(void) = 0;

	//コントローラーのデータを取得
	const  ControllerDate& GetControllerData(void)
	{
		return controllerData_;
	}

	//指定したキーIDが押されたかどうか？(長押し不可)
	bool ChaeckInputKey(KeyID id);

	//こっちは押している間を判定可能
	bool ChaeckLongInputKey(KeyID id);

private:

protected:
	ControllerDate controllerData_;

};

