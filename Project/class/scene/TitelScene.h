#pragma once
#include <vector>
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../object/Object.h"
#include "../tmx/TmxObj.h"

class Controller;
enum class ControllerType;

struct BG
{
    int hadle;  //�摜�n���h��
    std::pair<Vector2, Vector2> pos1;   //�ꖇ�ڂ̎n�I�_
    std::pair<Vector2, Vector2> pos2;   //�񖇖ڂ̎n�I�_
    float speed;    //�X�N���[�����x
};

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

    //�w�i���W
    Vector2 bgPos_;
    Vector2 bgEndPos_;

    //���S���W
    Vector2 logoPos_;
    Vector2 logoPosEnd_;


    //�摜�n���h��
    int bgImageH_;  //�w�i
    int logoImageH_; //���S�摜

    //�T�E���h
    int bgm_;
    int decideSe_;  //���艹

    bool controllerFlg;

    std::vector<BG> bgVec_;

protected:
    //tmx
    TmxObj tmxObj_;

    //�R���g���[���[
    std::unique_ptr<Controller> controller_;


};

