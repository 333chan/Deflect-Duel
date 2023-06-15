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

    //大きさ
    Vector2 size_;

    int rad_;

    bool flg;
    bool attackHitFlg_;

    Vector2 movepow;

    Vector2 movePos_;
    Vector2 attackPos_;
    
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

    Vector2 attackRefPow_;
    Vector2 attackRefDir_;

    //判定
    Raycast raycast_;



};

