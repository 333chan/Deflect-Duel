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

	/// <summary>
	/// プレイヤーとステージの線分の作成
	/// </summary>
	/// <param name="stagepos">ステージの座標</param>
	/// <param name="dir_">向いている方向</param>
	/// <param name="collRay">とばしたレイ</param>
	/// <param name="offset">差分</param>
	/// <returns></returns>
	/// nodiscard:関数の戻り値を全て必ず使用すること
	[[nodiscard]] bool CheckCollision(Collision stagepos,  Vector2& offset);

	bool PlayerToBallColl(Vector2& offset);
	bool StageToBallColl(Collision stagepos, Vector2& offset);

	void setBallRay(Vector2 pos, Vector2 size);
	void setPlayerRay(Line ray);
private:

	/// <summary>
	/// 判定計算と補正分計算
	/// </summary>
	/// <param name="playerLine">プレイヤーのレイ</param>
	/// <param name="stageLine">ステージのレイ</param>
	/// <param name="dir_">向いている方向</param>
	/// <param name="offset">差分</param>
	/// <returns></returns>
	/// nodiscard:関数の戻り値を全て必ず使用すること
	[[nodiscard]] bool CheckLine(Line playerLine, Line stageLine, Vector2& offset);

	void setStageRay(Collision stagepos);


	//ステージレイ
	std::vector<Line> stageray_;

	std::vector<Line> ballray_;

	Line playerray_;

};

