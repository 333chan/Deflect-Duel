#pragma once
#include "Geometry.h"
#include "Collision2D.h"

class Raycast
{
public:
	//ü•ª
	bool CheckCollision(Vector2 pos, Vector2 size, Collision stagepos, float& offset,int color);


private:

	//‚ ‚½‚è”»’è
	bool CheckLine(Line playerLine, Line stageLine, float& offset);

};

