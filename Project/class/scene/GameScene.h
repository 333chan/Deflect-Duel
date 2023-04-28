#pragma once
#include <memory>
#include "BaseScene.h"
#include "../common/Geometry.h"

class Controller;
class Player;
class Stage;

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();

    //������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    //�`�揈��
    void DrawScreen(void) override;

    //�������
    void Release(void) override;

    //�V�[��ID�ԋp
    SceneID GetSceneID(void) override
    {
        return SceneID::Game;
    };

private:
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene & scene);

    //�v���C���[���
    std::unique_ptr<Player> player_;

    //�R���g���[���[���
    std::unique_ptr<Controller> controller_;

    //�X�e�[�W���
    std::unique_ptr<Stage> stage_;
};

