#include "Raycast.h"
#include "../object/player/Player.h"
#include "../object/ball/Ball.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::StageToPlayerCheckColl(Collision stagepos, Vector2& offset)
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
		//ステージとプレイヤー
		if (StageToPlayerChackLine(playerray_, rs, offset))
		{
			//Hitしたら
			_dbgDrawFormatString(800, 0, 0xfffffff, "プレイヤーがステージにHit");
			return true;
		}
	}
	//当たってない
	_dbgDrawFormatString(800, 40, 0xffffff, "プレイヤーがステージに当たってない");
	return false;
}

bool Raycast::PlayerToBallChackColl(Vector2& offset)
{

	for (const auto& rb : ballray_)
	{
		//プレイヤーとボール
		if (BallToPlayerChackLine(playerray_, rb, offset))
		{
			//Hitしたら
			_dbgDrawFormatString(1000, 30, 0xfffffff, "プレイヤーがボールにHit");
			return true;
		}
	}
	_dbgDrawFormatString(800, 80, 0xffffff, "プレイヤーがボールに当たらず");
	return false;
}

bool Raycast::StageToBallColl(Collision stagepos, Vector2& offset)
{
	//ステージ判定用レイの作成
	setStageRay(stagepos);

	for (const auto& rs : stageray_)
	{
		for (const auto& rb : ballray_)
		{
			//ボールとステージ
			if (BallToStageChackLine(rb, rs, offset))
			{
				//Hitしたら
				_dbgDrawFormatString(800, 20, 0xfffffff, "ボールがステージにHit");
				return true;
			}

		}
	}
	_dbgDrawFormatString(800, 40, 0xffffff, "ボールがステージに当たらず");
	return false;
}

void Raycast::ChackLine(Line playerLine, Line stageLine)
{
	//３グループのベクトルを作成
	a_to_b = playerLine.vec();
	a_to_c = stageLine.p - playerLine.p;
	a_to_d = stageLine.end - playerLine.p;

	c_to_d = stageLine.vec();
	c_to_a = playerLine.p - stageLine.p;
	c_to_b = playerLine.end - stageLine.p;


	//外積で計算a_to系
	cross_01 = (a_to_b.x * a_to_c.y) - (a_to_c.x * a_to_b.y);
	cross_02 = (a_to_b.x * a_to_d.y) - (a_to_d.x * a_to_b.y);

	//外積で計算c_to系
	cross_03 = (c_to_d.x * c_to_a.y) - (c_to_a.x * c_to_d.y);
	cross_04 = (c_to_d.x * c_to_b.y) - (c_to_b.x * c_to_d.y);
}

bool Raycast::StageToPlayerChackLine(Line playerLine, Line stageLine, Vector2& offset)
{
	ChackLine(playerLine,stageLine);

	//交差していない
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}

	//交差している
	offset = { 0.0f,0.0f };

	//ステージとプレイヤー
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

bool Raycast::BallToPlayerChackLine(Line playerLine, Line ballLine, Vector2& bound)
{
	ChackLine(playerLine, ballLine);

	//交差していない
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}

	//交差している
	bound = { 0.0f,0.0f };

	//ボールとプレイヤー
	if (ballray_[3] == ballLine)
	{
		//bound.x = -abs(playerLine.p.x - ballLine.end.x);//左
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール左", true);
		return true;
	}
	if (ballray_[2] == ballLine)
	{
		//bound.x = abs(playerLine.end.x - ballLine.p.x);//右
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール右", true);
		return true;
	}
	if (ballray_[1] == ballLine)
	{
		//bound.y = -abs(playerLine.p.y - ballLine.p.y);//上
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール上", true);
		return true;
	}
	if (ballray_[0] == ballLine)
	{
		//bound.y = abs(playerLine.end.y - ballLine.end.y);//下
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール下", true);
		return true;
	}

	return true;
}

bool Raycast::BallToStageChackLine(Line ballLine, Line stageLine, Vector2& offset)
{
	ChackLine(ballLine, stageLine);

	//交差していない
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}

	//交差している
	offset = { 0.0f,0.0f };

	//ステージとボール
	if (stageray_[3] == stageLine)
	{
		offset.x = -abs(stageLine.p.x - ballLine.end.x);//左
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール左判定", true);
		return true;
	}
	if (stageray_[2] == stageLine)
	{
		offset.x = abs(stageLine.end.x - ballLine.end.x);//右
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール右判定", true);
		return true;
	}
	if (stageray_[0] == stageLine)
	{
		offset.y = -abs(stageLine.p.y - ballLine.p.y);//上
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール上判定", true);
		return true;
	}
	if (stageray_[1] == stageLine)
	{
		offset.y = abs(stageLine.end.y - ballLine.end.y);//下
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール下判定", true);
		return true;
	}

	return true;

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


