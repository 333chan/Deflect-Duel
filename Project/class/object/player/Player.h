#pragma once
#include <memory>	//ゆにーくポインタに必要
#include "../../common/Geometry.h"
#include "../Object.h"



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
    Vector2Int pos_; 

    float gravity_;

protected:
};

