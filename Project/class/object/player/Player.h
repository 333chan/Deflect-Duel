#pragma once
#include <memory>	//��Ɂ[���|�C���^�ɕK�v
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"
#include "../../common/Raycast.h"
#include "../../object/ball/Ball.h"


class Controller;
enum class ControllerType;

class AnimController;
enum class Anim;

//���
enum class State
{
    Idle,       //����
    JumpUp,     //�W�����v�㏸
    Fall,       //���~
    MoveLeft,   //���ړ�
    MoveRight,  //�E�ړ�
    Crouching,  //���Ⴊ��
    Attack,     //�n��U��
    AirAttack,  //�󒆍U��
    Death,     //��
    Max
};

//����
enum class Dir
{
    Up,     //��
    Down,   //��
    Right,  //�E
    Left,   //��   
    AirAttackRight, //�󒆍U���E
    AirAttackLeft,//�󒆍U����
    Max
};

//�v���C���[�̎��
enum class PlayerType
{
    One,    //1P
    Two,    //2P
    Three,  //3P
    Four,   //4P
    Max

};

class Player :
    public Object
{
public:
    Player(ControllerType type, PlayerType pType,std::shared_ptr<Ball>& ball);
    ~Player();

    //������
    void Init() override;

    //�X�V
    void Update(void) override;

    //�`��
    void Draw(void) override;

    //���
    void Release(void) override;

    //��Ԃ̎擾
    State GetState(void);

    //�v���C���[�̎�ނ̎擾
    PlayerType GetPlayerType(void);

private:
    //�R���g���[���[
    std::unique_ptr<Controller> controller_;

    //�A�j���[�V����
    std::unique_ptr<AnimController> animController_;

    //�{�[��
    std::shared_ptr<Ball> ball_;

    //1P��2P���f
    PlayerType playertype_;

    //�F
    int color ;

    //���W
    Vector2 pos_; 
    Vector2 p2Pos_; 
    
    //�傫��
    Vector2 collSize_;

    Vector2 DrawSize_;

    Vector2 imagePos_;
    Vector2 imageSize_;

    Vector2 movePos_;

    //�U�����̉摜�̑傫��
    Vector2 attacksize_;
    Vector2 attackpos_;

    int reverse_;

    //���˕Ԃ�����
    Vector2 refDir_;

    //���
    State state_;

    Anim animState_;

    //����
    Dir dir_;

    //�v���C���[�̎��
    PlayerType type_;

    //�d��
    float gravity_;

    double yVel_ ;
    bool animEnd_;

    //�␳����
    Vector2 offset_;

    //�n�_�A�I�_
    Line stageLine_;
    Line playerLine_;

    //����
    Raycast raycast_;

    bool isGround;

    //�o�ߎ���
    double jumpDeltaTime_;

    //�{�[���̍��W�Ƒ傫��
    Vector2 ballpos_;
    Vector2 ballsize_;

    //�{�[�����W
    Vector2 bpos_;


    //���菈��
    bool IsStageHit(Line collRay);
    bool IsBallHit();
    bool IsAttackHit();

    //�ړ�����
    void MovePosition(Dir dir);

    std::unique_ptr<Controller> SetControllerType(ControllerType type);

protected:
    //tmx
    TmxObj tmxObj_;
};

