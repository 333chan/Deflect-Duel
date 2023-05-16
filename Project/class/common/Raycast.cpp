#include "Raycast.h"
#include "../object/player/Player.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::CheckCollision(Vector2 pos, Vector2 size, Collision stagepos, Dir dir,bool flg, Vector2& offset, int color)
{
	Line ray[4] =
	{
		{pos,{pos.x+size.x,pos.y} },			//上
		{{pos.x,pos.y + size.y},pos + size },	//下
		{pos,{pos.x,pos.y+size.y} },			//左
		{{pos.x+size.x,pos.y},pos + size },		//右
	};

	stageray =
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
	if (dir == Dir::Up)
	{
		collray = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x / 2,pos.y} };
	}
	if (dir == Dir::Down|| flg)
	{
		collray = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x / 2,pos.y + size.y} };

	}
	if (dir == Dir::Left|| flg)
	{
		collray = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x,pos.y + size.y / 2} };

	}
	if (dir == Dir::Right || flg)
	{
		collray = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x ,pos.y + size.y / 2 }  };

	}

	DrawLine(collray.p.x, collray.p.y,collray.end.x, collray.end.y,0xff0000,true);
	

	//PlayerRay
	for (const auto& r : ray)
	{
		//ステージレイ
		for (const auto& rs : stageray)
		{

			if (CheckLine(r, rs, dir, offset, collray))
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


bool Raycast::CheckLine(Line playerLine, Line stageLine, Dir dir_, Vector2& offset, Line collRay)
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

		//Hit時
		offset = { 0.0f,0.0f };

		if (dir_ == Dir::Left && collRay.end.x < stageLine.p.x && stageray[2] == stageLine)
		{
			offset.x = -abs(stageLine.p.x - collRay.end.x);//左
			DrawString(450, 0, "左判定", 0xffffff, true);
			return true;
		}
		if (dir_ == Dir::Right && collRay.end.x > stageLine.end.x && stageray[3] == stageLine)
		{
			offset.x = abs(stageLine.end.x - collRay.end.x);//右
			DrawString(400, 0, "右判定", 0xffffff, true);
			return true;
		}
		if (dir_ == Dir::Up&& stageLine.p.y < collRay.end.y)
		{
			offset.y = -abs(stageLine.p.y - collRay.p.y);//上
			DrawString(300, 0, "上判定", 0xffffff, true);
			return true;
		}
		if (dir_ == Dir::Down && stageLine.end.y < playerLine.end.y)
		{
			offset.y = abs(stageLine.end.y - collRay.end.y);//下
			DrawString(350, 0, "下判定", 0xffffff, true);
			return true;
		}

		
		return true;
	}
}

