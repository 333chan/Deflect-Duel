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
	//初期化
	void Init(void);

	//更新
	void Run(void);

	//デルタタイム取得
	double GetDeltaTime(void);

	//スクリーンサイズ取得
	Vector2 GetScreenSize(void);

private:
	//システムの初期化処理
	bool SystemInit(void);
	SceneManager(void);
	~SceneManager(void);

	void Update(void);
	void Draw(void);
	void Relese(void);

	//経過時間
	std::chrono::system_clock::time_point tickCount_;

	//デルタタイム
	double deltaTime_;

	//シーン情報
	UniqueScene scene_;

};

