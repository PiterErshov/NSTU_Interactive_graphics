//ТРАССИРОВЩИК ЛУЧЕЙ

#include"scene.h"


const int MAX = 4;		//максимальный уровень рекурсии

struct sq
{
	int x1;
	int y1; 
	int x2;
	int y2;

	sq(int _1, int _2, int _3, int _4)
	{
		x1 = _1;
		y1 = _2;
		x2 = _3;
		y2 = _4;
	}
	sq(){};
};

struct pix
{
	Color color;
	sq p;

	pix(Color c, sq p_)
	{
		color = c;
		p = p_;
	}

	pix() {};
};

extern vector<pix> PIX;

//трассировщик лучей
class Raytracer
{
public:

	Vector3f eye;	//точка выхода лучей/положение камеры/
	Scene scene;	//сцена
	Camera camera;	//камера
	float aspect;
	int blockSize;	//размер блока пикселей
	int nCols;
	int nRows;

	Raytracer();	//конструктор по умолчанию
	Raytracer(Vector3f _eye, Scene _scene, Camera _camera, float _aspect, int _nCols, int _nRows, int _blockSize);	//конструктор
	void draw();
	void Raytrace();		//трассировка
	Color Shade(Ray ray);	//ядро трассировки
	Intersection getFirstHit(Ray ray);	//находит объект, с которым луч соударяется первым
};