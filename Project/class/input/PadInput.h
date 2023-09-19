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

    //�R���g���[���̎��
    ControllerType cType_;

    //������
    void Init(ControllerType type);

    //�R���g���[���f�[�^���擾
    int GetControllerData(ControllerType type);

};

