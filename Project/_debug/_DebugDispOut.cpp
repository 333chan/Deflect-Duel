#ifdef _DEBUG
#include <windows.h>
#include <DxLib.h>
#include "_DebugDispOut.h"
#include "_DebugConOut.h"

std::unique_ptr<_DebugDispOut, _DebugDispOut::_DebugDispOutDeleter> _DebugDispOut::s_Instance(new _DebugDispOut);
_DebugDispOut::_DebugDispOut()
{
	DbgScreen_ = -1;
	waitTime_  = 0;
	_alpha = 255;
	dispFlag_ = true;
	endKey_[0] = 0;
	endKey_[1] = 0;
	pouseKey_[0] = 0;
	pouseKey_[1] = 0;
	homeKey_[0] = 0;
	homeKey_[1] = 0;
	f1Key_[0] = 0;
	f1Key_[1] = 0;
	backSp_[0] = 0;
	backSp_[1] = 0;
	ghBefor_ = 0;
	clsFlag_ = true;
	fpsCount_ = 0;
	fpsView_ = 0;
	drawLineY_ = 0;
	mesPosX_ = 0;
	mesPosY_ = 0;
}

_DebugDispOut::~_DebugDispOut()
{
}

void _DebugDispOut::SetScreen(void)
{
	ghBefor_ = GetDrawScreen();
	SetDrawScreen(DbgScreen_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
}

void _DebugDispOut::RevScreen(void)
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawScreen(ghBefor_);
}

void _DebugDispOut::WaitMode(void)
{
	f1Key_[1] = f1Key_[0];
	f1Key_[0] = CheckHitKey(KEY_INPUT_F1);
	if (f1Key_[0] && !f1Key_[1])
	{
		TRACE("�f�o�b�O�\��ON/OFF        �FHome\n");
		TRACE("�f�o�b�O�\���N���A�[ON/OFF�FDelete\n\n");

		TRACE("�X���[�@�\(�������)�FPageDown\n");
		TRACE("�X���[�@�\(�� �� ��)�FPageUp\n");
		TRACE("�ꎞ��~/�X���[�E�ꎞ��~�����FPause/Break\n\n");

		TRACE("�o�b�N�O���E���h�����L��/�����FBackSpace\n");
	}

	if (CheckHitKey(KEY_INPUT_PGDN))
	{
		waitTime_+=10.0;
		TRACE("�X���[�F%f\n", waitTime_);
	}
	if (CheckHitKey(KEY_INPUT_PGUP))
	{
		waitTime_ -= 10.0;
		if (waitTime_ < 0.0)
		{
			waitTime_ = 0.0;
		}
		TRACE("�X���[�F%f\n", waitTime_);
	}
	pouseKey_[1] = pouseKey_[0];
	pouseKey_[0] = CheckHitKey(KEY_INPUT_PAUSE);

	if (waitTime_)
	{
		startTime_ = std::chrono::system_clock::now();
		do {
			pouseKey_[1] = pouseKey_[0];
			pouseKey_[0] = CheckHitKey(KEY_INPUT_PAUSE);
			if (ProcessMessage() != 0 || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			{
				break;
			}
			if (pouseKey_[0] && !pouseKey_[1])
			{
				waitTime_ = 0.0;
				TRACE("�X���[/�ꎞ��~�@����\n");
				pouseKey_[1] = pouseKey_[0];
			}
			endTime_ = std::chrono::system_clock::now();
		} while (std::chrono::duration_cast<std::chrono::milliseconds>(endTime_ - startTime_).count() < waitTime_ || waitTime_ < 0.0);
	}
	if (pouseKey_[0] && !pouseKey_[1])
	{
		TRACE("�ꎞ��~\n");
		waitTime_ = -1.0;
		pouseKey_[1] = pouseKey_[0];
	}

	endKey_[1] = endKey_[0];
	endKey_[0] = CheckHitKey(KEY_INPUT_DELETE);
	if (endKey_[0] && !endKey_[1])
	{
		clsFlag_ ^= 1;
		TRACE("�f�o�b�O�\���N���A�[�@�\�F%d\n",clsFlag_);
	}

	backSp_[1] = backSp_[0];
	backSp_[0] = CheckHitKey(KEY_INPUT_BACK);
	if (backSp_[0] && !backSp_[1])
	{
		SetAlwaysRunFlag(GetAlwaysRunFlag() ^ 1);
		TRACE("�o�b�N�O���E���h�����F%d\n", GetAlwaysRunFlag());
	}

}

void _DebugDispOut::SetMesPos(int x, int y)
{
	mesPosX_ = x;
	mesPosY_ = y;
}

int _DebugDispOut::DrawGraph(int x, int y, int GrHandle, int TransFlag)
{
	SetScreen();
	int rtnFlag = DxLib::DrawGraph(x , y , GrHandle, TransFlag);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawBox(int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag)
{
	SetScreen();
	int rtnFlag = DxLib::DrawBox(x1 , y1 , x2 , y2 , Color, FillFlag);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawString(int x, int y, char* String, unsigned int Color)
{
	SetScreen();
	int rtnFlag = DxLib::DrawString(x, y, String, Color);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawLine(int x1, int y1, int x2, int y2, unsigned int Color)
{
	SetScreen();
	int rtnFlag = DxLib::DrawLine(x1 , y1 , x2 , y2 , Color);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawLine3D(VECTOR pos1, VECTOR pos2, unsigned int Color)
{
	SetScreen();
	int rtnFlag = DxLib::DrawLine3D(pos1,pos2, Color);
	RevScreen();
	return rtnFlag;
}

int _DebugDispOut::DrawCircle(int x, int y, int r, unsigned int Color, int FillFlag)
{
	SetScreen();
	int rtnFlag = DxLib::DrawCircle(x , y , r, Color, FillFlag);
	RevScreen();
	return rtnFlag;
}

void _DebugDispOut::TRACE_S(int Color, std::string str, int num)
{
	SetScreen();
	auto length = GetDrawFormatStringWidth((str + ":%d").c_str(), num);
	DrawBox(mesPosX_, mesPosY_ + drawLineY_, mesPosX_ + length, mesPosY_ + drawLineY_ + 16, 0, true);
	DrawFormatString(mesPosX_, mesPosY_ + drawLineY_, Color, (str + ":%d").c_str(), num);
	RevScreen();
	drawLineY_ += 16;
}

void _DebugDispOut::TRACE_S(int Color, std::string str, double num)
{
	SetScreen();
	auto length = GetDrawFormatStringWidth((str + ":%f").c_str(), num);
	DrawBox(mesPosX_, mesPosY_ + drawLineY_, mesPosX_ + length, mesPosY_ + drawLineY_ +16, 0, true);
	DrawFormatString(mesPosX_, mesPosY_ + drawLineY_, Color, (str + ":%f").c_str(), num);
	RevScreen();
	drawLineY_ += 16;
}

int _DebugDispOut::DrawPixel(int x, int y, unsigned int Color)
{
	SetScreen();
	int rtnFlag = DxLib::DrawPixel(x , y , Color);
	RevScreen();
	return rtnFlag;
}

void _DebugDispOut::DrawFPS(void)
{
	fpsEndTime_ = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(fpsEndTime_ - fpsStartTime_).count() >= 1000)
	{
		fpsView_ = fpsCount_;
		fpsCount_ = 0;
		fpsStartTime_ = fpsEndTime_;
	}
	else
	{
		fpsCount_++;
	}
	_DebugDispOut::DrawBox(0, 0, 80, 24, 0, true);
	_dbgDrawFormatString(4, 4, 0xffffff, "fps:1/%d", fpsView_);
}

bool _DebugDispOut::StartDrawDebug(void)
{
	int ghBefor;
	ghBefor = GetDrawScreen();
	SetDrawScreen(DbgScreen_);
	if (clsFlag_)
	{
		ClsDrawScreen();
	}
	SetDrawScreen(ghBefor);
	drawLineY_ = 0;
	return true;
}

bool _DebugDispOut::DrawDebug(void)
{
	homeKey_[1] = homeKey_[0];
	homeKey_[0] = CheckHitKey(KEY_INPUT_HOME);
	if (homeKey_[0] && !homeKey_[1])
	{
		TRACE("�f�o�b�O�\���F%d\n", dispFlag_);
		dispFlag_ ^= 1;
	}
	if (dispFlag_)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DxLib::DrawGraph(0, 0, DbgScreen_, true);
		//lpSceneMng.AddDrawQue({ _DbgScreen,lpSceneMng.ScreenSize.x/2,lpSceneMng.ScreenSize.y / 2,0,INT_MAX,LAYER::UI,DX_BLENDMODE_NOBLEND,255 });
	}
	WaitMode();
	return true;
}

bool _DebugDispOut::SetAlpha(int alpha)
{
	_alpha = alpha;

	return true;
}

bool _DebugDispOut::Setup(int screenSizeX, int screenSizeY,int alpha)
{
	if (DbgScreen_ == -1)
	{
		DbgScreen_ = MakeScreen(screenSizeX, screenSizeY, true);
	}
	SetAlpha(alpha);
	return true;
}

bool _DebugDispOut::SetWait(double timeCnt)
{
	waitTime_ = timeCnt;
	return true;
}

#endif	// _DEBUG
