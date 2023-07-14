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

enum class playerType
{
    One,    //1P
    Two,    //2P
    Max

};

class Player :
    public Object
{
public:
    Player(ControllerType type, playerType pType,std::shared_ptr<Ball>& ball);
    ~Player();

    //������
    void Init() override;

    //�X�V
    void Update(void) override;

    //�`��
    void Draw(void) override;

    //���
    void Release(void) override;

    State GetState(void);
    playerType GetPlayerType(void);

private:
    //�R���g���[���[
    std::unique_ptr<Controller> controller_;

    playerType playertype_;

    //�{�[��
    std::shared_ptr<Ball> ball_;

    //�F
    int color ;

    //���W
    Vector2 pos_; 
    Vector2 p2Pos_; 
    
    //�傫��
    Vector2 size_;

    Vector2 movePos_;

    //�U�����̉摜�̑傫��
    Vector2 attacksize_;
    Vector2 attackpos_;


    Vector2 refDir_;

    //���
    State state_;

    //����
    Dir dir_;

    playerType type_;

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

    Vector2 bpos_;

    int jumpSe_;
    int attackMissSe_;
    int attackSe_;
    int daethSe_;

protected:
    //tmx
    TmxObj tmxObj_;
};

