#pragma once
#include "../../common/Geometry.h"
#include "../Object.h"

class Stage :
    public Object
{
public:
    Stage();
    ~Stage();

    //������
    void Init() override; 
    //�X�V
    void Update() override;
    //�`��
    void Draw() override;   

    void Release() override;

private:

};

