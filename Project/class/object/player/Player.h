#pragma once
#include <memory>	//��Ɂ[���|�C���^�ɕK�v
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"


class Controller;
enum class ControllerType;

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

    //���W
    Vector2 pos_; 
    Vector2 linepos_; 
    Vector2 linepos2_; 
    Vector2 size_;

    float gravity_;

    bool IsHit();

    Line stageLine_;

    

protected:
    //tmx
    TmxObj tmxObj_;
};

