#pragma once
#include "Geometry.h"
#include "Collision2D.h"

class Raycast
{
public:
	//線分
	bool CheckCollision(Vector2 pos, Vector2 size, Collision stagepos, float& offset,int color);


private:

	//あたり判定
	bool CheckLine(Line playerLine, Line stageLine, float& offset);

};

