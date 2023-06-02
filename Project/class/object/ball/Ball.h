#pragma once
#include "../Object.h"
#include "../../common/Raycast.h"
#include "../../common/Geometry.h"
#include "../../tmx/TmxObj.h"


enum class Dir;

class Ball :
    public Object
{
public:
    Ball();
    ~Ball();

    //������
    void Init() override;
    //�X�V
    void Update() override;
    //�`��
    void Draw() override;
    //���
    void Release() override;

    void SetBallform(Vector2& pos,Vector2&size);


    //���W
    Vector2 pos_;

    //�傫��
    Vector2 size_;

    int rad_;
    
private:
    //�{�[���摜
    int ballImage_;

    //�d��
    float gravity_;

    Vector2 offset_;

    //�����蔻�菈��
    bool IsStageHit();

    //�X�e�[�W
    TmxObj tmxObj_;

    //����
    Raycast raycast_;
};

