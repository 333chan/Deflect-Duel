#pragma once
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../object/Object.h"
#include "../tmx/TmxObj.h"

class Controller;
enum class ControllerType;

class Title;

class SelectScene :
    public BaseScene
{
public:
    SelectScene();
    ~SelectScene();

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
        return SceneID::Select;
    };

private:
    //シーン切り替え関数
    UniqueScene UpdateScene(UniqueScene& scene);

    //ステージ画像座標
    Vector2 stage1Pos;
    Vector2 stage1PosEnd;

protected:
    //tmx
    TmxObj tmxObj_;

    //コントローラー
    std::unique_ptr<Controller> controller_;


};

