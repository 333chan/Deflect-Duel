#pragma once
#include <DxLib.h>
#include <memory>	//ゆにーくポインタに必要
#include "../common/Geometry.h"
#include "../common/SceneID.h"

class BaseScene;



using UniqueScene = std::unique_ptr<BaseScene>;

class BaseScene
{
public:

	BaseScene();
	virtual ~BaseScene();
	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// 描画処理
	virtual void  Draw(void);
	virtual void  DrawScreen(void) = 0;

	// 解放処理
	virtual void  Release(void) = 0;

	virtual SceneID GetSceneID(void) = 0;


private:

protected:


	// スクリーン情報
	int screenID_;

	// スクリーンの大きさ
	int screenSizeX_;	// 横
	int screenSizeY_;	// 縦

};

