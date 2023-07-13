#pragma once
#include "../Object.h"
#include "../../common/Raycast.h"
#include "../../common/Geometry.h"
#include "../../tmx/TmxObj.h"

class Player;
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

    void SetAttackRef(Vector2& refDir);


    //���W
    Vector2 pos_;
    Vector2 centerPos_;
    Vector2 endPos_;

    //�傫��
    Vector2 size_;

    int rad_;

    bool flg;
    bool attackHitFlg_;

    Vector2 movePos_;
    
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

    Vector2 refPow_;
    Vector2 refDir_;

    Vector2 ballVec_;

    Vector2 speed_;
    Vector2 maxSpeed_;

    bool fastHitflg_;

    //����
    Raycast raycast_;

    Vector2 balloff;


    Vector2 vec;
    float vecLen;
    Vector2 vecNom;

    //���C
    void VelRay();


};

