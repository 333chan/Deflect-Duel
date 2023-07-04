#include "Raycast.h"
#include "../object/player/Player.h"
#include "../object/ball/Ball.h"
#include"../../_debug/_DebugDispOut.h"


bool Raycast::StageToPlayerCheckColl(Collision stagepos, Vector2& offset)
{
	//�X�e�[�W����p���C�̍쐬
	setStageRay(stagepos);

	//�X�e�[�W���C���`�F�b�N
	for (const auto& rs : stageRay_)
	{
		//�f�o�b�N�p���胉�C��
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y, 0xffffff);

		//�X�e�[�W�ƃv���C���[
		if (StageToPlayerChackLine(playerRay_, rs, offset))
		{
			//Hit������
			_dbgDrawFormatString(800, 0, 0xfffffff, "�v���C���[���X�e�[�W��Hit");
			return true;
		}
	}
	//�������ĂȂ�
	_dbgDrawFormatString(800, 100, 0xffffff, "�v���C���[���X�e�[�W�ɓ������ĂȂ�");
	return false;
}

bool Raycast::PlayerToBallChackColl(Vector2& offset)
{

	for (const auto& rps : playerSquareRay_)
	{
		//�f�o�b�N�p���胉�C��


		for (const auto& rb : ballRay_)
		{
			//�v���C���[�ƃ{�[��
			if (BallToPlayerChackLine(rps, rb, offset))
			{
				//Hit������
				_dbgDrawFormatString(1000, 30, 0xfffffff, "�v���C���[���{�[����Hit");
				return true;
			}
		}
		_dbgDrawLine(rps.p.x, rps.p.y, rps.end.x, rps.end.y, 0xff0000);
	}
	//�������ĂȂ�
	_dbgDrawFormatString(800, 80, 0xffffff, "�v���C���[���{�[���ɓ����炸");
	return false;
}

bool Raycast::StageToBallCheckColl(Collision stagepos, Vector2& offset, Vector2& refDir)
{
	//�X�e�[�W����p���C�̍쐬
	setStageRay(stagepos);

	for (const auto& rs : stageRay_)
	{
		//�f�o�b�N�p���胉�C��
		_dbgDrawLine(rs.p.x, rs.p.y, rs.end.x, rs.end.y, 0xffffff);

		for (const auto& rb : ballRay_)
		{
			
			//�f�o�b�N�p���胉�C��
			_dbgDrawLine(rb.p.x, rb.p.y, rb.end.x, rb.end.y, 0xff0000);
			//�{�[���ƃX�e�[�W
			if (BallToStageChackLine(rb, rs, offset, refDir))
			{
				//Hit������
				_dbgDrawFormatString(800, 20, 0xfffffff, "�{�[�����X�e�[�W��Hit");

				return true;
			}
		}
	}
	_dbgDrawFormatString(0, 350, 0xfffffff, "%f,%f,%f,%f", ballRay_[0].p.x, ballRay_[0].p.y);
	//�������ĂȂ�
	_dbgDrawFormatString(800, 40, 0xffffff, "�{�[�����X�e�[�W�ɓ����炸");
	return false;
}

bool Raycast::AttackToBallCheckColl(Vector2& refDir)
{

	for (const auto& rpa : playerAttackRay_)
	{
		//�f�o�b�N�p���胉�C��
		_dbgDrawLine(rpa.p.x, rpa.p.y, rpa.end.x, rpa.end.y, 0xff0000);

		for (const auto& rb : ballRay_)
		{
			//�U���ƃ{�[��
			if (AttackToBallChackLine(rpa, rb, refDir))
			{
				//Hit������
				_dbgDrawFormatString(1000, 100, 0xfffffff, "�U�����{�[����Hit");
				return true;
			}
		}

	}
	//�������ĂȂ�
	_dbgDrawFormatString(800, 100, 0xffffff, "�U�����{�[���ɂ�����Ȃ�");
	return false;
}

void Raycast::ChackLine(Line mainLine, Line subLine)
{
	//�R�O���[�v�̃x�N�g�����쐬
	a_to_b = mainLine.vec();
	a_to_c = subLine.p - mainLine.p;
	a_to_d = subLine.end - mainLine.p;

	c_to_d = subLine.vec();
	c_to_a = mainLine.p - subLine.p;
	c_to_b = mainLine.end - subLine.p;


	//�O�ςŌv�Za_to�n
	cross_01 = (a_to_b.x * a_to_c.y) - (a_to_c.x * a_to_b.y);
	cross_02 = (a_to_b.x * a_to_d.y) - (a_to_d.x * a_to_b.y);

	//�O�ςŌv�Zc_to�n
	cross_03 = (c_to_d.x * c_to_a.y) - (c_to_a.x * c_to_d.y);
	cross_04 = (c_to_d.x * c_to_b.y) - (c_to_b.x * c_to_d.y);
}

bool Raycast::StageToPlayerChackLine(Line playerLine, Line stageLine, Vector2& offset)
{
	ChackLine(playerLine,stageLine);

	//�������Ă��Ȃ�
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}

	//�������Ă���
	offset = { 0.0f,0.0f };

	//�X�e�[�W�ƃv���C���[
	if (stageRay_[2] == stageLine)
	{
		offset.x = -abs(stageLine.p.x - playerLine.end.x);//��
		_dbgDrawFormatString(450, 0, 0xffffff, "������", true);
		return true;
	}
	if (stageRay_[3] == stageLine)
	{
		offset.x = abs(stageLine.end.x - playerLine.end.x);//�E
		_dbgDrawFormatString(450, 0, 0xffffff, "�E����", true);
		return true;
	}
	if (stageRay_[0] == stageLine)
	{
		offset.y = -abs(stageLine.p.y - playerLine.p.y);//��
		_dbgDrawFormatString(450, 0, 0xffffff, "�㔻��", true);
		return true;
	}
	if (stageRay_[1] == stageLine)
	{
		offset.y = abs(stageLine.end.y - playerLine.end.y);//��
		_dbgDrawFormatString(450, 0, 0xffffff, "������", true);
		return true;
	}

	return true;
}

bool Raycast::BallToPlayerChackLine(Line playerLine, Line ballLine, Vector2& bound)
{
	ChackLine(playerLine, ballLine);

	//�������Ă��Ȃ�
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}

	//�������Ă���
	bound = { 0.0f,0.0f };

	//�{�[���ƃv���C���[
	if (ballRay_[3] == ballLine)
	{
		//bound.x = -abs(playerLine.p.x - ballLine.end.x);//��
		_dbgDrawFormatString(600, 0, 0xffffff, "�{�[����", true);
		return true;
	}
	if (ballRay_[2] == ballLine)
	{
		//bound.x = abs(playerLine.end.x - ballLine.p.x);//�E
		_dbgDrawFormatString(600, 0, 0xffffff, "�{�[���E", true);
		return true;
	}
	if (ballRay_[1] == ballLine)
	{
		//bound.y = -abs(playerLine.p.y - ballLine.p.y);//��
		_dbgDrawFormatString(600, 0, 0xffffff, "�{�[����", true);
		return true;
	}
	if (ballRay_[0] == ballLine)
	{
		//bound.y = abs(playerLine.end.y - ballLine.end.y);//��
		_dbgDrawFormatString(600, 0, 0xffffff, "�{�[����", true);
		return true;
	}

	//return true;
}

bool Raycast::BallToStageChackLine(Line ballLine, Line stageLine, Vector2& offset, Vector2& refDir)
{
	ChackLine(ballLine, stageLine);

	//�������Ă��Ȃ�
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}

	//�������Ă���
	offset = { 0.0f,0.0f };
	//refDir = { 0.0f,0.0f };

	//�X�e�[�W�ƃ{�[��
	if (stageRay_[3] >= stageLine&& stageRay_[2] >= stageLine)
	{
		offset.x = -abs(stageLine.p.x - ballLine.p.x);//��
		//offset.x = 20;//��
		refDir.x = 1;
		_dbgDrawFormatString(600, 0, 0xffffff, "�{�[��������", true);
		//return true;
	}
	if (stageRay_[2] <= stageLine && stageRay_[3] <= stageLine)
	{
		offset.x =abs(stageLine.end.x - ballLine.end.x);//�E
		//offset.x =-20;//�E
		refDir.x = -1;
		_dbgDrawFormatString(600, 0, 0xffffff, "�{�[���E����", true);
		//return true;
	}
	if (stageRay_[0] == stageLine)
	{
		offset.y = -abs(stageLine.p.y - ballLine.p.y);//��
		//offset.y = 20;//��
		refDir.y = 1;
		_dbgDrawFormatString(600, 0, 0xffffff, "�{�[���㔻��", true);
		//return true;
	}
	if (stageRay_[1] == stageLine)
	{
		offset.y = abs(stageLine.end.y - ballLine.end.y);//��
		//offset.y = -20;//��
		refDir.y=-1;
		_dbgDrawFormatString(600, 0, 0xffffff, "�{�[��������", true);
		//return true;
	}
	return true;
}

bool Raycast::AttackToBallChackLine(Line playerLine, Line ballLine, Vector2& refDir)
{
	ChackLine(playerLine, ballLine);

	//�������Ă��Ȃ�
	if (cross_01 * cross_02 > 0.0f)
	{
		return false;
	}
	if (cross_03 * cross_04 > 0.0f)
	{
		return false;
	}

	//�������Ă���
	refDir = { 0.0f,0.0f };

	//�U���ƃ{�[��
	if (playerAttackRay_[3] == playerLine)
	{
		refDir.x = 1;//��
		_dbgDrawFormatString(600, 0, 0xffffff, "���ɕԂ�", true);
		return true;
	}
	if (playerAttackRay_[2] == playerLine)
	{
		refDir.x = -1;//�E
		_dbgDrawFormatString(600, 0, 0xffffff, "�E�ɕԂ�", true);
		return true;
	}
	if (playerAttackRay_[0] == playerLine)
	{
		refDir.y = -1;//��
		_dbgDrawFormatString(600, 0, 0xffffff, "��ɕԂ�", true);
		return true;
	}
	if (playerAttackRay_[1] == playerLine)
	{
		refDir.y = 1;//��
		_dbgDrawFormatString(600, 0, 0xffffff, "���ɕԂ�", true);
		return true;
	}

	return true;

}

void Raycast::setStageRay(Collision stagepos)
{
	stageRay_ =
	{
		{stagepos.first,{stagepos.second.x,stagepos.first.y} },			//��
		{{stagepos.first.x,stagepos.second.y},stagepos.second},			//��
		{stagepos.first,{stagepos.first.x,stagepos.second.y} },			//��
		{{stagepos.second.x ,stagepos.first.y},stagepos.second},		//�E
	};
}



void Raycast::setBallRay(Vector2 pos, Vector2 size)
{
	ballRay_ =
	{
		{{pos.x,pos.y},{pos.x+ size.x,pos.y} },						//��
		{{pos.x,pos.y+ size.y} ,{pos.x + size.x,pos.y + size.y}},	//��
		{{pos.x,pos.y},{pos.x,pos.y + size.y}},						//��
		{{pos.x+ size.x,pos.y},{pos.x + size.x,pos.y + size.y}},	//�E
	};
}

void Raycast::setPlayerRay(Line ray)
{
	playerRay_ = ray;

	//���C�̃f�o�b�N�\��
	//_dbgDrawLine(ray.p.x, ray.p.y,ray.end.x, ray.end.y, 0xff0000);

}

void Raycast::setPlayerSquareRay(Vector2 pos, Vector2 size,Vector2 movePos)
{
	//playerSquareRay_ = 
	//{
	//	{{pos.x + movePos.x,pos.y + movePos.y },{pos.x  + size.x + movePos.x,pos.y + movePos.y } },						//��
	//	{{pos.x + movePos.x,pos.y + size.y + movePos.y} ,{pos.x + size.x + movePos.x,pos.y + size.y + movePos.y}},		//��
	//	{{pos.x + movePos.x,pos.y + movePos.y},{pos.x + movePos.x,pos.y + size.y + movePos.y }},							//��
	//	{{pos.x + size.x + movePos.x,pos.y + movePos.y},{pos.x + size.x + movePos.x,pos.y + size.y + movePos.y }},		//�E
	//};

	playerSquareRay_ =
	{
		{{pos.x ,pos.y },{pos.x + size.x ,pos.y} },						//��
		{{pos.x ,pos.y + size.y} ,{pos.x + size.x,pos.y + size.y}},		//��
		{{pos.x ,pos.y},{pos.x,pos.y + size.y}},						//��
		{{pos.x + size.x,pos.y},{pos.x + size.x,pos.y + size.y}},		//�E
	};

}

void Raycast::setPlayerAttackRay(Vector2 pos, Vector2 size)
{
	playerAttackRay_ =
	{
		{{pos.x ,pos.y },{pos.x + size.x,pos.y } },							//��
		{{pos.x ,pos.y + size.y } ,{pos.x + size.x ,pos.y + size.y}},		//��
		{{pos.x ,pos.y },{pos.x ,pos.y + size.y }},							//��
		{{pos.x + size.x ,pos.y },{pos.x + size.x,pos.y + size.y }},		//�E
	};
}


