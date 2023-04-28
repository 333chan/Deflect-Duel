#pragma once
#include <DxLib.h>
#include"../common/Geometry.h"
#include"BaseScene.h"

#define IpSceneMng SceneManager::GetInstance()

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

private:
	//�V�X�e���̏���������
	bool SystemInit(void);
	SceneManager(void);
	~SceneManager(void);

	void Update(void);
	void Draw(void);
	void Relese(void);

	//�V�[�����
	UniqueScene scene_;

};

