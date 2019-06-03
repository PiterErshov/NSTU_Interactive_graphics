#include "header.h"

vector<pix> PIX;

void Reshape(GLint w, GLint h)
{
	if(!raytracer_mode)
	{
		//изменить размеры окна
		width = w; 
		height = h;
	
		//вычислить соотношение между шириной и высотой
		//предотвратить деление на 0
		if(height == 0)
			height = 1;
		ratio = 1. * width / height;

		//установить матрицу проекции/определяет объем сцены
		glMatrixMode(GL_PROJECTION);

		//загрузить единичную матрицу
		glLoadIdentity();

		//определить окно просмотра
		glViewport(0, 0, width, height);

		//использовать перспективную проекциию
		gluPerspective(60, ratio, 0.1f, 100.0f);	

		//возврат к матрице модели
		glMatrixMode(GL_MODELVIEW);
	}
	else
	{
		//изменить размеры окна
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

	// отрисовать источник света
	for (int i = 0; i < scene1.vector_Light.size(); i++)
	{
		Light temp = scene1.vector_Light[i];
		Sphere *l = new Sphere(0.1, temp.position, temp.color);
		l->Draw();

		GLfloat light1_Diffuse[] = { temp.color.red, temp.color.green, temp.color.blue, 1.0 }; // цвет света
		GLfloat light1_position[] = { temp.position.x, temp.position.y, temp.position.z, 1.0 }; // местоположение света
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
	gluLookAt(camera1.Position.x, camera1.Position.y, camera1.Position.z,
		camera1.View.x, camera1.View.y, camera1.View.z,
		camera1.UpVector.x, camera1.UpVector.y, camera1.UpVector.z);

	// настроить освещение
	tune_lighting();


	//отрисовать сцену
	scene1.Draw();

	//есди включен режим трассировки
	if(raytracer_mode)
	{
		
		//установить параметры трассировщика
		raytracer1 = Raytracer(camera1.Position, scene1, camera1, ratio, width, height, block_size);
		
		if (changes) {

			cout << "\nВычисляю лучи";
			PIX.clear();
			raytracer1.Raytrace();//запустить трассировку
			cout << "\nВычислил!\n";
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
	//включение/выключение вращения камеры мышью
	if(key == 'q' || key == 'Q')
	{
			rot = !rot;
			ShowCursor(!rot);
	}
	
	//включение/выключение трассировки
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

	//включение/выключение режима включения/исключения фигур
	if(key == 'e' || key == 'E')
	{
		//переключить режим
		 scene1.add_del = !scene1.add_del;

		 //если режим включен
		 if(scene1.add_del)
		 {
			 //задать начальные установки
			 scene1.sphere_mod = true;
			 scene1.tetrahedron_mod = false;
			 scene1.active_Sphere = 0;
			 scene1.active_Tetrahedron = 0;
		 }

		 //иначе
		 else
		 {
			 scene1. sphere_mod = false;
			 scene1.tetrahedron_mod = false;
		 }
	}

	//включить исключить фигуру
	if(key == 'r' || key == 'R')
	{
		//если включен режив включения/исключения фигур
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

	//движение камеры
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

	// выбор источника света
	
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

	//перемещение источника света
	if(key == 'i')//поднять
	{
		scene1.vector_Light[0].position.y++;
	}
	if(key == 'p')//опустить
	{
		scene1.vector_Light[0].position.y--;
	}
	if(key == 'o')//вперёд
	{
		scene1.vector_Light[0].position.x++;
	}
	if(key == 'l')//назад
	{
		scene1.vector_Light[0].position.x--;
	}
	if(key == ';')//вправо
	{
		scene1.vector_Light[0].position.z++;
	}
	if(key == 'k')//влево
	{
		scene1.vector_Light[0].position.z--;
	}
	if (key == 'y')//включить/выключить фоновый свет
	{
		if(scene1.ambi == true)
			scene1.ambi = false;
		else
			scene1.ambi = true;
		Reshape(width, height);
	}
	if (key == 'h')//включить/выключить цвет сферы
	{
		if (scene1.clsh == true)
			scene1.clsh = false;
		else
			scene1.clsh = true;
		Reshape(width, height);
	}
	if (key == 'n')//включить/выключить цвет тетраэдера
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
	
	//переключать фигуры вперед
	if(key == GLUT_KEY_UP)
	{
		if(scene1.add_del)
			scene1.switch_forward();
	}

	//переключать фигуры назад
	if(key == GLUT_KEY_DOWN)
	{
		if(scene1.add_del)
			scene1.switch_backward();
	}

	//сменить активный контейнер фигур
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
	camera1.PositionCamera(-12.0f, 4.0f, -3.0f, -5.5f, 1.0f, -1.5f, 0.0f, 1.0f,  0.0f);	//установить начальную позицию камеры
}

int main(int argc, char *argv[])
{
	width = 1200;
	height = 800;
	glutInit(&argc, argv);
	setlocale(LC_ALL, "Russian");
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Трассировка");
	Initialize();
	glutKeyboardFunc(Process_Normal_Keys);
	glutSpecialFunc(Process_Special_Keys);		
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutPassiveMotionFunc(Process_Mouse_Move);
	glutMainLoop();
}