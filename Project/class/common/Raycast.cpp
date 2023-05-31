#include "Raycast.h"
#include "../object/player/Player.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::CheckCollision(Collision stagepos, Vector2& offset)
{
	//�X�e�[�W����p���C�̍쐬
	setStageRay(stagepos);

	for (const auto& rs : stageray_)
	{
		//�f�o�b�N�p���胉�C��
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y,0xffffff);

	}

	for (const auto& rb : ballray_)
	{
		//�f�o�b�N�p���胉�C��
		_dbgDrawLine(rb.p.x, rb.p.y, rb.end.x, rb.end.y, 0xff0000);

	}
	
	//�X�e�[�W���C���`�F�b�N
	for (const auto& rs : stageray_)
	{
		//�v���C���[�ƃX�e�[�W
		if (CheckLine(playerray_, rs, offset))
		{
			//Hit������
			_dbgDrawFormatString(800, 0, 0xfffffff, "�v���C���[���X�e�[�W��Hit");
			return true;
		}

		
		for (const auto& rb : ballray_)
		{
			//�X�e�[�W�ƃ{�[��
			if (CheckLine(rb, rs, offset))
			{
				//Hit������
				_dbgDrawFormatString(800, 20, 0xfffffff, "�{�[�����X�e�[�W��Hit");
				return true;
			}
			
		}
	}
	for (const auto& rb : ballray_)
	{
		//�v���C���[�ƃ{�[��
		if (CheckLine(playerray_, rb, offset))
		{
			//Hit������
			_dbgDrawFormatString(1000, 30, 0xfffffff, "�v���C���[���{�[����Hit");
			return true;
		}
	}

	//�������ĂȂ�
	_dbgDrawFormatString(800, 40, 0xffffff, "�͂���");
	return false;
}

bool Raycast::CheckLine(Line playerLine, Line stageLine, Vector2& offset)
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

		if (stageray_[2] == stageLine)
		{
			offset.x = -abs(stageLine.p.x - playerLine.end.x);//��
			_dbgDrawFormatString(450, 0, 0xffffff, "������", true);
			return true;
		}
		if (stageray_[3] == stageLine)
		{
			offset.x = abs(stageLine.end.x - playerLine.end.x);//�E
			_dbgDrawFormatString(450, 0, 0xffffff, "�E����", true);
			return true;
		}
		if (stageray_[0] == stageLine)
		{
			offset.y = -abs(stageLine.p.y - playerLine.p.y);//��
			_dbgDrawFormatString(450, 0, 0xffffff, "�㔻��", true);
			return true;
		}
		if (stageray_[1] == stageLine)
		{
			offset.y = abs(stageLine.end.y - playerLine.end.y);//��
			_dbgDrawFormatString(450, 0, 0xffffff, "������", true);
			return true;
		}

		return true;
	}
}

void Raycast::setStageRay(Collision stagepos)
{
	stageray_ =
	{
		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//��
		{{stagepos.first.x,stagepos.second.y},stagepos.second},			//��
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//��
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//�E
	};
}

void Raycast::setBallRay(Vector2 pos, Vector2 size)
{
	ballray_ =
	{
		{{pos.x,pos.y},{pos.x + size.x,pos.y} },						//��
		{{pos.x,pos.y + size.y} ,{pos.x + size.x,pos.y + size.y}},		//��
		{{pos.x,pos.y},{pos.x,pos.y + size.y}},							//��
		{{pos.x + size.x,pos.y},{pos.x + size.x,pos.y + size.y}},		//�E
	};
}

void Raycast::setPlayerRay(Line ray)
{
	playerray_ = ray;

	//���C�̃f�o�b�N�\��
	_dbgDrawLine(ray.p.x, ray.p.y,ray.end.x, ray.end.y, 0xff0000);

}

