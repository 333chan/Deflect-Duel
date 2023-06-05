#pragma once
#include <memory>	//��Ɂ[���|�C���^�ɕK�v
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"
#include "../../common/Raycast.h"
#include "../../object/ball/Ball.h"


class Controller;
enum class ControllerType;

//���
enum class State
{
    Idel,       //����
    JumpUp,     //�W�����v�㏸
    Fall,       //���~
    MoveLeft,   //���ړ�
    MoveRight,  //�E�ړ�
    Crouching,  //���Ⴊ��
    Attack,     //�U��
    Max
};

//����
enum class Dir
{
    Up,     //��
    Down,   //��
    Right,  //�E
    Left,   //��   
    Max
};

class Player :
    public Object
{
public:
    Player(ControllerType type, std::shared_ptr<Ball>& ball);
    ~Player();

    //������
    void Init() override;

    //�X�V
    void Update(void) override;

    //�`��
    void Draw(void) override;

    //���
    void Release(void) override;

private:
    //�R���g���[���[
    std::unique_ptr<Controller> controller_;

    //�F
    int color ;

    //���W
    Vector2 pos_; 
    
    //�傫��
    Vector2 size_;

    //�U�����̉摜�̑傫��
    Vector2 attacksize_;

    //���
    State state_;

    //����
    Dir dir_;

    //�d��
    float gravity_;

    //�␳����
    Vector2 offset_;

    //�n�_�A�I�_
    Line stageLine_;
    Line playerLine_;

    //����
    Raycast raycast_;

    //�{�[��
    std::shared_ptr<Ball> ball_;

    //�X�e�[�W�Ƃ̔��菈��
    bool IsStageHit(Line collRay);
    bool IsBallHit();
    bool IsAttackHit();

    //�o�ߎ���
    double jumpDeltaTime_;

    //�������v���C���[�摜�؂�ւ�
    int playerImage_;
    int playerImage2_;
    int playerImage3_;
    int playerImage4_;
    int playerImage5_;
    int playerImage6_;

    //�{�[���̍��W�Ƒ傫��
    Vector2 ballpos_;
    Vector2 ballsize_;

protected:
    //tmx
    TmxObj tmxObj_;
};

