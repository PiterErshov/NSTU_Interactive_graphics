// Lab2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>


void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	//один буфер отрисовки окна, использование всех компонент RGB
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Lab2 Octogon");
	Initialize();								//начальные установки


	createMenu();								//создание меню
	glutKeyboardFunc(Process_Normal_Keys);		//обработка клавиш с кодами ascii
	glutSpecialFunc(Process_Special_Keys);		//обработка не-ascii клавиш
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutMouseFunc(Process_Mouse);
	glutPassiveMotionFunc(Process_Mouse_Move);
	glutMainLoop();
}