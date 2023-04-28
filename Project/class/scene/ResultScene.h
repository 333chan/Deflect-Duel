#pragma once
#include "BaseScene.h"
#include "../common/Geometry.h"

class ResultScene :
    public BaseScene
{
public:
    ResultScene();
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



};

