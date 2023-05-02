#include "Raycast.h"
#include"../../_debug/_DebugDispOut.h"

/// <summary>
/// 線分の判定
/// </summary>
/// <param name="pos">座標</param>
/// <param name="size">大きさ</param>
/// <param name="stagepos">ステージの座標</param>
/// <param name="offset">差分</param>
/// <returns></returns>
bool Raycast::CheckCollision(Vector2 pos, Vector2 size, Collision stagepos ,float& offset,int color)
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
		{{stagepos.first.x,stagepos.second.y},stagepos.second},	//下
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//左
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//右
	};

	for (const auto& rs : stageray)
	{
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y, color);

	}
	
	for (const auto& r : ray)
	{
		for (const auto& rs : stageray)
		{
			if (CheckLine(r, rs, offset))
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

/// <summary>
/// 判定処理
/// </summary>
/// <param name="playerLine">プレイヤーのLine</param>
/// <param name="stageLine">stageのLine</param>
/// <param name="offset">差分</param>
/// <returns></returns>
bool Raycast::CheckLine(Line playerLine, Line stageLine,float& offset)
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
		offset = abs(stageLine.p.y - playerLine.end.y);
		return true;
	}
}
