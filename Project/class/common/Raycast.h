#pragma once
#include "Geometry.h"
#include "Collision2D.h"
#include <vector>
enum class Dir;
enum class State;

class Raycast
{
public:
	/// <summary>
	/// 線分の判定
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="stagepos">ステージの座標</param>
	/// <param name="state">状態</param>
	/// <param name="offset">差分</param>
	/// <returns></returns>
	[[nodiscard]] bool CheckCollision(Vector2 pos, Vector2 size, Collision stagepos, Dir dir_, bool flg, Vector2& offset, int color);


private:

	/// <summary>
	/// 判定処理
	/// </summary>
	/// <param name="playerLine">プレイヤーのLine</param>
	/// <param name="stageLine">stageのLine</param>
	/// <param name="state">状態</param>
	/// <param name="offset">差分</param>
	/// <returns></returns>
	[[nodiscard]] bool CheckLine(Line playerLine, Line stageLine, Dir dir_, Vector2& offset, Line collRay);
	std::vector<Line> stageray;
};

