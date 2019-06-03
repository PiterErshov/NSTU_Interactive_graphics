#pragma once
#include <windows.h>
#include <vector>
#include <gl\gl.h>
#include "norm.h"


class Camera
{
public:
	Vector3f Pos;	//позиция камеры/положение точки наблюдателя/
	Vector3f View;		//направление наблюдения
	Vector3f rot_vect;	//вектор поворота сцены

	Camera();	//конструктор
	void pos_camera(float posX, float posY, float posZ,
		float viewX, float viewY, float viewZ,
		float upX, float upY, float upZ);	//установка позиции камеры

	void set_view_mouse(GLint width, GLint height);			//установка вида с помощью мыши
	void move_c(float speed);							//передвижение камеры вперед/назад
	void r_view(float angle, float x, float y, float z);	//вращение камеры вокруг заданной оси
	void r_obs(Vector3f vCenter, float angle, float x, float y, float z);//вращение камеры вокруг наблюдателя
};