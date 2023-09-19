#pragma once
#include "Controller.h"
class PadInput :
    public Controller
{
public:

    PadInput();
    PadInput(ControllerType type);
    ~PadInput();

    void Update(void) override;
    ControllerType GetControllerType(void) override 
    {
        return ControllerType::Pad1;
    }
private:

    std::unordered_map<KeyID, unsigned int> inputTable_;

    //コントローラの種類
    ControllerType cType_;

    //初期化
    void Init(ControllerType type);

    //コントローラデータを取得
    int GetControllerData(ControllerType type);

};

