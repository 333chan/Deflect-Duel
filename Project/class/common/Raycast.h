#pragma once
#include "Geometry.h"
#include "Collision2D.h"
#include <vector>
#include<string>
enum class Dir;
enum class State;

class Raycast
{
public:

	[[nodiscard]] bool StageToPlayerCheckColl(Collision stagepos,  Vector2& offset);
	[[nodiscard]] bool PlayerToBallChackColl(Vector2& offset);
	[[nodiscard]] bool StageToBallCheckColl(Collision stagepos, Vector2& offset,Vector2& refDir);
	[[nodiscard]] bool AttackToBallCheckColl(Vector2& refDir);

	void setBallRay(Vector2 pos, Vector2 size);
	
	void setPlayerRay(Line ray);

	void setPlayerSquareRay(Vector2 pos, Vector2 size, Vector2 moveSpeed);

	void setPlayerAttackRay(Vector2 pos, Vector2 size);


	std::vector<Line> ballRay_;


private:

	[[nodiscard]] void ChackLine(Line playerLine, Line stageLine);

	[[nodiscard]] bool StageToPlayerChackLine(Line playerLine, Line stageLine, Vector2& offset);
	[[nodiscard]] bool BallToPlayerChackLine(Line playerLine, Line ballLine, Vector2& bound);
	[[nodiscard]] bool BallToStageChackLine(Line playerLine, Line ballLine, Vector2& offset,Vector2& refDir);
	[[nodiscard]] bool AttackToBallChackLine(Line playerLine, Line ballLine, Vector2& refDir);

	void setStageRay(Collision stagepos);


	//ステージレイ
	std::vector<Line> stageRay_;



	Line playerRay_;

	std::vector<Line> playerSquareRay_;
	std::vector<Line> playerAttackRay_;


	//３グループのベクトルを作成
	Vector2 a_to_b;
	Vector2 a_to_c;
	Vector2 a_to_d;

	Vector2 c_to_d;
	Vector2 c_to_a;
	Vector2 c_to_b;


	//外積で計算a_to系
	float cross_01;
	float cross_02;

	//外積で計算c_to系
	float cross_03;
	float cross_04;

};

