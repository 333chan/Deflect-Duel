#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class VersusScene :
    public BaseScene
{
public:
    VersusScene();
    ~VersusScene();


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
    UniqueScene UpdateScene(UniqueScene& scene);

    //�w�i���W
    Vector2 bgPos_;
    Vector2 bgEndPos_;

    Vector2 p1windowPos_;
    Vector2 p1windowEndPos_;

    Vector2 p2windowPos_;
    Vector2 p2windowEndPos_;


protected:
    //tmx
    TmxObj tmxObj_;
    //�R���g���[���[
    std::unique_ptr<Controller> controller_;

};

