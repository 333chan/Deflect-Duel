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
	//初期化
	void Init(void);

	//更新
	void Run(void);

private:
	//システムの初期化処理
	bool SystemInit(void);
	SceneManager(void);
	~SceneManager(void);

	void Update(void);
	void Draw(void);
	void Relese(void);

	//シーン情報
	UniqueScene scene_;

};

