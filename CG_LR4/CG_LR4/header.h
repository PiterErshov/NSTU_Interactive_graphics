#define _USE_MATH_DEFINES 
#include <math.h>
#include <windows.h>
#include "glut.h"
#include <vector>
#include <gl\gl.h>
#include <stdio.h>
#include "tracer.h"
#include <iostream>

#define kSpeed  0.03f	//�������� ����������� ������

using namespace std;

GLint width, height;	//������ � ������ ����
GLfloat ratio;			//����������� ������ � ������ ����
Camera camera1;			//������
Scene scene1;			//�����
Raytracer raytracer1;	//������������

Light _LT;
bool rot = false;			//�������� �������� ������ � ������� ����

bool raytracer_mode = false;	//����� �����������
bool en_material = false;
bool g_bLight = true;
int block_size = 1;
bool changes = false;

