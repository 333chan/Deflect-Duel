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
	//�v���J�n����
	int startTime_;	
	//�J�E���^
	int count_;
	//fps
	float fps_;


};

