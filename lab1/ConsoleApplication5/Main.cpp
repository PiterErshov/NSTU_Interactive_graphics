#include "pch.h"
#include <iostream>


void main(int argc, char *argv[])
{
								
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	//один буфер отрисовки окна, использование всех компонент RGB
	glutInitWindowSize(1300, 800);//уставить начальные размеры окна
	glutCreateWindow("CG_L1. GL_LINE_LOOP");
	Init();								//начальные установки

	Menu(0);								//создание меню
	glutKeyboardFunc(ASCII_Keys);		//обработка клавиш с кодами ascii
	glutSpecialFunc(NASCII_Keys);		//обработка не-ascii клавиш
	glutReshapeFunc(Resh);
	glutDisplayFunc(Disp);
	glutMouseFunc(Mouse);
	glutMainLoop();
}