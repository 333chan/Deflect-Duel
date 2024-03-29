#pragma once
#include <memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"
#include "../common/Geometry.h"

class Controller;
class Player;
class Ball;
class Stage;

enum class PlayerType;

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
    std::unique_ptr<Player> player_;    //P1
    std::unique_ptr<Player> player2_;   //P2
    std::unique_ptr<Player> player3_;   //P2
    std::unique_ptr<Player> player4_;   //P2

    //コントローラー情報
    std::unique_ptr<Controller> controller_;

    //ステージ情報
    std::unique_ptr<Stage> stage_;

    //ボール情報
    std::shared_ptr<Ball> ball_;

    //1P2P判断
    PlayerType playertype_;

    //サウンド
    int gameBgm_;

    Vector2 p1windowPos_;
    Vector2 p1windowEndPos_;

    Vector2 p2windowPos_;
    Vector2 p2windowEndPos_;


    //tmx
    TmxObj tmxObj_;
};

