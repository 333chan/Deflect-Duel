#pragma once
class FpsController
{
public:

	FpsController();
	~FpsController();

	void Init();
	bool Updeta();
	void Draw();
	void Wait();

private:
	//計測開始時間
	int startTime_;	
	//カウンタ
	int count_;
	//fps
	float fps_;


};

