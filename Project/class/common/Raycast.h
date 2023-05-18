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
	/// �����̍쐬
	/// </summary>
	/// <param name="stagepos">�X�e�[�W�̍��W</param>
	/// <param name="dir_">�����Ă������</param>
	/// <param name="collRay">�Ƃ΂������C</param>
	/// <param name="offset">����</param>
	/// <returns></returns>
	/// nodiscard:�֐��̖߂�l��S�ĕK���g�p���邱��
	[[nodiscard]] bool CheckCollision(Collision stagepos, Dir dir_, Line collRay, Vector2& offset);


private:

	/// <summary>
	/// ����v�Z�ƕ␳���v�Z
	/// </summary>
	/// <param name="playerLine">�v���C���[�̃��C</param>
	/// <param name="stageLine">�X�e�[�W�̃��C</param>
	/// <param name="dir_">�����Ă������</param>
	/// <param name="offset">����</param>
	/// <returns></returns>
	/// nodiscard:�֐��̖߂�l��S�ĕK���g�p���邱��
	[[nodiscard]] bool CheckLine(Line playerLine, Line stageLine, Dir dir_, Vector2& offset);

	//�X�e�[�W���C
	std::vector<Line> stageray;

};

