#pragma once
#include <vector>
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../object/Object.h"
#include "../tmx/TmxObj.h"

class Controller;
enum class ControllerType;

struct BG
{
    int hadle;  //画像ハンドル
    std::pair<Vector2, Vector2> pos1;   //一枚目の始終点
    std::pair<Vector2, Vector2> pos2;   //二枚目の始終点
    float speed;    //スクロール速度
};

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

    //背景座標
    Vector2 bgPos_;
    Vector2 bgEndPos_;

    //ロゴ座標
    Vector2 logoPos_;
    Vector2 logoPosEnd_;


    //画像ハンドル
    int bgImageH_;  //背景
    int logoImageH_; //ロゴ画像

    //サウンド
    int bgm_;
    int decideSe_;  //決定音

    bool controllerFlg;

    std::vector<BG> bgVec_;

protected:
    //tmx
    TmxObj tmxObj_;

    //コントローラー
    std::unique_ptr<Controller> controller_;


};

