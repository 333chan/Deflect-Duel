#pragma once
class Object
{
public:
	Object();
	~Object();

	virtual void Init(void) = 0;	//�������z�֐�
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;
private:
protected:

};

