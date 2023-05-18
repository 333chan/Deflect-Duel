#pragma once
#include "../../common/Geometry.h"
#include "../Object.h"
#include "../../tmx/TmxObj.h"

class Stage :
    public Object
{
public:
    Stage();
    ~Stage();

    //初期化
    void Init() override; 
    //更新
    void Update() override;
    //描画
    void Draw() override;   

    void Release() override;

private:

    Vector2 stagePos;		//始点
    Vector2 stagePosEnd;	//終点

    int stageImage_;

protected:
    //tmx
    TmxObj tmxObj_;

};

