#pragma once
#include "Controller.h"

class KeyInput :
    public Controller
{
public:
    KeyInput();
    ~KeyInput();


    void Update(void) override;
    ControllerType GetControllerType(void) override
    {
        return ControllerType::Key;
    };


private:
    std::array<char, 256U> keyData_;
    std::unordered_map<KeyID,unsigned int>inputTable_;

};

