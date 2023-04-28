#pragma once
#include "BaseScene.h"
#include "../common/Geometry.h"

class TitelScene :
    public BaseScene
{
public:
    TitelScene();
    ~TitelScene();

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
        return SceneID::Title;
    };

private :
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);



};

