#include "camera.h"

//луч
class Ray
{
public:
	Vector3f start;		//стартовая точка луча
	Vector3f dir;		//направление луча
	int recurseLevel;	//текущий уровень рекурсии

	Ray();							//конструктор
	void SetStart(Vector3f _start);	//установить стартовую точку
	void SetDir(Vector3f _dir);		//установить направление
};

//информация о соударении
class HitInfo
{
public:
	double hitTime;	//время соударения
	int objectType;	//тип объекта соударения
	int objectNum;	//номер объекта соударения
	int surface;	//поверхность соударения
	bool isEntering;	//луч входит или выходит из объекта?

	Vector3f hitPoint;	//точка соударения
	Vector3f hitNormal;	//нормаль в точке соударения

	HitInfo();
	void set(HitInfo hI);
};

//список соударений
class Intersection
{
public:
#define maxNumHits 8
	int numHits;	//число соударений для положительных значений времени
	HitInfo hit[maxNumHits];	//список соударений

	Intersection();
	void set(Intersection intr);
};