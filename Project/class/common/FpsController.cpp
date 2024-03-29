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
	//1フレーム目なら時刻を記憶
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
	//かかった時間
	int tookTime = GetNowCount() - startTime_;

	//待つ時間
	int waitTime = count_ * 1000 / FPS_MAX - tookTime;

	if (waitTime>0)
	{
		Sleep(waitTime);
	}

}
