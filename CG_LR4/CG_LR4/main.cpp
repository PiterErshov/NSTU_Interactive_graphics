#include "header.h"

vector<pix> PIX;

void Reshape(GLint w, GLint h)
{
	if(!raytracer_mode)
	{
		//�������� ������� ����
		width = w; 
		height = h;
	
		//��������� ����������� ����� ������� � �������
		//������������� ������� �� 0
		if(height == 0)
			height = 1;
		ratio = 1. * width / height;

		//���������� ������� ��������/���������� ����� �����
		glMatrixMode(GL_PROJECTION);

		//��������� ��������� �������
		glLoadIdentity();

		//���������� ���� ���������
		glViewport(0, 0, width, height);

		//������������ ������������� ���������
		gluPerspective(60, ratio, 0.1f, 100.0f);	

		//������� � ������� ������
		glMatrixMode(GL_MODELVIEW);
	}
	else
	{
		//�������� ������� ����
		width = w; 
		height = h;

		if(height == 0)
			height = 1;
		ratio = 1. * width / height;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0,0,width,height);
		gluOrtho2D(0.0,width,0.0,height);
	}

	changes = true;
}

void tune_lighting()
{
	if(en_material)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);

	GLfloat Diffuse[] = { 3.0, 3.0, 3.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Diffuse);

	// ���������� �������� �����
	for (int i = 0; i < scene1.vector_Light.size(); i++)
	{
		Light temp = scene1.vector_Light[i];
		Sphere *l = new Sphere(0.1, temp.position, temp.color);
		l->Draw();

		GLfloat light1_Diffuse[] = { temp.color.red, temp.color.green, temp.color.blue, 1.0 }; // ���� �����
		GLfloat light1_position[] = { temp.position.x, temp.position.y, temp.position.z, 1.0 }; // �������������� �����
		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light1_Diffuse);
		glLightfv(GL_LIGHT2, GL_POSITION, light1_position);
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.04);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.04);

	}
}


void Display(void)
{
	//������� ������ ����� � �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�������� ����� �������
	glEnable(GL_DEPTH_TEST);

	if (g_bLight)
		glEnable(GL_LIGHTING);	//�������� ����
	else
		glDisable(GL_LIGHTING); //��������� ����	


	//��������� �������������
	glLoadIdentity();

	//���������� ��� ������
	gluLookAt(camera1.Position.x, camera1.Position.y, camera1.Position.z,
		camera1.View.x, camera1.View.y, camera1.View.z,
		camera1.UpVector.x, camera1.UpVector.y, camera1.UpVector.z);

	// ��������� ���������
	tune_lighting();


	//���������� �����
	scene1.Draw();

	//���� ������� ����� �����������
	if(raytracer_mode)
	{
		
		//���������� ��������� �������������
		raytracer1 = Raytracer(camera1.Position, scene1, camera1, ratio, width, height, block_size);
		
		if (changes) {

			cout << "\n�������� ����";
			PIX.clear();
			raytracer1.Raytrace();//��������� �����������
			cout << "\n��������!\n";
			changes = false;
		}
		raytracer1.draw();
		
	}
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glutSwapBuffers();
	glutPostRedisplay();
}

void Process_Mouse_Move(int x, int y)
{
	if (rot)
	{
		changes = true;
		camera1.SetViewByMouse(width, height);
	}
}

void Process_Normal_Keys(unsigned char key, int x, int y)
{
	changes = true;
	//���������/���������� �������� ������ �����
	if(key == 'q' || key == 'Q')
	{
			rot = !rot;
			ShowCursor(!rot);
	}
	
	//���������/���������� �����������
	if(key == 't' || key == 'T')
	{
		 raytracer_mode = !raytracer_mode;
		 if(!raytracer_mode)
			 Reshape(width, height);
	}

	if (key == '=')
		block_size++;
	if (key == '-')
		if(block_size > 1)
			block_size--;

	//���������/���������� ������ ���������/���������� �����
	if(key == 'e' || key == 'E')
	{
		//����������� �����
		 scene1.add_del = !scene1.add_del;

		 //���� ����� �������
		 if(scene1.add_del)
		 {
			 //������ ��������� ���������
			 scene1.sphere_mod = true;
			 scene1.tetrahedron_mod = false;
			 scene1.active_Sphere = 0;
			 scene1.active_Tetrahedron = 0;
		 }

		 //�����
		 else
		 {
			 scene1. sphere_mod = false;
			 scene1.tetrahedron_mod = false;
		 }
	}

	//�������� ��������� ������
	if(key == 'r' || key == 'R')
	{
		//���� ������� ����� ���������/���������� �����
		if(scene1.add_del)
		{
			if(scene1.sphere_mod)
				scene1.vector_Sphere[scene1.active_Sphere].display = !scene1.vector_Sphere[scene1.active_Sphere].display;

			if(scene1.tetrahedron_mod)
				scene1.vector_Tetrahedron[scene1.active_Tetrahedron].display = !scene1.vector_Tetrahedron[scene1.active_Tetrahedron].display;
		}
	}
	if (key == 'z' || key == 'Z')
	{
		en_material = !en_material;
	}

	//�������� ������
	if(key == 'w' || key == 'W')
	{
		camera1.MoveCamera(kSpeed);
	}
	if (key == 's' || key == 'S')
	{
		camera1.MoveCamera(-kSpeed);
	}
	if (key == 'a' || key == 'A')
	{
		camera1.RotateAroundPoint(camera1.View, -kSpeed*2.0f, 0.0f, 1.0f, 0.0f);
	}
	if (key == 'd' || key == 'D')
	{
		camera1.RotateAroundPoint(camera1.View, kSpeed*2.0f, 0.0f, 1.0f, 0.0f);
	}

	// ����� ��������� �����
	
	if (key == '1')
	{
		scene1.lighting_num = 1;
		scene1.set_l();
	}
	if (key == '2')
	{
		scene1.lighting_num = 2;
		scene1.set_l();
	}
	if (key == '3')
	{
		scene1.lighting_num = 3;
		scene1.set_l();
	}
	if (key == '4')
	{
		scene1.lighting_num = 4;
		scene1.set_l();
	}

	//����������� ��������� �����
	if(key == 'i')//�������
	{
		scene1.vector_Light[0].position.y++;
	}
	if(key == 'p')//��������
	{
		scene1.vector_Light[0].position.y--;
	}
	if(key == 'o')//�����
	{
		scene1.vector_Light[0].position.x++;
	}
	if(key == 'l')//�����
	{
		scene1.vector_Light[0].position.x--;
	}
	if(key == ';')//������
	{
		scene1.vector_Light[0].position.z++;
	}
	if(key == 'k')//�����
	{
		scene1.vector_Light[0].position.z--;
	}
	if (key == 'y')//��������/��������� ������� ����
	{
		if(scene1.ambi == true)
			scene1.ambi = false;
		else
			scene1.ambi = true;
		Reshape(width, height);
	}
	if (key == 'h')//��������/��������� ���� �����
	{
		if (scene1.clsh == true)
			scene1.clsh = false;
		else
			scene1.clsh = true;
		Reshape(width, height);
	}
	if (key == 'n')//��������/��������� ���� ����������
	{
		if (scene1.cltr == true)
			scene1.cltr = false;
		else
			scene1.cltr = true;
		Reshape(width, height);
	}
	glutPostRedisplay();
}

void Process_Special_Keys(int key, int x, int y)
{
	
	//����������� ������ ������
	if(key == GLUT_KEY_UP)
	{
		if(scene1.add_del)
			scene1.switch_forward();
	}

	//����������� ������ �����
	if(key == GLUT_KEY_DOWN)
	{
		if(scene1.add_del)
			scene1.switch_backward();
	}

	//������� �������� ��������� �����
	if(key == GLUT_KEY_LEFT)
	{
		if(scene1.add_del)
			if(scene1.sphere_mod)
			{
				if(scene1.vector_Tetrahedron.size()>0)
				{
					scene1.sphere_mod = false;
					scene1.tetrahedron_mod = true;
				}
			}
			else
			{
				if(scene1.vector_Sphere.size()>0)
				{
					scene1.sphere_mod = true;
					scene1.tetrahedron_mod = false;
				}
			}
	}
	glutPostRedisplay();
}

void Initialize()
{
	ratio = 1. * width / height;
	camera1.PositionCamera(-12.0f, 4.0f, -3.0f, -5.5f, 1.0f, -1.5f, 0.0f, 1.0f,  0.0f);	//���������� ��������� ������� ������
}

int main(int argc, char *argv[])
{
	width = 1200;
	height = 800;
	glutInit(&argc, argv);
	setlocale(LC_ALL, "Russian");
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("�����������");
	Initialize();
	glutKeyboardFunc(Process_Normal_Keys);
	glutSpecialFunc(Process_Special_Keys);		
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutPassiveMotionFunc(Process_Mouse_Move);
	glutMainLoop();
}