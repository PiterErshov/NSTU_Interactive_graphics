//СЦЕНА

#include <math.h>
#include <windows.h>
#include "glut.h"
#include <vector>
#include <gl\gl.h>
#include <stdio.h>
#include "helpTracer.h"
#include "gauss.h"

using namespace std;

//=============Для преобразования луча к базовому виду==============
//И все остальные вспомогательные подпрограммы

//определить матрицу трансформирования
void makeTransformMatrix(double **TMatrix, Vector3f shift, float scale);

//перевод луча к базовому виду
Ray transformRay(Ray _ray, Vector3f shift, GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);

//скалярное произведение
double scal(Vector3f vec1, Vector3f vec2);

//определить позицию луча(точку соударения луча с объектом)
Vector3f rayPos(Ray ray, double t);
//==================================================================

 //int lighting_num = 1;

class material_properties
{
public:
	GLfloat Ambient[4];	//фоновое отражение
	GLfloat Diffuse[4];	//рассеянное отражение
	GLfloat Specular[4];	//зеркальное отражение
	GLfloat Shininess;		//коэффициент блеска
};

//цвет
class Color	
{
public:
	GLfloat red;
	GLfloat green;
	GLfloat blue;

	Color();
	Color(GLfloat _red, GLfloat _green, GLfloat _blue);
	void add(GLfloat _red, GLfloat _green, GLfloat _blue);
	void add(Color colr);
	void add(Color colr, Color refl);
};

//источник света
class Light
{
public:
	Vector3f position;	//позиция источника света
	Color color;		//цвет источника света

	Light();	//конструктор
	void setPosition(Vector3f pos);	//установить позицию источника света
	void setColor(Color col);		//установить цвет источника света
};

//объект сцены: сфера
class Sphere
{
public:
	GLfloat radius;			//радиус сферы
	Vector3f center_coord;	//координаты центра сферы
	Color color;			//цвет сферы
	Color FrameColor;		//цвет каркаса сферы
	bool display;			//true - если сфера отрисовывается

	//свойства материала сферы
	material_properties Properties;

	Sphere();				//конструктор по умолчанию
	Sphere(GLfloat _radius, Vector3f _center, Color _color);	//конструктор
	void Draw();			//отрисовка сферы
	void DrawFrame();		//отрисовка каркаса сферы

	bool hit(Ray ray, Intersection &inter);	//определение соударений луча со сферой
	bool hit(Ray ray);						//упрощенный метод hit 
											//(не строит запись о пересечениях, используется для определения тени)
};

//объект сцены: тетраэдр
class Tetrahedron	
{
public:
	Vector3f coord[4];		//координаты вершин
	Vector3f center_coord;	//координаты центра тетраэдра
	Color color;			//цвет тетраэдра
	Color FrameColor;		//цвет каркаса тетраэдра
	bool display;			//true - если тетраэдр отрисовывается
	material_properties Properties; //свойства материала тетраэдра

	Tetrahedron();			//конструктор по умолчанию
	Tetrahedron(Vector3f _coord[], Vector3f _center, Color _color);	//конструктор
	void Draw();			//отрисовка тетраэдра
	void DrawFrame();		//отрисовка каркаса тетраэдра

	bool hit(Ray ray, Intersection &inter);	//определение соударений луча с тетраэдром
	bool hit(Ray ray);						//упрощенный метод hit 
											//(не строит запись о пересечениях, используется для определения тени)

	bool hitPlane(Ray ray, int ver1, int ver2, int ver3, Vector3f &N, float &time);	//определение соударения с гранью тетраэдра
};

//объект сцены: квадрат
class Square
{
public:
	Vector3f center_coord;	//координаты центра плоскости
	GLfloat scaleX;			//длина по оси Х
	GLfloat scaleZ;			//длина по оси Z
	Color color;			//цвет квадрата

	//свойства материала квадрата
	material_properties Properties;

	Square();			//конструктор по умолчанию
	Square(Vector3f _center, GLfloat _scaleX, GLfloat _scaleZ, Color _color);	//конструктор
	void Draw();			//отрисовка квадрата
	bool hit(Ray ray, Intersection &inter);	//определение соударений луча с квадратом
	bool hit(Ray ray);						//упрощенный метод hit 
											//(не строит запись о пересечениях, используется для определения тени)
};

//сцена
class Scene
{
public:
	Square square;								//плоскость
	vector <Sphere> vector_Sphere;				//контейнер сфер
	vector <Tetrahedron> vector_Tetrahedron;	//контейнер тетраэдров
	vector <Light> vector_Light;				//контейнер источников освещения
	int lighting_num;

	Scene();				//конструктор
	void set_data();		//считать из файла координаты фигур и их параметры
	Light set_l();			// установить освещение
	void set_material();
	void switch_forward();	//переключение между фигурами вперед
	void switch_backward();	//переключение между фигурами назад
	bool ambi = true; //параметр учёта фонового света
	bool clsh = true; //параметр учёта цвета сферы
	bool cltr = true; //параметр учёта цвета сферы
	void Draw();				//отрисовка сцены
	bool isInShadow(Ray ray);	//находится ли объект в тени другого объекта

	bool add_del;			//режим включения/исключения объектов
	bool sphere_mod;		//переключение между объектами сферами
	bool tetrahedron_mod;	//переключение между объектами тетраэдрами
	int active_Sphere;		//активная сфера
	int active_Tetrahedron;	//активный тетраэдр
};
