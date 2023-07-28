#pragma once
#include "../Object.h"
#include "../../common/Raycast.h"
#include "../../common/Geometry.h"
#include "../../tmx/TmxObj.h"

class Player;
enum class Dir;

class AnimController;
enum class Anim;

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

    /// <summary>
    /// �{�[���̌`���
    /// </summary>
    /// <param name="pos">�{�[���̈ʒu</param>
    /// <param name="size">�{�[���̑傫��</param>
    void SetBallform(Vector2& pos,Vector2&size);

    //���˕����擾
    void SetAttackRef(Vector2& refDir);


    //���W
    Vector2 pos_;   
    Vector2 centerPos_; //���S
    Vector2 endPos_;    //�I�_


    //�ړ�����W
    Vector2 movePos_;

    //�傫��
    Vector2 collSize_;
    Vector2 imageSize_;

    //�U���q�b�g�t���O
    bool attackHitFlg_;

    
private:
    std::unique_ptr<AnimController> animController_;

    //���C
    void VelRay();

    //�����蔻�菈��
    bool IsStageHit();

    //��]
    double angle_;

    float spinSpead_;

    //�X�e�[�W
    TmxObj tmxObj_;

    //�␳
    Vector2 offset_;

    //����
    Vector2 refPow_;
    Vector2 refDir_;

    //�ړ������x�N�g��
    Vector2 ballVec_;

    //���x
    Vector2 speed_;
    Vector2 maxSpeed_;

    bool fastHitflg_;

    //����
    Raycast raycast_;

    //�x�N�g��
    Vector2 vec_;
    Vector2 vecNom_;
    float vecLen_;




};

