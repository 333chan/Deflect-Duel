#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class VersusScene :
    public BaseScene
{
public:
    VersusScene();
    ~VersusScene();


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
    UniqueScene UpdateScene(UniqueScene& scene);

    //背景座標
    Vector2 bgPos_;
    Vector2 bgEndPos_;

    Vector2 p1windowPos_;
    Vector2 p1windowEndPos_;

    Vector2 p2windowPos_;
    Vector2 p2windowEndPos_;


protected:
    //tmx
    TmxObj tmxObj_;
    //コントローラー
    std::unique_ptr<Controller> controller_;

};

