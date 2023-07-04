#include "Raycast.h"
#include "../object/player/Player.h"
#include "../object/ball/Ball.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::StageToPlayerCheckColl(Collision stagepos, Vector2& offset)
{
	//ステージ判定用レイの作成
	setStageRay(stagepos);

	//ステージレイ分チェック
	for (const auto& rs : stageRay_)
	{
		//デバック用判定ライン
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y, 0xffffff);

		//ステージとプレイヤー
		if (StageToPlayerChackLine(playerRay_, rs, offset))
		{
			//Hitしたら
			_dbgDrawFormatString(800, 0, 0xfffffff, "プレイヤーがステージにHit");
			return true;
		}
	}
	//当たってない
	_dbgDrawFormatString(800, 100, 0xffffff, "プレイヤーがステージに当たってない");
	return false;
}

bool Raycast::PlayerToBallChackColl(Vector2& offset)
{

	for (const auto& rps : playerSquareRay_)
	{
		//デバック用判定ライン


		for (const auto& rb : ballRay_)
		{
			//プレイヤーとボール
			if (BallToPlayerChackLine(rps, rb, offset))
			{
				//Hitしたら
				_dbgDrawFormatString(1000, 30, 0xfffffff, "プレイヤーがボールにHit");
				return true;
			}
		}
		_dbgDrawLine(rps.p.x, rps.p.y, rps.end.x, rps.end.y, 0xff0000);
	}
	//当たってない
	_dbgDrawFormatString(800, 80, 0xffffff, "プレイヤーがボールに当たらず");
	return false;
}

bool Raycast::StageToBallCheckColl(Collision stagepos, Vector2& offset, Vector2& refDir)
{
	//ステージ判定用レイの作成
	setStageRay(stagepos);

	for (const auto& rs : stageRay_)
	{
		//デバック用判定ライン
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y, 0xffffff);

		for (const auto& rb : ballRay_)
		{
			
			//デバック用判定ライン
			_dbgDrawLine(rb.p.x, rb.p.y, rb.end.x, rb.end.y, 0xff0000);
			//ボールとステージ
			if (BallToStageChackLine(rb, rs, offset, refDir))
			{
				//Hitしたら
				_dbgDrawFormatString(800, 20, 0xfffffff, "ボールがステージにHit");

				return true;
			}
		}
	}
	_dbgDrawFormatString(0, 350, 0xfffffff, "%f,%f,%f,%f", ballRay_[0].p.x, ballRay_[0].p.y);
	//当たってない
	_dbgDrawFormatString(800, 40, 0xffffff, "ボールがステージに当たらず");
	return false;
}

bool Raycast::AttackToBallCheckColl(Vector2& refDir)
{

	for (const auto& rpa : playerAttackRay_)
	{
		//デバック用判定ライン
		_dbgDrawLine(rpa.p.x, rpa.p.y, rpa.end.x, rpa.end.y, 0xff0000);

		for (const auto& rb : ballRay_)
		{
			//攻撃とボール
			if (AttackToBallChackLine(rpa, rb, refDir))
			{
				//Hitしたら
				_dbgDrawFormatString(1000, 100, 0xfffffff, "攻撃がボールにHit");
				return true;
			}
		}

	}
	//当たってない
	_dbgDrawFormatString(800, 100, 0xffffff, "攻撃がボールにあたらない");
	return false;
}

void Raycast::ChackLine(Line mainLine, Line subLine)
{
	//３グループのベクトルを作成
	a_to_b = mainLine.vec();
	a_to_c = subLine.p - mainLine.p;
	a_to_d = subLine.end - mainLine.p;

	c_to_d = subLine.vec();
	c_to_a = mainLine.p - subLine.p;
	c_to_b = mainLine.end - subLine.p;


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
	if (stageRay_[2] == stageLine)
	{
		offset.x = -abs(stageLine.p.x - playerLine.end.x);//左
		_dbgDrawFormatString(450, 0, 0xffffff, "左判定", true);
		return true;
	}
	if (stageRay_[3] == stageLine)
	{
		offset.x = abs(stageLine.end.x - playerLine.end.x);//右
		_dbgDrawFormatString(450, 0, 0xffffff, "右判定", true);
		return true;
	}
	if (stageRay_[0] == stageLine)
	{
		offset.y = -abs(stageLine.p.y - playerLine.p.y);//上
		_dbgDrawFormatString(450, 0, 0xffffff, "上判定", true);
		return true;
	}
	if (stageRay_[1] == stageLine)
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
	if (ballRay_[3] == ballLine)
	{
		//bound.x = -abs(playerLine.p.x - ballLine.end.x);//左
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール左", true);
		return true;
	}
	if (ballRay_[2] == ballLine)
	{
		//bound.x = abs(playerLine.end.x - ballLine.p.x);//右
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール右", true);
		return true;
	}
	if (ballRay_[1] == ballLine)
	{
		//bound.y = -abs(playerLine.p.y - ballLine.p.y);//上
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール上", true);
		return true;
	}
	if (ballRay_[0] == ballLine)
	{
		//bound.y = abs(playerLine.end.y - ballLine.end.y);//下
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール下", true);
		return true;
	}

	//return true;
}

bool Raycast::BallToStageChackLine(Line ballLine, Line stageLine, Vector2& offset, Vector2& refDir)
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
	//refDir = { 0.0f,0.0f };

	//ステージとボール
	if (stageRay_[3] >= stageLine&& stageRay_[2] >= stageLine)
	{
		offset.x = -abs(stageLine.p.x - ballLine.p.x);//左
		//offset.x = 20;//左
		refDir.x = 1;
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール左判定", true);
		//return true;
	}
	if (stageRay_[2] <= stageLine && stageRay_[3] <= stageLine)
	{
		offset.x =abs(stageLine.end.x - ballLine.end.x);//右
		//offset.x =-20;//右
		refDir.x = -1;
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール右判定", true);
		//return true;
	}
	if (stageRay_[0] == stageLine)
	{
		offset.y = -abs(stageLine.p.y - ballLine.p.y);//上
		//offset.y = 20;//上
		refDir.y = 1;
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール上判定", true);
		//return true;
	}
	if (stageRay_[1] == stageLine)
	{
		offset.y = abs(stageLine.end.y - ballLine.end.y);//下
		//offset.y = -20;//下
		refDir.y=-1;
		_dbgDrawFormatString(600, 0, 0xffffff, "ボール下判定", true);
		//return true;
	}
	return true;
}

bool Raycast::AttackToBallChackLine(Line playerLine, Line ballLine, Vector2& refDir)
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
	refDir = { 0.0f,0.0f };

	//攻撃とボール
	if (playerAttackRay_[3] == playerLine)
	{
		refDir.x = 1;//左
		_dbgDrawFormatString(600, 0, 0xffffff, "左に返す", true);
		return true;
	}
	if (playerAttackRay_[2] == playerLine)
	{
		refDir.x = -1;//右
		_dbgDrawFormatString(600, 0, 0xffffff, "右に返す", true);
		return true;
	}
	if (playerAttackRay_[0] == playerLine)
	{
		refDir.y = -1;//上
		_dbgDrawFormatString(600, 0, 0xffffff, "上に返す", true);
		return true;
	}
	if (playerAttackRay_[1] == playerLine)
	{
		refDir.y = 1;//下
		_dbgDrawFormatString(600, 0, 0xffffff, "下に返す", true);
		return true;
	}

	return true;

}

void Raycast::setStageRay(Collision stagepos)
{
	stageRay_ =
	{
		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//上
		{{stagepos.first.x,stagepos.second.y},stagepos.second},			//下
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//左
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//右
	};
}



void Raycast::setBallRay(Vector2 pos, Vector2 size)
{
	ballRay_ =
	{
		{{pos.x,pos.y},{pos.x+ size.x,pos.y} },						//上
		{{pos.x,pos.y+ size.y} ,{pos.x + size.x,pos.y + size.y}},	//下
		{{pos.x,pos.y},{pos.x,pos.y + size.y}},						//左
		{{pos.x+ size.x,pos.y},{pos.x + size.x,pos.y + size.y}},	//右
	};
}

void Raycast::setPlayerRay(Line ray)
{
	playerRay_ = ray;

	//レイのデバック表示
	//_dbgDrawLine(ray.p.x, ray.p.y,ray.end.x, ray.end.y, 0xff0000);

}

void Raycast::setPlayerSquareRay(Vector2 pos, Vector2 size,Vector2 movePos)
{
	//playerSquareRay_ = 
	//{
	//	{{pos.x + movePos.x,pos.y + movePos.y },{pos.x  + size.x + movePos.x,pos.y + movePos.y } },						//上
	//	{{pos.x + movePos.x,pos.y + size.y + movePos.y} ,{pos.x + size.x + movePos.x,pos.y + size.y + movePos.y}},		//下
	//	{{pos.x + movePos.x,pos.y + movePos.y},{pos.x + movePos.x,pos.y + size.y + movePos.y }},							//左
	//	{{pos.x + size.x + movePos.x,pos.y + movePos.y},{pos.x + size.x + movePos.x,pos.y + size.y + movePos.y }},		//右
	//};

	playerSquareRay_ =
	{
		{{pos.x ,pos.y },{pos.x + size.x ,pos.y} },						//上
		{{pos.x ,pos.y + size.y} ,{pos.x + size.x,pos.y + size.y}},		//下
		{{pos.x ,pos.y},{pos.x,pos.y + size.y}},						//左
		{{pos.x + size.x,pos.y},{pos.x + size.x,pos.y + size.y}},		//右
	};

}

void Raycast::setPlayerAttackRay(Vector2 pos, Vector2 size)
{
	playerAttackRay_ =
	{
		{{pos.x ,pos.y },{pos.x + size.x,pos.y } },							//上
		{{pos.x ,pos.y + size.y } ,{pos.x + size.x ,pos.y + size.y}},		//下
		{{pos.x ,pos.y },{pos.x ,pos.y + size.y }},							//左
		{{pos.x + size.x ,pos.y },{pos.x + size.x,pos.y + size.y }},		//右
	};
}


