#include <DxLib.h>
#include <math.h>
#include "../../_debug/_DebugDispOut.h"
#include "FpsController.h"

constexpr int FPS_AVERAGE = 60;
constexpr int FPS_MAX = 60;

FpsController::FpsController()
{
	Init();
}

FpsController::~FpsController()
{
}

void FpsController::Init()
{
	startTime_ = GetNowCount();
	count_ = 0;
	fps_ = 0;
}

bool FpsController::Updeta()
{
	//1ƒtƒŒ[ƒ€–Ú‚È‚çŽž‚ð‹L‰¯
	if (count_ ==0)
	{
		startTime_ = GetNowCount();
	}
	if (count_ == FPS_AVERAGE)
	{
		int time = GetNowCount();
		fps_ = 1000.0f / ((time -startTime_)/(float)FPS_AVERAGE);
		count_ = 0;
		startTime_ = time;
	}
	count_++;

	return true;
}

void FpsController::Draw()
{
	_dbgDrawFormatString(0, 0, 0x00ff00, "%.1f",fps_);
}

void FpsController::Wait()
{
	//‚©‚©‚Á‚½ŽžŠÔ
	int tookTime = GetNowCount() - startTime_;

	//‘Ò‚ÂŽžŠÔ
	int waitTime = count_ * 1000 / FPS_MAX - tookTime;

	if (waitTime>0)
	{
		Sleep(waitTime);
	}

}
