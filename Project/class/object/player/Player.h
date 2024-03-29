#pragma once
#include <memory>	//ゆにーくポインタに必要
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"
#include "../../common/Raycast.h"
#include "../../object/ball/Ball.h"


class Controller;
enum class ControllerType;

class AnimController;
enum class Anim;

//状態
enum class State
{
    Idle,       //立ち
    JumpUp,     //ジャンプ上昇
    Fall,       //下降
    MoveLeft,   //左移動
    MoveRight,  //右移動
    Crouching,  //しゃがみ
    Attack,     //地上攻撃
    AirAttack,  //空中攻撃
    Death,     //死
    Max
};

//方向
enum class Dir
{
    Up,     //上
    Down,   //下
    Right,  //右
    Left,   //左   
    AirAttackRight, //空中攻撃右
    AirAttackLeft,//空中攻撃左
    Max
};

//プレイヤーの種類
enum class PlayerType
{
    One,    //1P
    Two,    //2P
    Three,  //3P
    Four,   //4P
    Max

};

class Player :
    public Object
{
public:
    Player(ControllerType type, PlayerType pType,std::shared_ptr<Ball>& ball);
    ~Player();

    //初期化
    void Init() override;

    //更新
    void Update(void) override;

    //描画
    void Draw(void) override;

    //解放
    void Release(void) override;

    //状態の取得
    State GetState(void);

    //プレイヤーの種類の取得
    PlayerType GetPlayerType(void);

private:
    //コントローラー
    std::unique_ptr<Controller> controller_;

    //アニメーション
    std::unique_ptr<AnimController> animController_;

    //ボール
    std::shared_ptr<Ball> ball_;

    //1Pか2P判断
    PlayerType playertype_;

    //色
    int color ;

    //座標
    Vector2 pos_; 
    Vector2 p2Pos_; 
    
    //大きさ
    Vector2 collSize_;

    Vector2 DrawSize_;

    Vector2 imagePos_;
    Vector2 imageSize_;

    Vector2 movePos_;

    //攻撃時の画像の大きさ
    Vector2 attacksize_;
    Vector2 attackpos_;

    int reverse_;

    //跳ね返す方向
    Vector2 refDir_;

    //状態
    State state_;

    Anim animState_;

    //方向
    Dir dir_;

    //プレイヤーの種類
    PlayerType type_;

    //重力
    float gravity_;

    double yVel_ ;
    bool animEnd_;

    //補正差分
    Vector2 offset_;

    //始点、終点
    Line stageLine_;
    Line playerLine_;

    //判定
    Raycast raycast_;

    bool isGround;

    //経過時間
    double jumpDeltaTime_;

    //ボールの座標と大きさ
    Vector2 ballpos_;
    Vector2 ballsize_;

    //ボール座標
    Vector2 bpos_;


    //判定処理
    bool IsStageHit(Line collRay);
    bool IsBallHit();
    bool IsAttackHit();

    //移動処理
    void MovePosition(Dir dir);

    std::unique_ptr<Controller> SetControllerType(ControllerType type);

protected:
    //tmx
    TmxObj tmxObj_;
};

