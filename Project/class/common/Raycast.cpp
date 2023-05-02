#include "Raycast.h"
#include"../../_debug/_DebugDispOut.h"

/// <summary>
/// �����̔���
/// </summary>
/// <param name="pos">���W</param>
/// <param name="size">�傫��</param>
/// <param name="stagepos">�X�e�[�W�̍��W</param>
/// <param name="offset">����</param>
/// <returns></returns>
bool Raycast::CheckCollision(Vector2 pos, Vector2 size, Collision stagepos ,float& offset,int color)
{
	Line ray[4] =
	{
		{pos,{pos.x+size.x,pos.y} },			//��
		{{pos.x,pos.y + size.y},pos + size },	//��
		{pos,{pos.x,pos.y+size.y} },			//��
		{{pos.x+size.x,pos.y},pos + size },		//�E
	};

	Line stageray[4] =
	{

		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//��
		{{stagepos.first.x,stagepos.second.y},stagepos.second},	//��
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//��
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//�E
	};

	for (const auto& rs : stageray)
	{
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y, color);

	}
	
	for (const auto& r : ray)
	{
		for (const auto& rs : stageray)
		{
			if (CheckLine(r, rs, offset))
			{
				//Hit������
				DrawString(100, 0, "Hit", 0xffffff);
				color = GetColor(255, 0, 0);
				return true;
			}
			else
			{
				color = GetColor(255, 255, 0);
			}
		}
	}



	//�������ĂȂ�
	DrawString(100, 0, "�͂���", 0xffffff);
	return false;
}

/// <summary>
/// ���菈��
/// </summary>
/// <param name="playerLine">�v���C���[��Line</param>
/// <param name="stageLine">stage��Line</param>
/// <param name="offset">����</param>
/// <returns></returns>
bool Raycast::CheckLine(Line playerLine, Line stageLine,float& offset)
{

	//�R�O���[�v�̃x�N�g�����쐬
	Vector2 a_to_b = playerLine.vec();
	Vector2 a_to_c = stageLine.p - playerLine.p;
	Vector2 a_to_d = stageLine.end - playerLine.p;

	Vector2 c_to_d = stageLine.vec();
	Vector2 c_to_a = playerLine.p - stageLine.p;
	Vector2 c_to_b = playerLine.end - stageLine.p;


	//�O�ςŌv�Za_to�n
	float cross_01 = (a_to_b.x * a_to_c.y) - (a_to_c.x * a_to_b.y);
	float cross_02 = (a_to_b.x * a_to_d.y) - (a_to_d.x * a_to_b.y);

	//�O�ςŌv�Zc_to�n
	float cross_03 = (c_to_d.x * c_to_a.y) - (c_to_a.x * c_to_d.y);
	float cross_04 = (c_to_d.x * c_to_b.y) - (c_to_b.x * c_to_d.y);


	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}

	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}

	{
		//Hit
		offset = abs(stageLine.p.y - playerLine.end.y);
		return true;
	}
}
