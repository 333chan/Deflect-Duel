#pragma once
#include <DxLib.h>
#include <chrono>
#include"../common/Geometry.h"
#include"BaseScene.h"

#define lpSceneMng SceneManager::GetInstance()

class SceneManager
{
public:
	static SceneManager& GetInstance()
	{
		static SceneManager s_Instance;
		return s_Instance;
	}
	//������
	void Init(void);

	//�X�V
	void Run(void);

	//�f���^�^�C���擾
	double GetDeltaTime(void);

	//�X�N���[���T�C�Y�擾
	Vector2 GetScreenSize(void);

private:
	//�V�X�e���̏���������
	bool SystemInit(void);
	SceneManager(void);
	~SceneManager(void);

	void Update(void);
	void Draw(void);
	void Relese(void);

	//�o�ߎ���
	std::chrono::system_clock::time_point tickCount_;

	//�f���^�^�C��
	double deltaTime_;

	//�V�[�����
	UniqueScene scene_;

};

