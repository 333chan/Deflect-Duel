#pragma once
#include<DxLib.h>

//�T�C�Y��\���\����
struct Size {
	float w;//��
	float h;//����
};

//���W��\���\����
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;
	
	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
	Vector2 Normalized();

	void operator+=(const Vector2& v);
	void operator+=(const float v);

	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator*(float scale);
	Vector2 operator*(Vector2& vec);

	Vector2 operator/(float scale);
	Vector2 operator-() {
		return Vector2(-x, -y);
	}

	//��r���Z�q
	bool operator ==(const Vector2& vec)const;
	bool operator !=(const Vector2& vec)const;
	bool operator <=(const Vector2& vec)const;
	bool operator <(const Vector2& vec)const;
	bool operator >=(const Vector2& vec)const;
	bool operator >(const Vector2& vec)const;

};

struct Vector2Int {
	Vector2Int():x(0),y(0){}
	Vector2Int(int inx,int iny):x(inx),y(iny){}
	int x, y;

	void operator+=(const Vector2Int& v);
	void operator-=(const Vector2Int& v);
	void operator*=(int scale);
	Vector2Int operator+(const Vector2Int& v);
	Vector2Int operator-(const Vector2Int& v);
	Vector2Int operator*(int scale);
	Vector2Int operator-() {return Vector2Int(-x, -y);}
	Vector2Int operator+() {return Vector2Int(+x, +y);}

	//��r���Z�q
	bool operator ==(const Vector2Int& vec)const;
	bool operator !=(const Vector2Int& vec)const;
	bool operator <=(const Vector2Int& vec)const;
	bool operator <(const Vector2Int& vec)const;
	bool operator >=(const Vector2Int& vec)const;
	bool operator >(const Vector2Int& vec)const;
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);

struct Vector3 {
	Vector3() :x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float inx, float iny,float inz) :x(inx), y(iny) ,z(inz) {}
	float x, y, z;

	///�x�N�g���̑傫����Ԃ��܂�
	float Magnitude()const;
	
	///���K��(�傫�����P��)���܂�
	void Normalize();

	///���K���x�N�g����Ԃ��܂�
	Vector3 Normalized();
	Vector3 toDiopter();

	//VECTOR�ɕϊ�
	VECTOR toVECTOR();

	void operator+=(const Vector3& v);
	void operator-=(const Vector3& v);
	void operator*=(float scale);
	Vector3 operator*(float scale);
	Vector3 operator*(Vector3& vec);
	Vector3 operator=(const Vector3& v);

	Vector3 operator/(float scale);
	Vector3 operator-() {
		return Vector3(-x, -y,-z);
	}

	//��r���Z�q
	bool operator ==(const Vector3& vec)const;
	bool operator !=(const Vector3& vec)const;
	bool operator <=(const Vector3& vec)const;
	bool operator <(const Vector3& vec)const;
	bool operator >=(const Vector3& vec)const;
	bool operator >(const Vector3& vec)const;

};

Vector3 operator+(const Vector3& va, const Vector3 vb);
Vector3 operator-(const Vector3& va, const Vector3 vb);

struct Line
{
	Vector3 p;		//�n�_
	Vector3 end;	//�I�_
	Vector3 vec();
};

//���ς�Ԃ�
float Dot(const Vector2& va, const Vector2& vb);
float Dot(const Vector3& va, const Vector3& vb);

//�O�ς�Ԃ�
float Cross(const Vector2& va, const Vector2& vb);
Vector3 Cross(const Vector3& va, const Vector3& vb);

// ���W�A��(rad)����x(deg)�ɕϊ�
float Rad2DegF(float rad);

// �x(deg)���烉�W�A��(rad)�ɕϊ�
float Deg2RadF(float deg);

// 0(0)�`2��(360�x)�͈̔͂Ɏ��߂�
double RadIn2PI(double rad);

//�T�C���J�[�u
float SinCurve(float period, float count,float power);

// ���`���
Vector3 Lerp(const Vector3& start,const Vector3& end, float t);
Vector2Int Lerp(const Vector2Int& start,const Vector2Int& end, double t);

//Vector3�ɕϊ�
Vector3 ConvertVECTORtoVector3(const VECTOR& v3);

///���ω��Z�q
float operator*(const Vector2& va, const Vector2& vb);

///�O�ω��Z�q
float operator%(const Vector2& va, const Vector2& vb);
