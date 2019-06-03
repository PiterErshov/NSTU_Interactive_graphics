#pragma once
#include <windows.h>
#include <vector>
#include <gl\gl.h>
#include "norm.h"


class Camera
{
public:
	Vector3f Pos;	//������� ������/��������� ����� �����������/
	Vector3f View;		//����������� ����������
	Vector3f rot_vect;	//������ �������� �����

	Camera();	//�����������
	void pos_camera(float posX, float posY, float posZ,
		float viewX, float viewY, float viewZ,
		float upX, float upY, float upZ);	//��������� ������� ������

	void set_view_mouse(GLint width, GLint height);			//��������� ���� � ������� ����
	void move_c(float speed);							//������������ ������ ������/�����
	void r_view(float angle, float x, float y, float z);	//�������� ������ ������ �������� ���
	void r_obs(Vector3f vCenter, float angle, float x, float y, float z);//�������� ������ ������ �����������
};