#pragma once
#include "../Object.h"
#include "../../common/Raycast.h"
#include "../../common/Geometry.h"
#include "../../tmx/TmxObj.h"

class Player;
enum class Dir;

class AnimController;
enum class Anim;

class Ball :
    public Object
{
public:
    Ball();
    ~Ball();

    //初期化
    void Init() override;
    //更新
    void Update() override;
    //描画
    void Draw() override;
    //解放
    void Release() override;

    /// <summary>
    /// ボールの形情報
    /// </summary>
    /// <param name="pos">ボールの位置</param>
    /// <param name="size">ボールの大きさ</param>
    void SetBallform(Vector2& pos,Vector2&size);

    //反射方向取得
    void SetAttackRef(Vector2& refDir);


    //座標
    Vector2 pos_;   
    Vector2 centerPos_; //中心
    Vector2 endPos_;    //終点


    //移動後座標
    Vector2 movePos_;

    //大きさ
    Vector2 collSize_;
    Vector2 imageSize_;

    //攻撃ヒットフラグ
    bool attackHitFlg_;

    
private:
    std::unique_ptr<AnimController> animController_;

    //レイ
    void VelRay();

    //あたり判定処理
    bool IsStageHit();

    //回転
    double angle_;

    float spinSpead_;

    //ステージ
    TmxObj tmxObj_;

    //補正
    Vector2 offset_;

    //反射
    Vector2 refPow_;
    Vector2 refDir_;

    //移動方向ベクトル
    Vector2 ballVec_;

    //速度
    Vector2 speed_;
    Vector2 maxSpeed_;

    bool fastHitflg_;

    //判定
    Raycast raycast_;

    //ベクトル
    Vector2 vec_;
    Vector2 vecNom_;
    float vecLen_;




};

