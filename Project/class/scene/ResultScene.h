#pragma once
#include "BaseScene.h"
#include "../common/Geometry.h"

class Player;
class Controller;

enum class playerType;

class ResultScene :
    public BaseScene
{
public:
    ResultScene(playerType ptype);
    ~ResultScene();

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
        return SceneID::Result;
    };

private:
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);

    //�R���g���[���[���
    std::unique_ptr<Controller> controller_;

    int screenImage_;    //���݂̉�ʂ��摜�ŕۑ�

    playerType playertype_;

    int winSe;

};

