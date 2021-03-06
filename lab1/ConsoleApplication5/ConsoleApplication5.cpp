#include "pch.h"
#include <iostream>


void main(int argc, char *argv[])
{
	//width = 1300;
	//height = 800;								//уставить начальные размеры окна
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	//один буфер отрисовки окна, использование всех компонент RGB
	glutInitWindowSize(1300, 800);
	glutCreateWindow("CG_L1. GL_LINE_LOOP");
	Initialize();								//начальные установки

	createMenu();								//создание меню
	glutKeyboardFunc(Process_Normal_Keys);		//обработка клавиш с кодами ascii
	glutSpecialFunc(Process_Special_Keys);		//обработка не-ascii клавиш
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutMouseFunc(Process_Mouse);
	glutMainLoop();
}