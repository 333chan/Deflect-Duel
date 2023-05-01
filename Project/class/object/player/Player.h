#pragma once
#include <memory>	//ゆにーくポインタに必要
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"


class Controller;
enum class ControllerType;

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

    //座標
    Vector2 pos_; 
    Vector2 linepos_; 
    Vector2 linepos2_; 
    Vector2 size_;

    float gravity_;

    bool IsHit();

    Line stageLine_;

    

protected:
    //tmx
    TmxObj tmxObj_;
};

