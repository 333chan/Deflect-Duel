#pragma once
#include "Controller.h"

class PadInput2 :
    public Controller
{
public:
    PadInput2();
    ~PadInput2();

    void Update(void) override;
    ControllerType GetControllerType(void) override
    {
        return ControllerType::Pad2;
    }
private:
    std::unordered_map<KeyID, unsigned int> inputTable_;

};

