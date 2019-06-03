#define _USE_MATH_DEFINES 
#include <math.h>
#include <windows.h>
#include "glut.h"
#include <vector>
#include <gl\gl.h>
#include <stdio.h>
#include "tracer.h"
#include <iostream>

#define kSpeed  0.03f	//скорость перемещения камеры

using namespace std;

GLint width, height;	//ширина и высота окна
GLfloat ratio;			//соотношение ширины и высоты окна
Camera camera1;			//камера
Scene scene1;			//сцена
Raytracer raytracer1;	//трассировщик

Light _LT;
bool rot = false;			//включить вращение камеры с помощью мыши

bool raytracer_mode = false;	//режим трассировки
bool en_material = false;
bool g_bLight = true;
int block_size = 1;
bool changes = false;

