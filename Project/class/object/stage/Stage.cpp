#include <DxLib.h>
#include "Stage.h"
#include "../../scene/SceneManager.h"

Stage::Stage()
{
	Init();
}

Stage::~Stage()
{
	Release();
}

void Stage::Init()
{
}

void Stage::Update()
{

}

void Stage::Draw()
{
	DrawBox(0,600,2000,700,0xff0000,true);
}

void Stage::Release()
{
}
