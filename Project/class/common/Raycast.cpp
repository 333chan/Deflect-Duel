#include "Raycast.h"
#include "../object/player/Player.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::CheckCollision(Vector2 pos, Vector2 size, Collision stagepos, State state, Vector2& offset, int color)
{
	Line ray[4] =
	{
		{pos,{pos.x+size.x,pos.y} },			//上
		{{pos.x,pos.y + size.y},pos + size },	//下
		{pos,{pos.x,pos.y+size.y} },			//左
		{{pos.x+size.x,pos.y},pos + size },		//右
	};

	Line stageray[4] =
	{

		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//上
		{{stagepos.first.x,stagepos.second.y},stagepos.second},			//下
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//左
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//右
	};

	for (const auto& rs : stageray)
	{
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y, color);
	}

	//判定用レイ
	Line collray;
	if (state == State::JumpUp)
	{
		collray = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x / 2,pos.y} };
	}
	else if (state == State::Fall || state == State::Crouching)
	{
		collray = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x / 2,pos.y + size.y} };

	}
	else if (state == State::MoveLeft)
	{
		collray = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x,pos.y + size.y / 2} };

	}
	else if (state == State::MoveRight)
	{
		collray = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x,pos.y + size.y / 2} };

	}

	DrawLine(collray.p.x, collray.p.y,collray.end.x, collray.end.y,0xff0000,true);
	

	for (const auto& r : ray)
	{
		for (const auto& rs : stageray)
		{

			if (CheckLine(r, rs, state, offset, collray))
			{
				//Hitしたら
				DrawString(100, 0, "Hit", 0xffffff);
				
				
				color = GetColor(255, 0, 0);
				return true;
			}
			else
			{
				color = GetColor(255, 255, 0);
			}
		}
	}




	//当たってない
	DrawString(100, 0, "はずれ", 0xffffff);
	return false;
}


bool Raycast::CheckLine(Line playerLine, Line stageLine, State state, Vector2& offset, Line collRay)
{

	//３グループのベクトルを作成
	Vector2 a_to_b = playerLine.vec();
	Vector2 a_to_c = stageLine.p - playerLine.p;
	Vector2 a_to_d = stageLine.end - playerLine.p;

	Vector2 c_to_d = stageLine.vec();
	Vector2 c_to_a = playerLine.p - stageLine.p;
	Vector2 c_to_b = playerLine.end - stageLine.p;


	//外積で計算a_to系
	float cross_01 = (a_to_b.x * a_to_c.y) - (a_to_c.x * a_to_b.y);
	float cross_02 = (a_to_b.x * a_to_d.y) - (a_to_d.x * a_to_b.y);

	//外積で計算c_to系
	float cross_03 = (c_to_d.x * c_to_a.y) - (c_to_a.x * c_to_d.y);
	float cross_04 = (c_to_d.x * c_to_b.y) - (c_to_b.x * c_to_d.y);



	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}

	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}


	{

		//Hit
		offset = { 0.0f,0.0f };

		if (state == State::MoveLeft && collRay.end.x < stageLine.p.x)
		{
			offset.x = -abs(stageLine.p.x - playerLine.p.x);//左
			DrawString(450, 0, "左判定", 0xffffff, true);
			return true;
		}
		else if (state == State::MoveRight && collRay.end.x > stageLine.end.x)
		{
			offset.x = abs(stageLine.end.x - playerLine.end.x);//右
			DrawString(400, 0, "右判定", 0xffffff, true);
			return true;
		}
		else if (state == State::JumpUp && collRay.end.y < stageLine.p.y)
		{
			//offset.y = -abs(stageLine.p.y - playerLine.p.y);//上
			DrawString(300, 0, "上判定", 0xffffff, true);
			return true;
		}
		else if (state == State::Fall || state == State::Crouching && stageLine.end.y < playerLine.end.y)
		{
			//offset.y = abs(stageLine.end.y - playerLine.end.y);//下
			DrawString(350, 0, "下判定", 0xffffff, true);
			return true;
		}

		
		return true;
	}
}

