#include "Raycast.h"
#include "../object/player/Player.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::CheckCollision(Vector2 pos, Vector2 size, Collision stagepos, Dir dir,Line collRay, Vector2& offset, int color)
{
	Line ray[4] =
	{
		{pos,{pos.x+size.x,pos.y} },			//��
		{{pos.x,pos.y + size.y},pos + size },	//��
		{pos,{pos.x,pos.y+size.y} },			//��
		{{pos.x+size.x,pos.y},pos + size },		//�E
	};

	stageray =
	{

		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//��
		{{stagepos.first.x,stagepos.second.y},stagepos.second},			//��
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//��
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//�E
	};

	for (const auto& rs : stageray)
	{
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y, color);
	}

	//����p���C
	//if (dir == Dir::Up)
	//{
	//	collRay = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x / 2,pos.y} };
	//}
	//if (dir == Dir::Down)
	//{
	//	collRay = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x / 2,pos.y + size.y} };

	//}
	//if (dir == Dir::Left)
	//{
	//	collRay = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x,pos.y + size.y / 2} };

	//}
	//if (dir == Dir::Right)
	//{
	//	collRay = { {pos.x + size.x / 2,pos.y + size.y / 2},{pos.x + size.x ,pos.y + size.y / 2 }  };

	//}

	//DrawLine(collRay.p.x, collRay.p.y, collRay.end.x, collRay.end.y,0xff0000,true);
	

	//PlayerRay
	//for (const auto& r : ray)
	{
		//�X�e�[�W���C
		for (const auto& rs : stageray)
		{

			if (CheckLine(collRay, rs, dir, offset))
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


bool Raycast::CheckLine(Line playerLine, Line stageLine, Dir dir_, Vector2& offset)
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

		//Hit��
		offset = { 0.0f,0.0f };

		if (dir_ == Dir::Left /*&& collRay.end.x < stageLine.p.x*/ && stageray[2] == stageLine)
		{
			offset.x = -abs(stageLine.p.x - playerLine.end.x);//��
			DrawString(450, 0, "������", 0xffffff, true);
			return true;
		}
		if (dir_ == Dir::Right /*&& collRay.end.x > stageLine.end.x*/ && stageray[3] == stageLine)
		{
			offset.x = abs(stageLine.end.x - playerLine.end.x);//�E
			DrawString(400, 0, "�E����", 0xffffff, true);
			return true;
		}
		if (dir_ == Dir::Up/*&& stageLine.p.y < collRay.end.y*/ && stageray[0] == stageLine)
		{
			offset.y = -abs(stageLine.p.y - playerLine.p.y);//��
			DrawString(300, 0, "�㔻��", 0xffffff, true);
			return true;
		}
		if (dir_ == Dir::Down /*&& stageLine.end.y < playerLine.end.y*/ && stageray[1] == stageLine)
		{
			offset.y = abs(stageLine.end.y - playerLine.end.y);//��
			DrawString(350, 0, "������", 0xffffff, true);
			return true;
		}

		return true;
	}
}

