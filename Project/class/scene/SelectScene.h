#pragma once
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../object/Object.h"
#include "../tmx/TmxObj.h"

class Controller;
enum class ControllerType;

class SelectScene :
    public BaseScene
{
public:
    SelectScene();
    ~SelectScene();

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
        return SceneID::Select;
    };

private:
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);

    //�w�i���W
    Vector2 bgPos;
    Vector2 bgPosEnd;

    //�X�e�[�W�摜���W
    Vector2 stage1Pos;
    Vector2 stage1PosEnd;


    int bgImageH_;      //�w�i�摜
    int stage1ImageH_;  //�X�e�[�W�摜

protected:
    //tmx
    TmxObj tmxObj_;

    //�R���g���[���[
    std::unique_ptr<Controller> controller_;


};
