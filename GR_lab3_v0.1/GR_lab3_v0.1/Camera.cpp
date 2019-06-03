#include "Camera.h"

Camera::Camera()
{

}
void Camera::pos_camera(float posX, float posY, float posZ,
	float viewX, float viewY, float viewZ,
	float upX, float upY, float upZ)
{
	//���������� ������� ������
	Vector3f _Position = Vector3f(posX, posY, posZ);
	Vector3f _View = Vector3f(viewX, viewY, viewZ);
	Vector3f _rot_vect = Vector3f(upX, upY, upZ);

	Pos = _Position;
	View = _View;
	rot_vect = _rot_vect;
}
void Camera::move_c(float speed)
{
	Vector3f _View = View - Pos;	//���������� ����������� �������
	Pos.x += _View.x * speed;	//�������� ��������� 
	View.x += _View.x * speed;
	Pos.z += _View.z * speed;	//������
	View.z += _View.z * speed;
	Pos.y += _View.y * speed;
	View.y += _View.y * speed;
}

void Camera::r_view(float angle, float x, float y, float z)
{
	Vector3f _newView;
	Vector3f _View;

	//���������� ����������� �������
	_View = View - Pos;

	//���������� ����� � ������� ����������� ����
	float cosA = (float)cos(angle);
	float sinA = (float)sin(angle);

	//����������� ���������� �� �����-�� ����� ��������
	//����� ���������� X ��� ��������� �����
	_newView.x = (cosA + (1 - cosA)*x*x)	* _View.x;
	_newView.x += ((1 - cosA)*x*y - z * sinA) * _View.y;
	_newView.x += ((1 - cosA)*x*z + y * sinA) * _View.z;

	//����� ���������� Y ��� ��������� �����
	_newView.y = ((1 - cosA)*x*y + z * sinA) * _View.x;
	_newView.y += (cosA + (1 - cosA)*y*y)	* _View.y;
	_newView.y += ((1 - cosA)*y*z - x * sinA) * _View.z;

	//����� ���������� Z ��� ��������� �����
	_newView.z = ((1 - cosA)*x*z - y * sinA) * _View.x;
	_newView.z += ((1 - cosA)*y*z + x * sinA) * _View.y;
	_newView.z += (cosA + (1 - cosA)*z*z)	* _View.z;

	//���������� ����� ������ ������
	View.x = Pos.x + _newView.x;
	View.y = Pos.y + _newView.y;
	View.z = Pos.z + _newView.z;
}
void Camera::set_view_mouse(GLint width, GLint height)
{
	POINT mousePos;		//������� ����

	//��������� ���������� ������ ����
	int middleX = width / 2.0f;
	int middleY = height / 2.0f;

	float angleY = 0.0f;	//����������� ������� �����/����
	float angleZ = 0.0f;	//��������, ����������� ��� �������� �����-������ (�� ��� Y)
	static float currentRotX = 0.0f;

	//�������� ������� ���������� ����
	GetCursorPos(&mousePos);

	//���� ��������� ���� �� ����������
	//������ ������� �� �����
	if (mousePos.x == middleX && mousePos.y == middleY) return;

	//������� ���������� ������� � ����� ����
	SetCursorPos(middleX, middleY);

	//����������, ���� ��� ������� ������
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;	//��������� ��������� ���� �������� 
							//� ���������� ������ currentRotX

	//���� ������� ������ ������ �������, �������� ���
	//����� ��������� �������� ��������
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;
		if (lastRotX != 1.0f)
		{
			//����� ����� ���, ������ ������� ����� ��������� �������� ����� � ����, ����� 
			//����� ������, ���������������� ������� ������� ������ � 
			//������������� �������
			Vector3f vAxis = Cross(View - Pos, rot_vect);

			//����������� ���
			vAxis = Normalize(vAxis);

			//������� ������ ������ ����� ��� �� �������� ����
			r_view(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	//���� ���� ������ -1.0f
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		if (lastRotX != -1.0f)
		{
			//��������� ���
			Vector3f vAxis = Cross(View - Pos, rot_vect);

			//����������� ���
			vAxis = Normalize(vAxis);

			//�������
			r_view(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	//���� � �������� 1.0f -1.0f - ������ �������
	else
	{
		Vector3f vAxis = Cross(View - Pos, rot_vect);
		vAxis = Normalize(vAxis);
		r_view(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	//������ ������� ������ ������ Y-���
	r_view(angleY, 0, 1, 0);
}
void Camera::r_obs(Vector3f _Center, float angle, float x, float y, float z)
{
	Vector3f _NewPosition;

	//������� �����, ������ �������� ����� ��������
	Vector3f vPos = Pos - _Center;

	//�������� ����� � ������� ����
	float cosA = (float)cos(angle);
	float sinA = (float)sin(angle);

	//������ �������� X ����� ��������
	_NewPosition.x = (cosA + (1 - cosA)*x*x)	* vPos.x;
	_NewPosition.x += ((1 - cosA)*x*y - z * sinA)	* vPos.y;
	_NewPosition.x += ((1 - cosA)*x*z + y * sinA)	* vPos.z;

	//�������� Y
	_NewPosition.y = ((1 - cosA)*x*y + z * sinA)	* vPos.x;
	_NewPosition.y += (cosA + (1 - cosA)*y*y)	* vPos.y;
	_NewPosition.y += ((1 - cosA)*y*z - x * sinA)	* vPos.z;

	//�������� Z
	_NewPosition.z = ((1 - cosA)*x*z - y * sinA)	* vPos.x;
	_NewPosition.z += ((1 - cosA)*y*z + x * sinA)	* vPos.y;
	_NewPosition.z += (cosA + (1 - cosA)*z*z)	* vPos.z;

	//���������� ����� ������� ������
	Pos = _Center + _NewPosition;
}