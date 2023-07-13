#pragma once
#include "../Object.h"
#include "../../common/Raycast.h"
#include "../../common/Geometry.h"
#include "../../tmx/TmxObj.h"

class Player;
enum class Dir;

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

    void SetBallform(Vector2& pos,Vector2&size);

    void SetAttackRef(Vector2& refDir);


    //座標
    Vector2 pos_;
    Vector2 centerPos_;
    Vector2 endPos_;

    //大きさ
    Vector2 size_;

    int rad_;

    bool flg;
    bool attackHitFlg_;

    Vector2 movePos_;
    
private:
    //ボール画像
    int ballImage_;

    //重力
    float gravity_;

    Vector2 offset_;

    //あたり判定処理
    bool IsStageHit();

    //ステージ
    TmxObj tmxObj_;

    Vector2 refPow_;
    Vector2 refDir_;

    Vector2 ballVec_;

    Vector2 speed_;
    Vector2 maxSpeed_;

    bool fastHitflg_;

    //判定
    Raycast raycast_;

    Vector2 balloff;


    Vector2 vec;
    float vecLen;
    Vector2 vecNom;

    //レイ
    void VelRay();


};

