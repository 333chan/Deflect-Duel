#pragma once
#include <DxLib.h>
#include <memory>	//��Ɂ[���|�C���^�ɕK�v
#include "../common/Geometry.h"
#include "../common/SceneID.h"

class BaseScene;



using UniqueScene = std::unique_ptr<BaseScene>;

class BaseScene
{
public:

	BaseScene();
	virtual ~BaseScene();
	// ����������
	virtual void Init(void) = 0;

	// �X�V�X�e�b�v
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// �`�揈��
	virtual void  Draw(void);
	virtual void  DrawScreen(void) = 0;

	// �������
	virtual void  Release(void) = 0;

	virtual SceneID GetSceneID(void) = 0;


private:

protected:


	// �X�N���[�����
	int screenID_;

	// �X�N���[���̑傫��
	int screenSizeX_;	// ��
	int screenSizeY_;	// �c

};

