#pragma once
#define _USE_MATH_DEFINES 
#include <windows.h>
#include <gl\glut.h>
#include "glaux.h"
#include <vector>
#include <gl\gl.h>
#include <stdio.h>
#include "Camera.h"

using namespace std;

float kSpeed = 0.03f;	//скорость перемещения камеры
#define NTEXT 8			//количество текстур
bool Mult = false;		//тип наложения текстуры
float angle = 60;		//угол в перспективной проекции
float factor = 10;		//множитель в ортографической проекции
GLint width, height;	//ширина и высота окна
Camera camera1;			//камера
bool rot = false;		//включить вращение камеры с помощью мыши
bool persp = true;		//включить перспективную проекцию
bool wireframe = false;	//каркасный режим
bool snormal = false;	//отображение нормалей
bool smooth = false;	//сгладить нормали
bool grid = true;		//отображение сетки
bool g_bLight = true;	//включить освещение
bool textureMod = false;//отображение текстуры
int light_sours = 1;	//текущий источник света
float shift_m[4][4];	//матрица сдвига
float rot_m[4][4];		//матрица поворота
float scale_m[4][4];	//матрица масштаба
unsigned int names_tex[NTEXT];//имена текстур
int nt = 0;				//номер текстуры

//структура для хранения треугольника
struct triangle
{
	Vector3f one;
	Vector3f two;
	Vector3f tr;
};

vector <float> percent;				//вектор процентных положений сечений тиражирования
vector <Vector3f> normals;			//вектор нормалей/содержит нормаль каждого полигона/
vector <Vector3f> sm_normals;	//вектор сглаженных нормалей/содержит нормаль к каждой вершине
vector <Vector3f> traj;		//траектория тиражирования
vector <Vector3f> scales;	//масштабирование каждого сечения
vector <triangle> fig;	//набор сечений треугольника
triangle temp;	//шаблон треугольника

//умножение координаты на указанную матрицу
Vector3f multCoord(float _matrix[4][4], Vector3f _vec)
{
	float vec[4] = { _vec.x, _vec.y, _vec.z, 1.0f };
	float resVec[4];

	for (int i = 0; i < 4; i++)
	{
		double s = 0;
		for (int j = 0; j < 4; j++)
			s += _matrix[i][j] * vec[j];
		resVec[i] = s;
	}
	return Vector3f::Vector3f(resVec[0], resVec[1], resVec[2]);
}

//определить матрицу сдвига
void makeTranslateMatrix(float dx, float dy, float dz)
{
	shift_m[0][0] = 1; shift_m[0][1] = 0;
	shift_m[0][2] = 0; shift_m[0][3] = dx;
	shift_m[1][0] = 0; shift_m[1][1] = 1;
	shift_m[1][2] = 0; shift_m[1][3] = dy;
	shift_m[2][0] = 0; shift_m[2][1] = 0;
	shift_m[2][2] = 1; shift_m[2][3] = dz;
	shift_m[3][0] = 0; shift_m[3][1] = 0;
	shift_m[3][2] = 0; shift_m[3][3] = 1;
}

//определить матрицу поворота/относительно произвольной оси/
void makeRotateMatrix(float angle, Vector3f Axis)
{
	float c = (float)cos(angle* M_PI / 180.0);
	float s = (float)sin(angle* M_PI / 180.0);
	Vector3f Os = Normalize(Axis);

	rot_m[0][0] = c + (1 - c)*Os.x*Os.x;
	rot_m[0][1] = (1 - c)*Os.x*Os.y - s * Os.z;
	rot_m[0][2] = (1 - c)*Os.x*Os.z + s * Os.y;
	rot_m[0][3] = 0;
	rot_m[1][0] = (1 - c)*Os.x*Os.y + s * Os.z;
	rot_m[1][1] = c + (1 - c)*Os.y*Os.y;
	rot_m[1][2] = (1 - c)*Os.z*Os.y - s * Os.x;
	rot_m[1][3] = 0;
	rot_m[2][0] = (1 - c)*Os.x*Os.z - s * Os.y;
	rot_m[2][1] = (1 - c)*Os.z*Os.y + s * Os.x;
	rot_m[2][2] = c + (1 - c)*Os.z*Os.z;
	rot_m[2][3] = 0;
	rot_m[3][0] = 0;
	rot_m[3][1] = 0;
	rot_m[3][2] = 0;
	rot_m[3][3] = 1;
}

//определить матрицу масштаба
void makeScaleMatrix(float sx, float sy, float sz)
{
	scale_m[0][0] = sx; scale_m[0][1] = 0;
	scale_m[0][2] = 0; scale_m[0][3] = 0;
	scale_m[1][0] = 0; scale_m[1][1] = sy;
	scale_m[1][2] = 0; scale_m[1][3] = 0;
	scale_m[2][0] = 0; scale_m[2][1] = 0;
	scale_m[2][2] = sz; scale_m[2][3] = 0;
	scale_m[3][0] = 0; scale_m[3][1] = 0;
	scale_m[3][2] = 0; scale_m[3][3] = 1;
}

//трансформация нового треугольника
triangle transform(int num, float angle, Vector3f Axis, Vector3f shift)
{
	triangle _triangle;
	_triangle.one = temp.one;
	_triangle.two = temp.two;
	_triangle.tr = temp.tr;

	//если угол не равен нулю - повернуть треугольник
	if (angle != 0)
	{
		makeRotateMatrix(angle, Axis);
		_triangle.one = multCoord(rot_m, _triangle.one);
		_triangle.two = multCoord(rot_m, _triangle.two);
		_triangle.tr = multCoord(rot_m, _triangle.tr);
	}

	//масштабируем треугольник
	makeScaleMatrix(scales[num].x, scales[num].y, scales[num].z);
	_triangle.one = multCoord(scale_m, _triangle.one);
	_triangle.two = multCoord(scale_m, _triangle.two);
	_triangle.tr = multCoord(scale_m, _triangle.tr);

	//сдвинуть треугольник
	makeTranslateMatrix(shift.x, shift.y, shift.z);
	_triangle.one = multCoord(shift_m, _triangle.one);
	_triangle.two = multCoord(shift_m, _triangle.two);
	_triangle.tr = multCoord(shift_m, _triangle.tr);

	return _triangle;
}

//тиражировать фигуру
void makeDuplication()
{
	FILE *in1;
	FILE *in2;
	FILE *in3;

	in1 = fopen("triangle.txt", "r");
	in2 = fopen("trajectory.txt", "r");
	in3 = fopen("percent.txt", "r");

	//считать координаты шаблонного треугольника
	fscanf(in1, "%f%f%f", &temp.one.x, &temp.one.y, &temp.one.z);
	fscanf(in1, "%f%f%f", &temp.two.x, &temp.two.y, &temp.two.z);
	fscanf(in1, "%f%f%f", &temp.tr.x, &temp.tr.y, &temp.tr.z);

	Vector3f coord;
	Vector3f scale;
	float ang;

	//считать координаты траектории
	while (!feof(in2))
	{
		fscanf(in2, "%f%f%f", &coord.x, &coord.y, &coord.z);
		traj.push_back(coord);
	}

	float per;
	while (!feof(in3))
	{
		fscanf(in3, "%f", &per);
		fscanf(in3, "%f%f%f", &scale.x, &scale.y, &scale.z);
		percent.push_back(per);
		scales.push_back(scale);
	}
	fclose(in1);
	fclose(in2);
	fclose(in3);

	Vector3f Axis;	//ось поворота
	//предыдущее направление - нормаль шаблонного треугольника
	Vector3f predPath = norm_pol(temp.one, temp.two, temp.tr);
	Vector3f curPath;	//текущее направление
	triangle _triangle;

	int i;
	float trajLength = 0;	//длина пути

	//вычисление длины пути
	for (i = 0; i < traj.size() - 1; i++)
	{
		curPath = traj[i + 1] - traj[i];
		trajLength += Norm(curPath);
	}

	for (i = 0; i < percent.size(); i++)
	{
		//расстояние до точки тиражирования
		float pointDistance = (trajLength*percent[i]) / 100.;

		//поиск отрезка траектории, которому принадлежит точка тиражирования
		int j;
		Vector3f curPathLength;	//текущее направление вектора траектории
		float curLength = 0;	//текущая длина пути
		float predLength = 0;	//предыдущая длина пути
		bool found = false;		//найден отрезок траектории

		int numPath;	//индекс начала отрезка траектории

		//если это последнее сечение
		if (i == percent.size() - 1)
			numPath = traj.size() - 2;
		else
		{
			for (j = 0; j < traj.size() - 1, !found; j++)
			{
				curPathLength = traj[j + 1] - traj[j];
				curLength += Norm(curPathLength);

				//если точка тиражирования пренадлежит текущему отрезку
				if (pointDistance >= predLength && pointDistance <= curLength)
					found = true;
				else
					predLength = curLength;
			}

			numPath = j - 1;
		}
		//вычислить текущее направление
		curPath = traj[numPath + 1] - traj[numPath];

		//величина сдвига от начала отрезка, которому принадлежит точка тиражирования
		float localPath = pointDistance - predLength;
		if (i == percent.size() - 1)
			localPath = Norm(curPath);
		float normCurPath = Norm(curPath);

		//вычисление координат сдвига
		Vector3f shift = Vector3f(traj[numPath].x + (localPath * curPath.x) / normCurPath,
			traj[numPath].y + (localPath * curPath.y) / normCurPath,
			traj[numPath].z + (localPath * curPath.z) / normCurPath);

		//вычислить скалярное произведение
		float scal = scalar(curPath, predPath);

		//вычислить ось
		Axis = Cross(curPath, predPath);

		//вычислить угол поворота
		ang = acos(scal / (Norm(curPath) * Norm(predPath)))* 180.0 / M_PI;

		//в зависимости от знака скалярного произведения изменить угол
		if (scal < 0)
			ang = 180 - ang;
		else
			ang = -(180 + ang);
		if (ang == 180) ang = 0;

		//определить координаты нового треугольника
		_triangle = transform(i, ang, Axis, shift);

		//добавить треугольник в вектор
		fig.push_back(_triangle);
	}
}
//вычисление нормалей к плоскостям
void calcNormals()
{
	Vector3f normal;
	//отдельно вычисляются нормали к первому треугольнику
	normal = norm_pol(fig[0].one, fig[0].two, fig[0].tr);
	normals.push_back(normal);

	for (int i = 0; i < fig.size() - 1; i++)
	{
		normal = norm_pol(fig[i].two, fig[i].one, fig[i + 1].one);
		normals.push_back(normal);

		normal = norm_pol(fig[i].one, fig[i].tr, fig[i + 1].tr);
		normals.push_back(normal);

		normal = norm_pol(fig[i].tr, fig[i].two, fig[i + 1].two);
		normals.push_back(normal);
	}

	//и к последнему
	int j = fig.size() - 1;
	normal = norm_pol(fig[j].one, fig[j].tr, fig[j].two);
	normals.push_back(normal);
}

//вычисление сглаженных нормалей
void smoothNormals()
{
	Vector3f _smoothNormal;

	_smoothNormal = (normals[0] + normals[1] + normals[2]) / 3.0;
	sm_normals.push_back(_smoothNormal);

	_smoothNormal = (normals[0] + normals[1] + normals[3]) / 3.0;
	sm_normals.push_back(_smoothNormal);

	_smoothNormal = (normals[0] + normals[2] + normals[3]) / 3.0;
	sm_normals.push_back(_smoothNormal);

	int nN = 1;
	for (int i = 1; i < fig.size() - 1; i++)
	{
		_smoothNormal = (normals[nN] + normals[nN + 1] + normals[nN + 3] + normals[nN + 4]) / 4.0;
		sm_normals.push_back(_smoothNormal);

		_smoothNormal = (normals[nN] + normals[nN + 2] + normals[nN + 3] + normals[nN + 5]) / 4.0;
		sm_normals.push_back(_smoothNormal);

		_smoothNormal = (normals[nN + 1] + normals[nN + 2] + normals[nN + 4] + normals[nN + 5]) / 4.0;
		sm_normals.push_back(_smoothNormal);
		nN += 3;
	}
	int j = normals.size() - 1;
	_smoothNormal = (normals[j] + normals[j - 2] + normals[j - 3]) / 3.0;
	sm_normals.push_back(_smoothNormal);

	_smoothNormal = (normals[j] + normals[j - 1] + normals[j - 3]) / 3.0;
	sm_normals.push_back(_smoothNormal);

	_smoothNormal = (normals[j] + normals[j - 1] + normals[j - 2]) / 3.0;
	sm_normals.push_back(_smoothNormal);
}
//*/
void loadTexture()//загрузка текстуры
{
	FILE *F;
	char str[20] = "1.bmp";
	char num = '1';
	auto widthT = width;
	auto heightT = height;
	glGenTextures(NTEXT, names_tex); //создание имен текстур 
	for (int i = 0; i < NTEXT; i++)
	{
		str[0] = num;
		num++;
		if ((F = fopen(str, "rb")) != NULL)
		{
			/*Перемещаемся в bmp-файле на нужную позицию, и считываем ширину и длинну */
			fseek(F, 18, SEEK_SET);
			fread(&(widthT), 2, 1, F);
			fseek(F, 2, SEEK_CUR);
			fread(&(heightT), 2, 1, F);

			/* Выделяем память под изображение. Если память не выделилась, закрываем файл и выходим с ошибкой */
			auto pixels = new GLubyte[3 * widthT * heightT];

			/* Считываем изображение в память по 3 бита, то бишь RGB для каждого пикселя */
			fseek(F, 30, SEEK_CUR);
			fread(pixels, 3, widthT * heightT, F);

			glBindTexture(GL_TEXTURE_2D, names_tex[i]); //выбрать текущую текстуру 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, widthT, heightT, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

			free(pixels);
			fclose(F);
		}
	}
}

void Reshape(GLint w, GLint h)
{
	//изменить размеры окна
	width = w;
	height = h;

	//вычислить соотношение между шириной и высотой
	//предотвратить деление на 0
	if (height == 0)
		height = 1;
	float ratio = 1. * width / height;

	//установить матрицу проекции/определяет объем сцены
	glMatrixMode(GL_PROJECTION);

	//загрузить единичную матрицу
	glLoadIdentity();

	//определить окно просмотра
	glViewport(0, 0, width, height);

	//если стоит режим перспективы
	if (persp)
		gluPerspective(60, ratio, 0.1f, 10000.0f);	//использовать перспективную проекциию

	//иначе - ортографическую проекцию
	else
		if (width >= height)
			glOrtho(-factor * ratio, factor*ratio, -factor, factor, -10000.0f, 10000.0f);
		else
			glOrtho(-factor, factor, -factor / ratio, factor / ratio, -10000.0f, 10000.0f);

	//возврат к матрице модели
	glMatrixMode(GL_MODELVIEW);
}

void Draw3DSGrid()
{
	glColor3ub(0, 0, 255);;
	for (float i = -50; i <= 50; i += 1)
	{
		glBegin(GL_LINES);
		// Ось Х
		glVertex3f(-50, 0, i);
		glVertex3f(50, 0, i);

		// Ось Z
		glVertex3f(i, 0, -50);
		glVertex3f(i, 0, 50);
		glEnd();
	}
}
//отрисовка тиражированной фигуры
void drawing_f()
{
	Vector3f normal;
	int nN = 0;

	//НЕ КАРКАСНЫЙ РЕЖИМ
	if (!wireframe)
	{
		//НОРМАЛИ НЕ СГЛАЖЕНЫ
		if (!smooth)
		{
			//ТЕКСТУРИРОВАНИЕ ОТКЛЮЧЕНО
			if (!textureMod)
			{
				//устанавливается нормаль для каждой плоскости
				glBegin(GL_TRIANGLES);
				glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;
				glColor3ub(255, 0, 0);;
				glVertex3f(fig[0].one.x, fig[0].one.y, fig[0].one.z);
				glVertex3f(fig[0].two.x, fig[0].two.y, fig[0].two.z);
				glVertex3f(fig[0].tr.x, fig[0].tr.y, fig[0].tr.z);
				glEnd();

				for (int i = 0; i < fig.size() - 1; i++)
				{
					glBegin(GL_POLYGON);
					glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;

					glColor3ub(255, 0, 0);;
					glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
					glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);
					glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);
					glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
					glEnd();

					glBegin(GL_POLYGON);
					glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;

					glColor3ub(255, 0, 0);;
					glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
					glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);
					glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);
					glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
					glEnd();

					glBegin(GL_POLYGON);
					glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;

					glColor3ub(255, 0, 0);;
					glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
					glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);
					glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);
					glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
					glEnd();
				}

				int j = fig.size() - 1;
				glBegin(GL_TRIANGLES);
				glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;

				glColor3ub(255, 0, 0);;
				glVertex3f(fig[j].one.x, fig[j].one.y, fig[j].one.z);
				glVertex3f(fig[j].two.x, fig[j].two.y, fig[j].two.z);
				glVertex3f(fig[j].tr.x, fig[j].tr.y, fig[j].tr.z);
				glEnd();
			}

			//ТЕКСТУРИРОВАНИЕ ВКЛЮЧЕНО
			else
			{
				glEnable(GL_TEXTURE_2D);	//разрешить текстурирование
				glBindTexture(GL_TEXTURE_2D, names_tex[nt]);	//выбрать текстуру
				if (!Mult)
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);	//умножение
				else
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				//устанавливается нормаль для каждой плоскости
				//и тестурные координаты
				glBegin(GL_TRIANGLES);
				glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;
				glColor3ub(255, 0, 0);;
				glTexCoord2f(0, 0); glVertex3f(fig[0].one.x, fig[0].one.y, fig[0].one.z);
				glTexCoord2f(0.5, 1); glVertex3f(fig[0].two.x, fig[0].two.y, fig[0].two.z);
				glTexCoord2f(1, 0); glVertex3f(fig[0].tr.x, fig[0].tr.y, fig[0].tr.z);
				glEnd();

				for (int i = 0; i < fig.size() - 1; i++)
				{
					glBegin(GL_POLYGON);
					glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;

					glColor3ub(255, 0, 0);;
					//glTexCoord2f(0, 1);
					glTexCoord2f(percent[i] / 100., 0.5);
					glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
					//glTexCoord2f(1, 1);
					glTexCoord2f(percent[i + 1] / 100., 0.5);
					glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);
					//glTexCoord2f(1, 0);
					glTexCoord2f(percent[i + 1] / 100., 0.25);
					glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);
					//glTexCoord2f(0, 0);
					glTexCoord2f(percent[i] / 100., 0.25);
					glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
					glEnd();

					glBegin(GL_POLYGON);
					glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;

					glColor3ub(255, 0, 0);;
					//glTexCoord2f(0, 1); 
					glTexCoord2f(percent[i] / 100., 0.75);
					glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
					//glTexCoord2f(1, 1);
					glTexCoord2f(percent[i + 1] / 100., 0.75);
					glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);
					//glTexCoord2f(1, 0);
					glTexCoord2f(percent[i + 1] / 100., 0.25);
					glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);
					//glTexCoord2f(0, 0);
					glTexCoord2f(percent[i] / 100., 0.25);
					glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
					glEnd();

					glBegin(GL_POLYGON);
					glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;

					glColor3ub(255, 0, 0);;
					//glTexCoord2f(0, 1); 
					glTexCoord2f(percent[i] / 100., 0.5);
					glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
					//glTexCoord2f(1, 1); 
					glTexCoord2f(percent[i + 1] / 100., 0.5);
					glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);
					//glTexCoord2f(1, 0); 
					glTexCoord2f(percent[i + 1] / 100., 0.25);
					glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);
					//glTexCoord2f(0, 0); 
					glTexCoord2f(percent[i] / 100., 0.25);
					glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
					glEnd();
				}

				int j = fig.size() - 1;
				glBegin(GL_TRIANGLES);
				glNormal3f(normals[nN].x, normals[nN].y, normals[nN].z); nN++;

				glColor3ub(255, 0, 0);;
				glTexCoord2f(0, 0); glVertex3f(fig[j].one.x, fig[j].one.y, fig[j].one.z);
				glTexCoord2f(0.5, 1); glVertex3f(fig[j].two.x, fig[j].two.y, fig[j].two.z);
				glTexCoord2f(1, 0); glVertex3f(fig[j].tr.x, fig[j].tr.y, fig[j].tr.z);
				glEnd();

				//отключить текстурирование
				glDisable(GL_TEXTURE_2D);
			}
		}

		//НОРМАЛИ СГЛАЖЕНЫ
		else
		{
			//ТЕКСТУРИРОВАНИЕ ОТКЛЮЧЕНО
			if (!textureMod)
			{
				//устанавливается нормаль для каждой вершины
				glBegin(GL_TRIANGLES);
				glColor3ub(255, 0, 0);;

				glNormal3f(sm_normals[0].x, sm_normals[0].y, sm_normals[0].z);
				glVertex3f(fig[0].one.x, fig[0].one.y, fig[0].one.z);

				glNormal3f(sm_normals[1].x, sm_normals[1].y, sm_normals[1].z);
				glVertex3f(fig[0].two.x, fig[0].two.y, fig[0].two.z);

				glNormal3f(sm_normals[2].x, sm_normals[2].y, sm_normals[2].z);
				glVertex3f(fig[0].tr.x, fig[0].tr.y, fig[0].tr.z);
				glEnd();

				nN = 0;
				for (int i = 0; i < fig.size() - 1; i++)
				{
					glBegin(GL_POLYGON);
					glColor3ub(255, 0, 0);;

					glNormal3f(sm_normals[nN].x, sm_normals[nN].y, sm_normals[nN].z);
					glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);

					glNormal3f(sm_normals[nN + 3].x, sm_normals[nN + 3].y, sm_normals[nN + 3].z);
					glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);

					glNormal3f(sm_normals[nN + 4].x, sm_normals[nN + 4].y, sm_normals[nN + 4].z);
					glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);

					glNormal3f(sm_normals[nN + 1].x, sm_normals[nN + 1].y, sm_normals[nN + 1].z);
					glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
					glEnd();

					glBegin(GL_POLYGON);
					glColor3ub(255, 0, 0);;

					glNormal3f(sm_normals[nN + 2].x, sm_normals[nN + 2].y, sm_normals[nN + 2].z);
					glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);

					glNormal3f(sm_normals[nN + 5].x, sm_normals[nN + 5].y, sm_normals[nN + 5].z);
					glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);

					glNormal3f(sm_normals[nN + 3].x, sm_normals[nN + 3].y, sm_normals[nN + 3].z);
					glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);

					glNormal3f(sm_normals[nN].x, sm_normals[nN].y, sm_normals[nN].z);
					glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
					glEnd();

					glBegin(GL_POLYGON);
					glColor3ub(255, 0, 0);;

					glNormal3f(sm_normals[nN + 1].x, sm_normals[nN + 1].y, sm_normals[nN + 1].z);
					glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);

					glNormal3f(sm_normals[nN + 4].x, sm_normals[nN + 4].y, sm_normals[nN + 4].z);
					glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);

					glNormal3f(sm_normals[nN + 5].x, sm_normals[nN + 5].y, sm_normals[nN + 5].z);
					glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);

					glNormal3f(sm_normals[nN + 2].x, sm_normals[nN + 2].y, sm_normals[nN + 2].z);
					glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
					glEnd();

					nN += 3;
				}

				int j = fig.size() - 1;
				glBegin(GL_TRIANGLES);
				glColor3ub(255, 0, 0);;
				glNormal3f(sm_normals[nN].x, sm_normals[nN].y, sm_normals[nN].z);
				glVertex3f(fig[j].one.x, fig[j].one.y, fig[j].one.z);

				glNormal3f(sm_normals[nN + 1].x, sm_normals[nN + 1].y, sm_normals[nN + 1].z);
				glVertex3f(fig[j].two.x, fig[j].two.y, fig[j].two.z);

				glNormal3f(sm_normals[nN + 2].x, sm_normals[nN + 2].y, sm_normals[nN + 2].z);
				glVertex3f(fig[j].tr.x, fig[j].tr.y, fig[j].tr.z);
				glEnd();
			}

			//ТЕКСТУРИРОВАНИЕ ВКЛЮЧЕНО
			else
			{
				glEnable(GL_TEXTURE_2D);	//разрешить текстурирование
				glBindTexture(GL_TEXTURE_2D, names_tex[nt]);	//выбрать текстуру
				if (!Mult)
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);	//умножение
				else
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				//устанавливается нормаль для каждой вершины
				//и текстурные коорднаты
				glBegin(GL_TRIANGLES);
				glColor3ub(255, 0, 0);;

				glNormal3f(sm_normals[0].x, sm_normals[0].y, sm_normals[0].z);
				glTexCoord2f(0, 0); glVertex3f(fig[0].one.x, fig[0].one.y, fig[0].one.z);

				glNormal3f(sm_normals[1].x, sm_normals[1].y, sm_normals[1].z);
				glTexCoord2f(0.5, 1); glVertex3f(fig[0].two.x, fig[0].two.y, fig[0].two.z);

				glNormal3f(sm_normals[2].x, sm_normals[2].y, sm_normals[2].z);
				glTexCoord2f(1, 0); glVertex3f(fig[0].tr.x, fig[0].tr.y, fig[0].tr.z);
				glEnd();

				nN = 0;
				for (int i = 0; i < fig.size() - 1; i++)
				{
					glBegin(GL_POLYGON);
					glColor3ub(255, 0, 0);;

					glNormal3f(sm_normals[nN].x, sm_normals[nN].y, sm_normals[nN].z);
					glTexCoord2f(0, 1); glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);

					glNormal3f(sm_normals[nN + 3].x, sm_normals[nN + 3].y, sm_normals[nN + 3].z);
					glTexCoord2f(1, 1); glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);

					glNormal3f(sm_normals[nN + 4].x, sm_normals[nN + 4].y, sm_normals[nN + 4].z);
					glTexCoord2f(1, 0); glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);

					glNormal3f(sm_normals[nN + 1].x, sm_normals[nN + 1].y, sm_normals[nN + 1].z);
					glTexCoord2f(0, 0); glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
					glEnd();

					glBegin(GL_POLYGON);
					glColor3ub(255, 0, 0);;

					glNormal3f(sm_normals[nN + 2].x, sm_normals[nN + 2].y, sm_normals[nN + 2].z);
					glTexCoord2f(0, 1); glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);

					glNormal3f(sm_normals[nN + 5].x, sm_normals[nN + 5].y, sm_normals[nN + 5].z);
					glTexCoord2f(1, 1); glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);

					glNormal3f(sm_normals[nN + 3].x, sm_normals[nN + 3].y, sm_normals[nN + 3].z);
					glTexCoord2f(1, 0); glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);

					glNormal3f(sm_normals[nN].x, sm_normals[nN].y, sm_normals[nN].z);
					glTexCoord2f(0, 0); glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
					glEnd();

					glBegin(GL_POLYGON);
					glColor3ub(255, 0, 0);;

					glNormal3f(sm_normals[nN + 1].x, sm_normals[nN + 1].y, sm_normals[nN + 1].z);
					glTexCoord2f(0, 1); glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);

					glNormal3f(sm_normals[nN + 4].x, sm_normals[nN + 4].y, sm_normals[nN + 4].z);
					glTexCoord2f(1, 1); glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);

					glNormal3f(sm_normals[nN + 5].x, sm_normals[nN + 5].y, sm_normals[nN + 5].z);
					glTexCoord2f(1, 0); glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);

					glNormal3f(sm_normals[nN + 2].x, sm_normals[nN + 2].y, sm_normals[nN + 2].z);
					glTexCoord2f(0, 0); glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
					glEnd();
				}

				int j = fig.size() - 1;
				glBegin(GL_TRIANGLES);
				glColor3ub(255, 0, 0);;

				glNormal3f(sm_normals[nN].x, sm_normals[nN].y, sm_normals[nN].z);
				glTexCoord2f(0, 0); glVertex3f(fig[j].one.x, fig[j].one.y, fig[j].one.z);

				glNormal3f(sm_normals[nN + 1].x, sm_normals[nN + 1].y, sm_normals[nN + 1].z);
				glTexCoord2f(0.5, 1); glVertex3f(fig[j].two.x, fig[j].two.y, fig[j].two.z);

				glNormal3f(sm_normals[nN + 2].x, sm_normals[nN + 2].y, sm_normals[nN + 2].z);
				glTexCoord2f(1, 0); glVertex3f(fig[j].tr.x, fig[j].tr.y, fig[j].tr.z);
				glEnd();

				//отключить текстурирование
				glDisable(GL_TEXTURE_2D);
			}
		}
	}

	//КАРКАСНЫЙ РЕЖИМ
	else
	{
		glBegin(GL_LINE_STRIP);
		glColor3ub(0, 255, 0);
		for (int i = 0; i < fig.size(); i++)
			glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glColor3ub(0, 255, 0);
		for (int i = 0; i < fig.size(); i++)
			glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glColor3ub(0, 255, 0);
		for (int i = 0; i < fig.size(); i++)
			glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glColor3ub(0, 255, 0);
		for (int i = 0; i < traj.size(); i++)
			glVertex3f(traj[i].x, traj[i].y, traj[i].z);
		glEnd();

		for (int i = 0; i < fig.size(); i++)
		{
			glBegin(GL_LINE_LOOP);
			glColor3ub(0, 255, 0);
			glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
			glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
			glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
			glEnd();
		}
	}

	//ПОКАЗ НОРМАЛЕЙ
	if (snormal)
	{
		//НОРМАЛИ НЕ СГЛАЖЕНЫ
		if (!smooth)
		{
			glBegin(GL_LINE_STRIP);
			glColor3ub(0, 255, 255);
			glVertex3f(fig[0].one.x, fig[0].one.y, fig[0].one.z);
			glVertex3f(fig[0].one.x + normals[0].x, fig[0].one.y + normals[0].y, fig[0].one.z + normals[0].z);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glColor3ub(0, 255, 255);
			glVertex3f(fig[0].two.x, fig[0].two.y, fig[0].two.z);
			glVertex3f(fig[0].two.x + normals[0].x, fig[0].two.y + normals[0].y, fig[0].two.z + normals[0].z);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glColor3ub(0, 255, 255);
			glVertex3f(fig[0].tr.x, fig[0].tr.y, fig[0].tr.z);
			glVertex3f(fig[0].tr.x + normals[0].x, fig[0].tr.y + normals[0].y, fig[0].tr.z + normals[0].z);
			glEnd();

			nN = 1;
			for (int i = 0; i < fig.size() - 1; i++)
			{
				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
				glVertex3f(fig[i].one.x + normals[nN].x, fig[i].one.y + normals[nN].y, fig[i].one.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);
				glVertex3f(fig[i + 1].one.x + normals[nN].x, fig[i + 1].one.y + normals[nN].y, fig[i + 1].one.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);
				glVertex3f(fig[i + 1].two.x + normals[nN].x, fig[i + 1].two.y + normals[nN].y, fig[i + 1].two.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
				glVertex3f(fig[i].two.x + normals[nN].x, fig[i].two.y + normals[nN].y, fig[i].two.z + normals[nN].z);
				glEnd();

				nN++;

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
				glVertex3f(fig[i].tr.x + normals[nN].x, fig[i].tr.y + normals[nN].y, fig[i].tr.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);
				glVertex3f(fig[i + 1].tr.x + normals[nN].x, fig[i + 1].tr.y + normals[nN].y, fig[i + 1].tr.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i + 1].one.x, fig[i + 1].one.y, fig[i + 1].one.z);
				glVertex3f(fig[i + 1].one.x + normals[nN].x, fig[i + 1].one.y + normals[nN].y, fig[i + 1].one.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
				glVertex3f(fig[i].one.x + normals[nN].x, fig[i].one.y + normals[nN].y, fig[i].one.z + normals[nN].z);
				glEnd();

				nN++;

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
				glVertex3f(fig[i].two.x + normals[nN].x, fig[i].two.y + normals[nN].y, fig[i].two.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i + 1].two.x, fig[i + 1].two.y, fig[i + 1].two.z);
				glVertex3f(fig[i + 1].two.x + normals[nN].x, fig[i + 1].two.y + normals[nN].y, fig[i + 1].two.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i + 1].tr.x, fig[i + 1].tr.y, fig[i + 1].tr.z);
				glVertex3f(fig[i + 1].tr.x + normals[nN].x, fig[i + 1].tr.y + normals[nN].y, fig[i + 1].tr.z + normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
				glVertex3f(fig[i].tr.x + normals[nN].x, fig[i].tr.y + normals[nN].y, fig[i].tr.z + normals[nN].z);
				glEnd();

				nN++;
			}

			int j = fig.size() - 1;
			glBegin(GL_LINE_STRIP);
			glColor3ub(0, 255, 255);
			glVertex3f(fig[j].one.x, fig[j].one.y, fig[j].one.z);
			glVertex3f(fig[j].one.x + normals[nN].x, fig[j].one.y + normals[nN].y, fig[j].one.z + normals[nN].z);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glColor3ub(0, 255, 255);
			glVertex3f(fig[j].two.x, fig[j].two.y, fig[j].two.z);
			glVertex3f(fig[j].two.x + normals[nN].x, fig[j].two.y + normals[nN].y, fig[j].two.z + normals[nN].z);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glColor3ub(0, 255, 255);
			glVertex3f(fig[j].tr.x, fig[j].tr.y, fig[j].tr.z);
			glVertex3f(fig[j].tr.x + normals[nN].x, fig[j].tr.y + normals[nN].y, fig[j].tr.z + normals[nN].z);
			glEnd();

		}

		//НОРМАЛИ СГЛАЖЕНЫ
		else
		{
			nN = 0;
			for (int i = 0; i < fig.size(); i++)
			{
				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].one.x, fig[i].one.y, fig[i].one.z);
				glVertex3f(fig[i].one.x + sm_normals[nN].x, fig[i].one.y + sm_normals[nN].y, fig[i].one.z + sm_normals[nN].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].two.x, fig[i].two.y, fig[i].two.z);
				glVertex3f(fig[i].two.x + sm_normals[nN + 1].x, fig[i].two.y + sm_normals[nN + 1].y, fig[i].two.z + sm_normals[nN + 1].z);
				glEnd();

				glBegin(GL_LINE_STRIP);
				glColor3ub(0, 255, 255);
				glVertex3f(fig[i].tr.x, fig[i].tr.y, fig[i].tr.z);
				glVertex3f(fig[i].tr.x + sm_normals[nN + 2].x, fig[i].tr.y + sm_normals[nN + 2].y, fig[i].tr.z + sm_normals[nN + 2].z);
				glEnd();
				nN += 3;
			}
		}
	}
}

//установка освещения
void Shine_a_Light()
{
	GLfloat ambience[4] = { 0.1f, 0.1f, 0.1f, 1.0 };	//Цвет мирового света
	GLfloat material_diffuse[] = { 0.19, 0.19, 0.19, 1 };
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glEnable(GL_COLOR_MATERIAL);
	if (light_sours == 1)
	{
		//направленный источник света
		//находится в бесконечности и свет от него распространяется в заданном направлении
		GLfloat amb[4] = { 1, 1, 1, 1 };
		GLfloat light0_direction[] = { 0, 0, 0, 0 };
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	}
	if (light_sours == 2)
	{
		//точечный источник света
		//убывание интенсивности с расстоянием
		//отключено (по умолчанию)
		GLfloat light1_diffuse[] = { 0.4, 0.7, 0.2 };
		GLfloat light1_position[] = { 0, 0, 0, 0 };
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambience);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	}
	if (light_sours == 3)
	{
		//точечный источник света
		//убывание интенсивности с расстоянием
		GLfloat ambi[4] = { 0.1, 0, 0, 0 };
		GLfloat light2_diffuse[] = { 2, 1.5, 0.2 };
		GLfloat light2_position[] = { -5.0, 8.0, -4.0, 1.0 };
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
		glLightfv(GL_LIGHT2, GL_AMBIENT, ambi);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
		glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 10);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.04);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.04);
	}
	if (light_sours == 4)
	{
		//прожектор
		//убывание интенсивности с расстоянием
		//отключено (по умолчанию)
		GLfloat ambi[4] = { -0.5, 0, 0, 0 };
		GLfloat light3_diffuse[] = { 0.8, 0.4, 0.2 };
		GLfloat light3_position[] = { -5.0, 8.0, -3.0, 1.0 };
		GLfloat light3_spot_direction[] = { 1.0, -1.0, -1.0 };
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
		glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 50);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_spot_direction);
	}
	if (light_sours == 5)
	{
		//прожектор		
		//включен рассчет убывания интенсивности для прожектора
		GLfloat ambi[4] = { -0.5, 0, 0, 0 };
		GLfloat light4_diffuse[] = { 0.9, 0.7, 0.2 };
		GLfloat light4_position[] = { -5.0, 8.0, -3.0, 1.0 };
		GLfloat light4_spot_direction[] = { 1.0, -1.0, -1.0 };
		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
		glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
		glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
		glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 50);
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light4_spot_direction);
		glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 20.0);
	}
}

void shp()
{
	GLUquadricObj *quadObj1;
	quadObj1 = gluNewQuadric();
	glPushMatrix();
	glTranslated(-5.0, 8.0, -3.0);
	glColor4ub(0, 255, 0, 0);
	gluQuadricDrawStyle(quadObj1, GLU_FILL);
	gluSphere(quadObj1, 0.5, 50, 50);

	gluDeleteQuadric(quadObj1);
	glPopMatrix();
}

void Display(void)
{
	//очистка буфера цвета и глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//включить буфер глубины
	glEnable(GL_DEPTH_TEST);

	if (g_bLight)
		glEnable(GL_LIGHTING);	//включить свет
	else
		glDisable(GL_LIGHTING); //выключить свет	
	//обнуление трансформации
	glLoadIdentity();
	//установить вид камеры
	gluLookAt(camera1.Pos.x, camera1.Pos.y, camera1.Pos.z,
		camera1.View.x, camera1.View.y, camera1.View.z,
		camera1.rot_vect.x, camera1.rot_vect.y, camera1.rot_vect.z);

	//устаносить освещение
	Shine_a_Light();

	//нарисовать сетку
	if (grid)
		Draw3DSGrid();

	//нарисовать тиражированную фигуру
	drawing_f();
	shp();
	//переместиться в точку взгляда
	glTranslatef(camera1.View.x, 0, camera1.View.z);

	//отключить все источники
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);

	//двойная буферизация
	glutSwapBuffers();
}

void Process_Mouse_Move(int x, int y)
{
	camera1.set_view_mouse(width, height);
}


void Process_Normal_Keys(unsigned char key, int x, int y)
{
	//приближение/отдаление в ортографической проекции
	if (key == '+')
	{
		if (factor > 0.1 && !persp)
		{
			factor -= 0.05;
			Reshape(width, height);
		}
	}
	if (key == '-' && !persp)
	{
		if (factor < 10)
		{
			factor += 0.05;
			Reshape(width, height);
		}
	}
	//включение/выключение вращения камеры мышью
	if (key == 'q' || key == 'Q')
	{
		rot = !rot;
		ShowCursor(!rot);
	}
	//переключение между режимами перспективной и ортографической проекции
	if (key == 'p' || key == 'P')
	{
		persp = !persp;
		Reshape(width, height);
	}
	//включение/выключение каркасного режима
	if (key == 'm' || key == 'M')
	{
		wireframe = !wireframe;
		if (wireframe)
			g_bLight = false;
		else
			g_bLight = true;
	}
	//включение/выключение отображения нормалей
	if (key == 'n' || key == 'N')
	{
		snormal = !snormal;
	}

	if (key == 'r' || key == 'R')
	{
		if (nt < NTEXT)
			nt++;
		else
			nt = 0;
	}
	//включение/выключение сглаженных нормалей
	if (key == 'b' || key == 'B')
	{
		smooth = !smooth;
	}
	//включение/выключение показа сетки
	if (key == 'g' || key == 'G')
	{
		grid = !grid;
	}
	//включение/выключение света
	if (key == 'l' || key == 'L')
	{
		g_bLight = !g_bLight;
	}

	//включение/выключение текстуры
	if (key == 't' || key == 'T')
	{
		textureMod = !textureMod;
	}

	//движение камеры
	if (key == 'w' || key == 'W')
	{
		camera1.move_c(kSpeed);
	}

	if (key == 's' || key == 'S')
	{
		camera1.move_c(-kSpeed);
	}

	if (key == 'd' || key == 'D')
	{
		camera1.r_obs(camera1.View, -kSpeed * 2.0f, 0.0f, 1.0f, 0.0f);
	}

	if (key == 'a' || key == 'A')
	{
		camera1.r_obs(camera1.View, kSpeed*2.0f, 0.0f, 1.0f, 0.0f);
	}
	//увеличить/уменьшить скорость перемещения камеры
	if (key == '<')
	{
		kSpeed += 0.01;
	}
	if (key == '>')
	{
		kSpeed -= 0.01;
	}
	//возврат на исходную позицию
	if (key == 'O' || key == 'o')
	{
		camera1.pos_camera(-18.0f, 0.5f, -3.0f, -12.0f, 0.5f, -3.0f, 0.0f, 1.0f, 0.0f);
	}
	//изменение режима наложения текстур
	if (key == 'x' || key == 'X')
	{
		if (!Mult)
			Mult = true;
		else
			Mult = false;
	}
	//выбор типа освещения
	if (key == '1')
		light_sours = 1;//фоновый
	if (key == '2')
		light_sours = 2;//эмиссионный 
	if (key == '3')
		light_sours = 3;//точечный
	if (key == '4')
		light_sours = 4;//прожекторный
	if (key == '5')
		light_sours = 5;//удалённый направленный
}
void Process_Special_Keys(int key, int x, int y)
{
	//движение камеры
	if (key == GLUT_KEY_UP)
	{
		camera1.move_c(kSpeed);
	}

	if (key == GLUT_KEY_DOWN)
	{
		camera1.move_c(-kSpeed);
	}

	if (key == GLUT_KEY_LEFT)
	{
		camera1.r_obs(camera1.View, -kSpeed * 2.0f, 0.0f, 1.0f, 0.0f);
	}

	if (key == GLUT_KEY_RIGHT)
	{
		camera1.r_obs(camera1.View, kSpeed*2.0f, 0.0f, 1.0f, 0.0f);
	}
	glutPostRedisplay();
}

void Initialize()
{
	Shine_a_Light();	//установить освещение
	camera1.pos_camera(-18.0f, 0.5f, -3.0f, -12.0f, 0.5f, -3.0f, 0.0f, 1.0f, 0.0f);	//установить начальную позицию камеры
	makeDuplication();	//рассчитать координаты тиражированной фигуры
	calcNormals();		//вычислить нормали
	smoothNormals();	//вычислить сглаженные нормали
	loadTexture();		//загрузить текстуру
}
