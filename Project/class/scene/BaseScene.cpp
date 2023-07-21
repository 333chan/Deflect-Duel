#include <DxLib.h>
#include "BaseScene.h"
#include"../common/SoundManager.h"

BaseScene::BaseScene()
{
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&screenSizeX_,&screenSizeY_);
	screenID_ = MakeScreen(screenSizeX_, screenSizeY_,true);
	//lpSoundMng.Loading();
}

BaseScene::~BaseScene()
{

}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, screenID_, false);
}


