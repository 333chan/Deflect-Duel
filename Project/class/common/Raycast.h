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
	/// �����̔���
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="size">�傫��</param>
	/// <param name="stagepos">�X�e�[�W�̍��W</param>
	/// <param name="state">���</param>
	/// <param name="offset">����</param>
	/// <returns></returns>
	[[nodiscard]] bool CheckCollision(Vector2 pos, Vector2 size, Collision stagepos, Dir dir_, bool flg, Vector2& offset, int color);


private:

	/// <summary>
	/// ���菈��
	/// </summary>
	/// <param name="playerLine">�v���C���[��Line</param>
	/// <param name="stageLine">stage��Line</param>
	/// <param name="state">���</param>
	/// <param name="offset">����</param>
	/// <returns></returns>
	[[nodiscard]] bool CheckLine(Line playerLine, Line stageLine, Dir dir_, Vector2& offset, Line collRay);
	std::vector<Line> stageray;
};

