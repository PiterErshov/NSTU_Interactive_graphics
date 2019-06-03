#include "sours.h"

int main(int argc, char *argv[])
{
	width = 1200;
	height = 500;
	glutInit(&argc, argv);

	//включить буфер глубины/двойную буферизацию
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Тиражированный треугольник");
	Initialize();								//начальные установки

	glutKeyboardFunc(Process_Normal_Keys);		//обработка клавиш с кодами ascii
	glutSpecialFunc(Process_Special_Keys);		//обработка не-ascii клавиш

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	glutIdleFunc(Display);
	glutPassiveMotionFunc(Process_Mouse_Move);

	glutMainLoop();
}