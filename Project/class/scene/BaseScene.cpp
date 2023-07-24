#include <DxLib.h>
#include "BaseScene.h"
#include"../common/SoundManager.h"
#include"../common/ImageManager.h"

BaseScene::BaseScene()
{
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&screenSizeX_,&screenSizeY_);
	screenID_ = MakeScreen(screenSizeX_, screenSizeY_,true);
}

BaseScene::~BaseScene()
{

}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, false);
}


