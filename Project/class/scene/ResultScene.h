#pragma once
#include "BaseScene.h"
#include "../common/Geometry.h"

class Player;
class Controller;

enum class playerType;

class ResultScene :
    public BaseScene
{
public:
    ResultScene(playerType ptype);
    ~ResultScene();

    //初期化
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    //描画処理
    void DrawScreen(void) override;

    //解放処理
    void Release(void) override;

    //シーンID返却
    SceneID GetSceneID(void) override
    {
        return SceneID::Result;
    };

private:
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);

    //コントローラー情報
    std::unique_ptr<Controller> controller_;

    int screenImage_;    //現在の画面を画像で保存

    playerType playertype_;

    int winSe;

};

