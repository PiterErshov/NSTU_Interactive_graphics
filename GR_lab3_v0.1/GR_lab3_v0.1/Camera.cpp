#include "Camera.h"

Camera::Camera()
{

}
void Camera::pos_camera(float posX, float posY, float posZ,
	float viewX, float viewY, float viewZ,
	float upX, float upY, float upZ)
{
	//установить позицию камеры
	Vector3f _Position = Vector3f(posX, posY, posZ);
	Vector3f _View = Vector3f(viewX, viewY, viewZ);
	Vector3f _rot_vect = Vector3f(upX, upY, upZ);

	Pos = _Position;
	View = _View;
	rot_vect = _rot_vect;
}
void Camera::move_c(float speed)
{
	Vector3f _View = View - Pos;	//определить направление взгляда
	Pos.x += _View.x * speed;	//изменить положение 
	View.x += _View.x * speed;
	Pos.z += _View.z * speed;	//камеры
	View.z += _View.z * speed;
	Pos.y += _View.y * speed;
	View.y += _View.y * speed;
}

void Camera::r_view(float angle, float x, float y, float z)
{
	Vector3f _newView;
	Vector3f _View;

	//определить направление взгляда
	_View = View - Pos;

	//рассчитать синус и косинус переданного угла
	float cosA = (float)cos(angle);
	float sinA = (float)sin(angle);

	//пересчитаем координаты по каким-то диким формулам
	//новая координата X для вращаемой точки
	_newView.x = (cosA + (1 - cosA)*x*x)	* _View.x;
	_newView.x += ((1 - cosA)*x*y - z * sinA) * _View.y;
	_newView.x += ((1 - cosA)*x*z + y * sinA) * _View.z;

	//новая координата Y для вращаемой точки
	_newView.y = ((1 - cosA)*x*y + z * sinA) * _View.x;
	_newView.y += (cosA + (1 - cosA)*y*y)	* _View.y;
	_newView.y += ((1 - cosA)*y*z - x * sinA) * _View.z;

	//новая координата Z для вращаемой точки
	_newView.z = ((1 - cosA)*x*z - y * sinA) * _View.x;
	_newView.z += ((1 - cosA)*y*z + x * sinA) * _View.y;
	_newView.z += (cosA + (1 - cosA)*z*z)	* _View.z;

	//установить новый взгляд камеры
	View.x = Pos.x + _newView.x;
	View.y = Pos.y + _newView.y;
	View.z = Pos.z + _newView.z;
}
void Camera::set_view_mouse(GLint width, GLint height)
{
	POINT mousePos;		//позиция мыши

	//вычислить координаты центра окна
	int middleX = width / 2.0f;
	int middleY = height / 2.0f;

	float angleY = 0.0f;	//Направление взгляда вверх/вниз
	float angleZ = 0.0f;	//Значение, необходимое для вращения влево-вправо (по оси Y)
	static float currentRotX = 0.0f;

	//получить текущие координаты мыши
	GetCursorPos(&mousePos);

	//если положение мыши не изменилось
	//камеру вращать не нужно
	if (mousePos.x == middleX && mousePos.y == middleY) return;

	//вернуть координаты курсора в центр окна
	SetCursorPos(middleX, middleY);

	//определить, куда был сдвинут курсор
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;	//Сохраняем последний угол вращения 
							//и используем заново currentRotX

	//если поворот больше одного градуса, умешьшим его
	//чтобы уменьшить скорость вращения
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;
		if (lastRotX != 1.0f)
		{
			//Чтобы найти ось, вокруг которой нужно совершать вращение вверх и вниз, нужно 
			//найти вектор, перпендикулярный вектору взгляда камеры и 
			//вертикальному вектору
			Vector3f vAxis = Cross(View - Pos, rot_vect);

			//нормализуем ось
			vAxis = Normalize(vAxis);

			//вращаем камеру вокруг нашей оси на заданный угол
			r_view(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	//Если угол меньше -1.0f
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		if (lastRotX != -1.0f)
		{
			//вычисляем ось
			Vector3f vAxis = Cross(View - Pos, rot_vect);

			//нормализуем ось
			vAxis = Normalize(vAxis);

			//вращаем
			r_view(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	//если в пределах 1.0f -1.0f - просто вращаем
	else
	{
		Vector3f vAxis = Cross(View - Pos, rot_vect);
		vAxis = Normalize(vAxis);
		r_view(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	//всегда вращаем камеру вокруг Y-оси
	r_view(angleY, 0, 1, 0);
}
void Camera::r_obs(Vector3f _Center, float angle, float x, float y, float z)
{
	Vector3f _NewPosition;

	//получим центр, вокруг которого нужно вращатся
	Vector3f vPos = Pos - _Center;

	//вычислим синус и косинус угла
	float cosA = (float)cos(angle);
	float sinA = (float)sin(angle);

	//Найдем значение X точки вращения
	_NewPosition.x = (cosA + (1 - cosA)*x*x)	* vPos.x;
	_NewPosition.x += ((1 - cosA)*x*y - z * sinA)	* vPos.y;
	_NewPosition.x += ((1 - cosA)*x*z + y * sinA)	* vPos.z;

	//значение Y
	_NewPosition.y = ((1 - cosA)*x*y + z * sinA)	* vPos.x;
	_NewPosition.y += (cosA + (1 - cosA)*y*y)	* vPos.y;
	_NewPosition.y += ((1 - cosA)*y*z - x * sinA)	* vPos.z;

	//значение Z
	_NewPosition.z = ((1 - cosA)*x*z - y * sinA)	* vPos.x;
	_NewPosition.z += ((1 - cosA)*y*z + x * sinA)	* vPos.y;
	_NewPosition.z += (cosA + (1 - cosA)*z*z)	* vPos.z;

	//установить новую позицию камеры
	Pos = _Center + _NewPosition;
}