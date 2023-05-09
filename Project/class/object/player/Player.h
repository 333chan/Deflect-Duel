#pragma once
#include <memory>	//��Ɂ[���|�C���^�ɕK�v
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"
#include "../../common/Raycast.h"


class Controller;
enum class ControllerType;

//����
enum class Dir
{
    Up,
    Down,
    Right,
    Left,
    Max
};

class Player :
    public Object
{
public:
    Player(ControllerType type);
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

    //�����Ă������
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

    //�����蔻�菈��
    bool IsHit(void);

protected:
    //tmx
    TmxObj tmxObj_;
};

