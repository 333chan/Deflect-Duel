#include "Raycast.h"
#include "../object/player/Player.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::CheckCollision(Collision stagepos, Vector2& offset)
{
	//ステージ判定用レイの作成
	setStageRay(stagepos);

	for (const auto& rs : stageray_)
	{
		//デバック用判定ライン
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y,0xffffff);

	}

	for (const auto& rb : ballray_)
	{
		//デバック用判定ライン
		_dbgDrawLine(rb.p.x, rb.p.y, rb.end.x, rb.end.y, 0xff0000);

	}
	
	//ステージレイ分チェック
	for (const auto& rs : stageray_)
	{
		//プレイヤーとステージ
		if (CheckLine(playerray_, rs, offset))
		{
			//Hitしたら
			_dbgDrawFormatString(800, 0, 0xfffffff, "プレイヤーがステージにHit");
			return true;
		}

		
		for (const auto& rb : ballray_)
		{
			//ステージとボール
			if (CheckLine(rb, rs, offset))
			{
				//Hitしたら
				_dbgDrawFormatString(800, 20, 0xfffffff, "ボールがステージにHit");
				return true;
			}
			
		}
	}
	for (const auto& rb : ballray_)
	{
		//プレイヤーとボール
		if (CheckLine(playerray_, rb, offset))
		{
			//Hitしたら
			_dbgDrawFormatString(1000, 30, 0xfffffff, "プレイヤーがボールにHit");
			return true;
		}
	}

	//当たってない
	_dbgDrawFormatString(800, 40, 0xffffff, "はずれ");
	return false;
}

bool Raycast::CheckLine(Line playerLine, Line stageLine, Vector2& offset)
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


	//交差していない
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	//交差していない
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}


	{

		//交差している
		offset = { 0.0f,0.0f };

		if (stageray_[2] == stageLine)
		{
			offset.x = -abs(stageLine.p.x - playerLine.end.x);//左
			_dbgDrawFormatString(450, 0, 0xffffff, "左判定", true);
			return true;
		}
		if (stageray_[3] == stageLine)
		{
			offset.x = abs(stageLine.end.x - playerLine.end.x);//右
			_dbgDrawFormatString(450, 0, 0xffffff, "右判定", true);
			return true;
		}
		if (stageray_[0] == stageLine)
		{
			offset.y = -abs(stageLine.p.y - playerLine.p.y);//上
			_dbgDrawFormatString(450, 0, 0xffffff, "上判定", true);
			return true;
		}
		if (stageray_[1] == stageLine)
		{
			offset.y = abs(stageLine.end.y - playerLine.end.y);//下
			_dbgDrawFormatString(450, 0, 0xffffff, "下判定", true);
			return true;
		}

		return true;
	}
}

void Raycast::setStageRay(Collision stagepos)
{
	stageray_ =
	{
		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//上
		{{stagepos.first.x,stagepos.second.y},stagepos.second},			//下
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//左
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//右
	};
}

void Raycast::setBallRay(Vector2 pos, Vector2 size)
{
	ballray_ =
	{
		{{pos.x,pos.y},{pos.x + size.x,pos.y} },						//上
		{{pos.x,pos.y + size.y} ,{pos.x + size.x,pos.y + size.y}},		//下
		{{pos.x,pos.y},{pos.x,pos.y + size.y}},							//左
		{{pos.x + size.x,pos.y},{pos.x + size.x,pos.y + size.y}},		//右
	};
}

void Raycast::setPlayerRay(Line ray)
{
	playerray_ = ray;

	//レイのデバック表示
	_dbgDrawLine(ray.p.x, ray.p.y,ray.end.x, ray.end.y, 0xff0000);

}

