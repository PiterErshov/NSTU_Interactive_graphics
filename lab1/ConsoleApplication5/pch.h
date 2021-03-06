#ifndef PCH_H
#define PCH_H

#include <windows.h>
#include <gl/glut.h> 
#include <vector>
#include <gl\gl.h>
#include <vector>

// константы для подменю тип линии
#define SOLID		1
#define DOT			2
#define DASH		3
#define DOTDASH		4

// константы для подменю толщина линии
#define WIDTH_0_5	5
#define WIDTH_1		6
#define WIDTH_1_5	7
#define WIDTH_2		8
#define WIDTH_2_5	9
#define WIDTH_3		10
#define WIDTH_3_5	11
#define WIDTH_4		12

// константы для подменю цвет линии
#define RED		13
#define BLUE	14
#define GREEN	15
#define ORANGE	16
#define YELLOW	17
#define VIOLET	18
#define BLACK	19
#define WHITE	20

GLint width, height;	//ширина и высота окна
int current_group;		//текущий набор примитивов
bool drawing_mode;		//true - режим отрисовки
int max_group;			//максимальный порядковый номер набора (число наборов-1)
bool prev_space;		//true - был добавлен новый набор, но не отрисована ни одна линия

class point
{
public:
	GLint x, y; // GLint - signed int
	point(GLint p1, GLint p2);
};

struct line
{
public:
	std::vector <point> coord;
	int group;
};

class colour
{
public:
	GLint red, blue, green;
	int group;
	colour(GLint r, GLint b, GLint g, int gr);
};

class linetype
{
public:
	float widht;
	unsigned short type;
	int group;
	linetype(float _widht, unsigned short _type, int _gr);
};

point::point(GLint p1, GLint p2)
{
	x = p1;
	y = p2;
}

colour::colour(GLint r, GLint b, GLint g, int gr)
{
	red = r;
	blue = b;
	green = g;
	group = gr;
}

linetype::linetype(float _widht, unsigned short _type, int _gr)
{
	widht = _widht;
	type = _type;
	group = _gr;
}

std::vector <line> v;		//вектор ломаных
line l;					//отрисовываемая ломаная
std::vector <colour> colours;
std::vector <linetype> lineTypeVec;

void Up_Grup();//переход к следующему набору точек

void Down_Grup();//переход к предыдущему набору точек

void New_Grup();//новый набор точек

void Plus_Red();//увеличить значение RED на 10

void Plus_Green();//увеличить значение GREEN на 10

void Plus_Blue();//увеличить значение BLUE на 10

void Minus_Red();//уменьшить значение RED на 10

void Minus_Green();//уменьшить значение GREEN на 10

void Minus_Blue();//уменьшить значение BLUE на 10

void Del_Group();//удалить группу

void Save_Prim();//сохранить примитив

void Delete_Last_Prim();//удалить последний примитив

void MoveUp();//движение вверх

void MoveDown();//движение вниз

void MoveLeft();//движение влево 

void MoveRight();//движение вправо

void MoveUpR();//движение в верхний правый угол

void MoveUpL();//движение в верхний левый угол

void MoveDownR();//движение в нижний правый угол

void MoveDownL();//движение в нижний левый угол

void param(int opt);//обраьботка всех параметров

void Color(unsigned char key);//обработка изменения цветов

void Menu(int opt);//меню

void Mouse(int button, int state, int x, int y);//обработка мыши

void Disp(void);//подключение дисплея

void ASCII_Keys(unsigned char key, int x, int y);//обрабока ASCII исмволов

void NASCII_Keys(int key, int x, int y);//обрабока спец исмволов

void Init();//начальные параметры

void Resh(GLint w, GLint h);//изменение параметров окна

#endif 
