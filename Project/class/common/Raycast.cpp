#include "Raycast.h"
#include "../object/player/Player.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::CheckCollision(Collision stagepos, Dir dir,Line collRay, Vector2& offset)
{
	//�X�e�[�W����p���C�̍쐬
	stageray =
	{
		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//��
		{{stagepos.first.x,stagepos.second.y},stagepos.second},			//��
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//��
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//�E
	};

	for (const auto& rs : stageray)
	{
		//�f�o�b�N�p���胉�C��
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y,0xffffff);

	}
	
	//�X�e�[�W���C���`�F�b�N
	for (const auto& rs : stageray)
	{
		if (CheckLine(collRay, rs, dir, offset))
		{
			//Hit������
			_dbgDrawFormatString(100, 0, 0xfffffff, "Hit");
			return true;
		}
	}
	//�������ĂȂ�
	_dbgDrawFormatString(100, 30, 0xffffff, "�͂���");
	return false;
}

bool Raycast::CheckStagetoBallCollision(Collision stagepos, Vector2& ballpos, Vector2& size_, Vector2& offset)
{
	//�X�e�[�W����p���C�̍쐬
	stageray =
	{
		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//��
		{{stagepos.first.x,stagepos.second.y},stagepos.second},			//��
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//��
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//�E
	};

	//�{�[���̃��C�̍쐬
	ballray =
	{
		{{ballpos.x,ballpos.y},{ballpos.x + size_.x,ballpos.y} },			//��
		{{ballpos.x,ballpos.y+size_.y} ,{ballpos.x+size_.x,ballpos.y+size_.y}},			//��
		{{ballpos.x,ballpos.y},{ballpos.x,ballpos.y+size_.y}},			//��
		{{ballpos.x+size_.x,ballpos.y},{ballpos.x+size_.x,ballpos.y + size_.y}},		//�E
	};

	//�X�e�[�W���C���`�F�b�N
	for (const auto& r : ballray) {
		for (const auto& rs : stageray)
		{
			if (ChecBallLine(r, rs, offset))
			{
				//Hit������
				_dbgDrawFormatString(100, 0, 0xfffffff, "Hit");
				return true;
			}
		}

	}

	//�������ĂȂ�
	_dbgDrawFormatString(100, 30, 0xffffff, "�͂���");
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


	//�������Ă��Ȃ�
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	//�������Ă��Ȃ�
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}


	{

		//�������Ă���
		offset = { 0.0f,0.0f };

		if (dir_ == Dir::Left && stageray[2] == stageLine)
		{
			offset.x = -abs(stageLine.p.x - playerLine.end.x);//��
			_dbgDrawFormatString(450, 0, 0xffffff, "������", true);
			return true;
		}
		if (dir_ == Dir::Right&& stageray[3] == stageLine)
		{
			offset.x = abs(stageLine.end.x - playerLine.end.x);//�E
			_dbgDrawFormatString(450, 0, 0xffffff, "�E����", true);
			return true;
		}
		if (dir_ == Dir::Up&& stageray[0] == stageLine)
		{
			offset.y = -abs(stageLine.p.y - playerLine.p.y);//��
			_dbgDrawFormatString(450, 0, 0xffffff, "�㔻��", true);
			return true;
		}
		if (dir_ == Dir::Down&& stageray[1] == stageLine)
		{
			offset.y = abs(stageLine.end.y - playerLine.end.y);//��
			_dbgDrawFormatString(450, 0, 0xffffff, "������", true);
			return true;
		}

		return true;
	}
}

bool Raycast::ChecBallLine(Line ballLine, Line stageLine, Vector2& offset)
{
	//�R�O���[�v�̃x�N�g�����쐬
	Vector2 a_to_b = ballLine.vec();
	Vector2 a_to_c = stageLine.p - ballLine.p;
	Vector2 a_to_d = stageLine.end - ballLine.p;

	Vector2 c_to_d = stageLine.vec();
	Vector2 c_to_a = ballLine.p - stageLine.p;
	Vector2 c_to_b = ballLine.end - stageLine.p;


	//�O�ςŌv�Za_to�n
	float cross_01 = (a_to_b.x * a_to_c.y) - (a_to_c.x * a_to_b.y);
	float cross_02 = (a_to_b.x * a_to_d.y) - (a_to_d.x * a_to_b.y);

	//�O�ςŌv�Zc_to�n
	float cross_03 = (c_to_d.x * c_to_a.y) - (c_to_a.x * c_to_d.y);
	float cross_04 = (c_to_d.x * c_to_b.y) - (c_to_b.x * c_to_d.y);


	//�������Ă��Ȃ�
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	//�������Ă��Ȃ�
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}


	{

		//�������Ă���
		offset = { 0.0f,0.0f };

		//if (stageray[2] == ballray[2])
		//{
		//	offset.x = -abs(stageLine.p.x - ballLine.end.x);//��
		//	_dbgDrawFormatString(450, 0, 0xffffff, "������", true);
		//	return true;
		//}
		//if (stageray[3] == ballray[3])
		//{
		//	offset.x = abs(stageLine.end.x - ballLine.end.x);//�E
		//	_dbgDrawFormatString(450, 0, 0xffffff, "�E����", true);
		//	return true;
		//}
		//if (stageray[0] == ballray[0])
		//{
		//	offset.y = -abs(stageLine.p.y - ballLine.p.y);//��
		//	_dbgDrawFormatString(450, 0, 0xffffff, "�㔻��", true);
		//	return true;
		//}

		//if (stageray[1] == ballray[1])
		//{
		//	offset.y = abs(stageLine.end.y - ballLine.end.y);//��
		//	_dbgDrawFormatString(450, 0, 0xffffff, "������", true);
		//	return true;
		//}

		return true;
	}
}

