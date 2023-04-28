#pragma once
#include "../../common/Geometry.h"
#include "../Object.h"

class Stage :
    public Object
{
public:
    Stage();
    ~Stage();

    //‰Šú‰»
    void Init() override; 
    //XV
    void Update() override;
    //•`‰æ
    void Draw() override;   

    void Release() override;

private:

};

