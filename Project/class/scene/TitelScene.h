#pragma once
#include "BaseScene.h"
#include "../common/Geometry.h"

class TitelScene :
    public BaseScene
{
public:
    TitelScene();
    ~TitelScene();

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
        return SceneID::Title;
    };

private :
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);



};

