#pragma once
#include "Geometry.h"
#include "Collision2D.h"

class Raycast
{
public:
	//����
	bool CheckCollision(Vector2 pos, Vector2 size, Collision stagepos, float& offset,int color);


private:

	//�����蔻��
	bool CheckLine(Line playerLine, Line stageLine, float& offset);

};

