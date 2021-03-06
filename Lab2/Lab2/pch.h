#ifndef PCH_H
#define PCH_H
#define GLUT_DISABLE_ATEXIT_HACK 
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include "glut.h" 
//#include "glaux.h"
#include <vector>
//#include <GL.H>
#include "glut.h"
#include "GL\GL.h"
#include <gl\GLU.h>
//#include <GL\glut.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <algorithm>	//да

#pragma comment(lib,"glaux.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment(lib,"glut32.lib")

// константы для подменю цвет линии
#define RED		1
#define BLUE	2
#define GREEN	3
#define ORANGE	4
#define YELLOW	5
#define VIOLET	6
#define BLACK	7
#define WHITE	8

// константы для подменю масштаб
#define ZOOM_IN		1
#define ZOOM_OUT	2

// константы для подподменю типа текстуры
#define NONTEXTURE 0
#define	TEXTURE1 1
#define TEXTURE2 2
#define TEXTURE3 3
#define TEXTURE4 4
#define TEXTURE5 5
#define TEXTURE6 6
#define TEXTURE7 7	//больше текстурок
#define TEXTURE8 8	//потому что все остальное не очень

// константы для подподменю типа наложения текстуры
#define	OVERLAY1 1
#define OVERLAY2 2

using namespace std;

GLint width, height;	//ширина и высота окна
int current_figure;		//текущая фигура
int max_figure;			//общее число фигур
int N = 8;
bool drawing_mode;		//true - режим отрисовки

bool dot_mode;			//режим редактирования точек
int current_dot;		//текущая точка

int main_menu1;			//идентификатор меню работы с шестиугольниками
int main_menu2;			//идентификатор меню работы с точками

unsigned int names_tex[8];	//имена структур

int colour_shuffle_mode;	//режим смешения цветов: 0 - нет, 1 - and, 2 - not and

int netsize;	//шаг сетки

vector <pair<int, int>> net_line;

class point
{
public:
	GLint x, y;
	point(GLint p1, GLint p2)
	{
		x = p1;
		y = p2;
	}
};

class pointf	//вещественная точка/использовалась для расчета текстурных координат/
{
public:
	GLfloat x, y;
	pointf(GLfloat p1, GLfloat p2)
	{
		x = p1;
		y = p2;
	}
};

class colour
{
public:
	GLint red, blue, green;
	colour(GLint r, GLint b, GLint g)
	{
		red = r;
		blue = b;
		green = g;
	}
};

struct active_edge
{
	double k;
	int x, y, xk, yk;
	int dx, dy;
	int yfin;
}a;


class polygon
{
public:
	vector <point> coord_polygon;	//координаты восьмиугольника
	vector <colour> color_point;	//цвет координаты восьмиугольника
	int Ox;							//сдвиг по оси Ох от начала координат
	int Oy;							//сдвиг по оси Оу от начала координат
	GLint center_x;
	GLint center_y;
	//координаты первой вершины восьмиугольника
	GLint vertex_x;
	GLint vertex_y;
	double ScaleOx;					//коэффициент масштабирования по Ох
	double ScaleOy;					//коэффициент масштабирования по Оу
	int Angle;						//угол поворота
	int number;						//номер восьмиугольника
	int texture_number;				//номер текстуры -1 если нет текстуры
	int overlay_type_tex;			//тип наложения текстуры
	bool cent_stat;//false - центр еще не установлен, или координаты фигуры определены окончательно
	bool cord_stat;//true - можно рассчитывать координаты восьмиугольника
};

extern bool group(const pair<int, int> &a, const pair <int, int> &b);

vector <polygon> v;				//вектор восьмиугольников

polygon cen_ver;		//содержит координаты центра и первой вершины отрисовываемого восьмиугольника

vector <point> drawing_v;		//вектор для расчета координат отрисовываемого восьмиугольника

vector <pointf> texture_coord;	//текстурные координаты/считаются один раз и на века функцией calc_texture_coord/

polygon drawing_polygon;		//отрисовываемый восьмиугольник/нужен только для установки стандартных параметров отрисованного восьмиугольника, потом сразу скидывается в v и вычищается/

colour drawing_color = colour(150, 150, 150);	//цвет по умолчанию

int raster_mode;	//режим растеризации, 0 - нет, 1 - контур, 2 - заполнение

vector <int> raster_size;	//массив, своими значениями разбивающий массив сеткелей на принадлежность к фигурам

extern int rasterize(const polygon & n, vector <pair<int, int>> * v);	//алгоритм заполнения многоугольника

extern bool group(const pair<int, int> &a, const pair <int, int> &b);

void Change_Mode();

void On_Up_Key();

void On_Down_Key();

void MoveUp(); 

void MoveDown();

void MoveLeft();

void MoveRight();

void Increase(double zoom);

void Reduce(double zoom);

void Del_Group();//удалить фигуру

void Clear();//удалить все фигуры

void RotateLeft();

void RotateRight();

bool equality(point one, point two);

void Finish_Primitive();;

void calc_polygon_coord(point center, point vertex);//вычисление координат шестиугольника

void set_texture_coord();//расчет текстурных координат

void processColorMenu(int option);//обработчик подменю цветов mainmenu1


void processColorMenu2(int option);//обработчик подменю цветов mainmenu2


void processScaleMenu(int option);

void processTextureMenuMain(int option);

void processTextureMenu(int option);


void processOverlayMenu(int option);

void processLogicMenu(int option);

void processRasterMenu(int option);

void processRasterModeMenu(int option);

void processNetMenu(int option);

void processMainMenu(int option);

void createMenu();

void loadTexture();

void SwitchColourShuffleMode();

void SwitchRasterMode();

void SwitchNetSize();

bool point_in_net(int x0, int y0, int * xi, int * yj);	//координаты искомой точки, координаты сеткеля

void quater_net(int x, int y, int i, int j, int * xn, int * yn);

int contour_Bresenham(int x_0, int y_0, int x_k, int y_k, vector <pair<int, int>> * v, int cx, int cy);

bool pred(const pair<int, int> &a, const pair <int, int> &b);

//сортировка по возрастанию внутри группы
bool group(const pair<int, int> &a, const pair <int, int> &b);

int fill_raster(const polygon & n, vector <pair<int, int>> * v);

void Process_Mouse_Move(int x, int y);

void Process_Mouse(int button, int state, int x, int y);

void Display(void);

void Process_Normal_Keys(unsigned char key, int x, int y);

void Process_Special_Keys(int key, int x, int y);

void Initialize();

void Reshape(GLint w, GLint h);

#endif //PCH_H
