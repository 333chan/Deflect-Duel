#pragma once
#include <memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"
#include "../common/Geometry.h"

class Controller;
class Player;
class Ball;
class Stage;

enum class PlayerType;

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
    std::unique_ptr<Player> player_;    //P1
    std::unique_ptr<Player> player2_;   //P2

    //�R���g���[���[���
    std::unique_ptr<Controller> controller_;

    //�X�e�[�W���
    std::unique_ptr<Stage> stage_;

    //�{�[�����
    std::shared_ptr<Ball> ball_;

    //1P2P���f
    PlayerType playertype_;

    //�T�E���h
    int gameBgm_;

    Vector2 p1windowPos_;
    Vector2 p1windowEndPos_;

    Vector2 p2windowPos_;
    Vector2 p2windowEndPos_;


    //tmx
    TmxObj tmxObj_;
};

