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

	bool PlayerToBallChackColl(Vector2& offset);
	bool StageToBallColl(Collision stagepos, Vector2& offset);

	void setBallRay(Vector2 pos, Vector2 size, Vector2 movepow);
	
	void setPlayerRay(Line ray);

	void setPlayerSquareRay(Vector2 pos, Vector2 size);
private:

	[[nodiscard]] bool StageToPlayerChackLine(Line playerLine, Line stageLine, Vector2& offset);
	[[nodiscard]] void ChackLine(Line playerLine, Line stageLine);

	[[nodiscard]] bool BallToPlayerChackLine(Line playerLine, Line ballLine, Vector2& bound);

	[[nodiscard]] bool BallToStageChackLine(Line playerLine, Line ballLine, Vector2& bound);

	void setStageRay(Collision stagepos);


	//�X�e�[�W���C
	std::vector<Line> stageRay_;

	std::vector<Line> ballRay_;

	Line playerRay_;

	std::vector<Line> playerSquareRay_;




	//�R�O���[�v�̃x�N�g�����쐬
	Vector2 a_to_b;
	Vector2 a_to_c;
	Vector2 a_to_d;

	Vector2 c_to_d;
	Vector2 c_to_a;
	Vector2 c_to_b;


	//�O�ςŌv�Za_to�n
	float cross_01;
	float cross_02;

	//�O�ςŌv�Zc_to�n
	float cross_03;
	float cross_04;

};

