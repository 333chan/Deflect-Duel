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
	/// �v���C���[�ƃX�e�[�W�̐����̍쐬
	/// </summary>
	/// <param name="stagepos">�X�e�[�W�̍��W</param>
	/// <param name="dir_">�����Ă������</param>
	/// <param name="collRay">�Ƃ΂������C</param>
	/// <param name="offset">����</param>
	/// <returns></returns>
	/// nodiscard:�֐��̖߂�l��S�ĕK���g�p���邱��
	[[nodiscard]] bool CheckCollision(Collision stagepos,  Vector2& offset);

	bool PlayerToBallColl(Vector2& offset);
	bool StageToBallColl(Collision stagepos, Vector2& offset);

	void setBallRay(Vector2 pos, Vector2 size);
	void setPlayerRay(Line ray);
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
	[[nodiscard]] bool CheckLine(Line playerLine, Line stageLine, Vector2& offset);

	void setStageRay(Collision stagepos);


	//�X�e�[�W���C
	std::vector<Line> stageray_;

	std::vector<Line> ballray_;

	Line playerray_;

};

