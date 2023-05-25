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
	[[nodiscard]] bool CheckCollision(Collision stagepos, Dir dir_, Line collRay, Vector2& offset);

	[[nodiscard]] bool CheckStagetoBallCollision(Collision stagepos, Vector2& ballpos, Vector2& size_, Vector2& offset);

	[[nodiscard]] bool CheckStagetoBallCollision(Line collRay, Vector2& ballpos, Vector2& size_, Vector2& offset);


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
	[[nodiscard]] bool CheckLine(Line playerLine, Line stageLine, Dir dir_, Vector2& offset);
	[[nodiscard]] bool ChecBallLine(Line ballLine, Line stageLine, Vector2& offset);

	//ステージレイ
	std::vector<Line> stageray;

	std::vector<Line> ballray;

};

