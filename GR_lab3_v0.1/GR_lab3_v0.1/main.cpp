#include "sours.h"

int main(int argc, char *argv[])
{
	width = 1200;
	height = 500;
	glutInit(&argc, argv);

	//�������� ����� �������/������� �����������
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("�������������� �����������");
	Initialize();								//��������� ���������

	glutKeyboardFunc(Process_Normal_Keys);		//��������� ������ � ������ ascii
	glutSpecialFunc(Process_Special_Keys);		//��������� ��-ascii ������

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	glutIdleFunc(Display);
	glutPassiveMotionFunc(Process_Mouse_Move);

	glutMainLoop();
}