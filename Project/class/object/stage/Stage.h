#pragma once
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"

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

    //���W
    Vector2 stagePos;		//�n�_
    Vector2 stagePosEnd;	//�I�_

    // �摜
    int stageImage_;        //�w�i

protected:
    //tmx
    TmxObj tmxObj_;

};

