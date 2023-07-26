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
    Attack,     //�U��
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
    Vector2 size_;

    Vector2 imageSize_;

    Vector2 movePos_;

    //�U�����̉摜�̑傫��
    Vector2 attacksize_;
    Vector2 attackpos_;

    int reversal_;

    //���˕Ԃ�����
    Vector2 refDir_;

    //���
    State state_;

    //����
    Dir dir_;

    //�v���C���[�̎��
    PlayerType type_;

    //�d��
    float gravity_;

    //�␳����
    Vector2 offset_;

    //�n�_�A�I�_
    Line stageLine_;
    Line playerLine_;

    //����
    Raycast raycast_;

    //���菈��
    bool IsStageHit(Line collRay);
    bool IsBallHit();
    bool IsAttackHit();

    bool isGround;

    //�o�ߎ���
    double jumpDeltaTime_;

    //�������v���C���[�摜�؂�ւ�
    int playerImage_;
    int playerImage2_;
    int playerImage3_;
    int playerImage4_;
    int playerImage5_;
    int playerImage6_;
    int playerImage7_;

    //�{�[���̍��W�Ƒ傫��
    Vector2 ballpos_;
    Vector2 ballsize_;

    //�{�[�����W
    Vector2 bpos_;

protected:
    //tmx
    TmxObj tmxObj_;
};

