#pragma once
#include <memory>
#include "BaseScene.h"
#include "../common/Geometry.h"

class Controller;
class Player;
class Stage;

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();

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
        return SceneID::Game;
    };

private:
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene & scene);

    //プレイヤー情報
    std::unique_ptr<Player> player_;

    //コントローラー情報
    std::unique_ptr<Controller> controller_;

    //ステージ情報
    std::unique_ptr<Stage> stage_;
};

