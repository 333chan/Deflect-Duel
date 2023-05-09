#pragma once
#include <memory>	//ゆにーくポインタに必要
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"
#include "../../common/Raycast.h"


class Controller;
enum class ControllerType;

//向き
enum class Dir
{
    Up,
    Down,
    Right,
    Left,
    Max
};

class Player :
    public Object
{
public:
    Player(ControllerType type);
    ~Player();

    //初期化
    void Init() override;

    //更新
    void Update(void) override;

    //描画
    void Draw(void) override;

    //解放
    void Release(void) override;

private:
    //コントローラー
    std::unique_ptr<Controller> controller_;

    //色
    int color ;

    //座標
    Vector2 pos_; 
    
    //大きさ
    Vector2 size_;

    //向いている方向
    Dir dir_;

    //重力
    float gravity_;

    //補正差分
    Vector2 offset_;

    //始点、終点
    Line stageLine_;
    Line playerLine_;

    //判定
    Raycast raycast_;

    //あたり判定処理
    bool IsHit(void);

protected:
    //tmx
    TmxObj tmxObj_;
};

