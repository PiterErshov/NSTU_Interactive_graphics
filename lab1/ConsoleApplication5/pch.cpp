#include "pch.h"

void Up_Grup()
{
	char str[50];
	//перейти к следующему набору
	current_group++;
	//перейти к нулевому, если был достигнут последний добавленный набор
	current_group = current_group % (max_group + 1);

	sprintf(str, "Current group: %i", current_group + 1);
	glutSetWindowTitle(str);
}

void Down_Grup()
{
	char str[50];
	//перейти к предыдущему набору
	current_group--;
	//перейти к последнему добавленному набору, если был пройден нулевой набор
	if (current_group < 0) current_group = max_group;

	sprintf(str, "Current group: %i", current_group + 1);
	glutSetWindowTitle(str);
}

void New_Grup()
{
	char str[50];
	if (!prev_space)	//исключает возможность добавления "пустого" набора
	{
		prev_space = true;	//был нажат пробел
		max_group++;		//увеличить число наборов
		current_group = max_group;	//перейти к новому набору
		colours.push_back(colour(0, 0, 0, current_group));	//установить изначальный (чёрный) цвет
		lineTypeVec.push_back(linetype(1.0, 0xFFFF, current_group));

		sprintf(str, "%i groups", max_group + 1);
		glutSetWindowTitle(str);
	}
}

void Plus_Red()
{
	if (colours[current_group].red + 10 < 255) colours[current_group].red += 10;
}

void Plus_Green()
{
	if (colours[current_group].green + 10 < 255) colours[current_group].green += 10;
}

void Plus_Blue()
{
	if (colours[current_group].blue + 10 < 255) colours[current_group].blue += 10;
}

void Minus_Red()
{
	if (colours[current_group].red - 10 >= 0) colours[current_group].red -= 10;
}

void Minus_Green()
{
	if (colours[current_group].green - 10 >= 0) colours[current_group].green -= 10;
}

void Minus_Blue()
{
	if (colours[current_group].blue - 10 >= 0) colours[current_group].blue -= 10;
}

void Del_Group()
{
	char str[50];
	int j, k;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		//удалить все ломаные, принадлежащие текущему набору
		if (v[j].group == current_group)
		{
			v.erase(v.begin() + j);
			k = v.size();
			j--;
		}
	}

	for (j = 0; j < v.size(); j++)
	{
		//уменьшить на 1 те номера наборов, которые были добавлены позже удаляемого
		if (v[j].group > current_group)
		{
			v[j].group--;
		}
	}

	//удалить информацию о цвете текущего набора
	colours.erase(colours.begin() + current_group);

	for (j = 0; j < colours.size(); j++)
	{
		//уменьшить на 1 те номера наборов, которые были добавлены позже удаляемого
		if (colours[j].group > current_group)
		{
			colours[j].group--;
		}
	}

	//уменьшить максимальный номер наборов
	//максимальный номер наборов не может быть меньше 0
	if (max_group) max_group--;

	//сменить текущий набор на следующий
	current_group = current_group % (max_group + 1);

	//если были удалены все наборы, добавить цвет для нулевого набора
	if (colours.size() == 0)
	{
		colours.push_back(colour(0, 0, 0, current_group));
		lineTypeVec.push_back(linetype(1.0, 0xFFFF, current_group));
	}

	sprintf(str, "%i groups", max_group + 1);
	glutSetWindowTitle(str);
}

void Save_Prim()
{
	int k, j;
	//добавить ломаную в вектор ломаных
	v.push_back(l);
	//"выйти" из режима рисования
	drawing_mode = false;

	//удалить данные о текущей ломаной
	k = l.coord.size();
	for (j = 0; j < k; j++)
	{
		l.coord.pop_back();
	}
}

void Delete_Last_Prim()
{
	int k;
	bool found, one_in_group;

	k = v.size();
	k--;
	found = false;
	one_in_group = true;

	//просмотреть вектор ломаных с конца
	for (k; k >= 0 && one_in_group; k--)
	{
		if (v[k].group == current_group)		//если ломаная принадлежит текущей группе
		{
			if (!found)
			{
				//удалить ломаную
				v.erase(v.begin() + k);
				//зафиксировать факт удаления ломаной
				found = true;
			}
			else
			{
				//если в наборе еще есть ломаные, закончить цикл
				one_in_group = false;
			}
		}
	}

	//если в наборе не осталось ломаных, удалить набор
	if (one_in_group) Del_Group();
}

void MoveUp()
{
	int j, k;
	int jj, l;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		if (v[j].group == current_group)
		{
			l = v[j].coord.size();
			for (jj = 0; jj < l; jj++)
				v[j].coord[jj].y += 3;
		}
	}
}

void MoveUpR()
{
	int j, k;
	int jj, l;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		if (v[j].group == current_group)
		{
			l = v[j].coord.size();
			for (jj = 0; jj < l; jj++)
			{
				v[j].coord[jj].y += 3;
				v[j].coord[jj].x += 3;
			}
		}
	}
}

void MoveUpL()
{
	int j, k;
	int jj, l;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		if (v[j].group == current_group)
		{
			l = v[j].coord.size();
			for (jj = 0; jj < l; jj++)
			{
				v[j].coord[jj].y += 3;
				v[j].coord[jj].x -= 3;
			}
		}
	}
}

void MoveDown()
{
	int j, k;
	int jj, l;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		if (v[j].group == current_group)
		{
			l = v[j].coord.size();
			for (jj = 0; jj < l; jj++)
				v[j].coord[jj].y -= 3;
		}
	}
}

void MoveDownR()
{
	int j, k;
	int jj, l;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		if (v[j].group == current_group)
		{
			l = v[j].coord.size();
			for (jj = 0; jj < l; jj++)
			{
				v[j].coord[jj].y -= 3;
				v[j].coord[jj].x += 3;
			}
		}
	}
}

void MoveDownL()
{
	int j, k;
	int jj, l;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		if (v[j].group == current_group)
		{
			l = v[j].coord.size();
			for (jj = 0; jj < l; jj++)
			{
				v[j].coord[jj].y -= 3;
				v[j].coord[jj].x -= 3;
			}
		}
	}
}

void MoveLeft()
{
	int j, k;
	int jj, l;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		if (v[j].group == current_group)
		{
			l = v[j].coord.size();
			for (jj = 0; jj < l; jj++)
				v[j].coord[jj].x -= 3;
		}
	}
}

void MoveRight()
{
	int j, k;
	int jj, l;
	k = v.size();
	for (j = 0; j < k; j++)
	{
		if (v[j].group == current_group)
		{
			l = v[j].coord.size();
			for (jj = 0; jj < l; jj++)
				v[j].coord[jj].x += 3;
		}
	}
}

void Color(unsigned char key)//обработчик подменю цветов
{
	if (key == 'R')
	{
		if (colours[current_group].red < 256)
			colours[current_group].red += 1;
		else
			colours[current_group].red = 0;
	}
	
	if (key == 'G')
	{
		if (colours[current_group].green < 256)
			colours[current_group].green += 1;
		else
			colours[current_group].green = 0;
	}

	if (key == 'B')
	{
		if (colours[current_group].blue < 256)
			colours[current_group].blue += 1;
		else
			colours[current_group].blue = 0;
	}
	
	if (key == 'r')
	{
		if (colours[current_group].red > 0)
			colours[current_group].red -= 1;
		else
			colours[current_group].red = 255;
	}

	if (key == 'g')
	{
		if (colours[current_group].green > 0)
			colours[current_group].green -= 1;
		else
			colours[current_group].green = 255;
	}

	if (key == 'b')
	{
		if (colours[current_group].blue > 0)
			colours[current_group].blue -= 1;
		else
			colours[current_group].blue = 255;
	}
}


void param(int opt)
{
	//обработка типа линий
	if (opt == SOLID)
		lineTypeVec[current_group].type = 0xFFFF;
	if (opt == DOT)
		lineTypeVec[current_group].type = 0x0101;
	if (opt == DASH)
		lineTypeVec[current_group].type = 0x00F0;
	if (opt == DOTDASH)
		lineTypeVec[current_group].type = 0x1C47;
	//обработка толщины линий
	if (opt == WIDTH_0_5)
		lineTypeVec[current_group].widht = 0.5;
	if (opt == WIDTH_1)
		lineTypeVec[current_group].widht = 1.0;
	if (opt == WIDTH_1_5)
		lineTypeVec[current_group].widht = 1.5;
	if (opt == WIDTH_2)
		lineTypeVec[current_group].widht = 2.0;
	if (opt == WIDTH_2_5)
		lineTypeVec[current_group].widht = 2.5;
	if (opt == WIDTH_3)
		lineTypeVec[current_group].widht = 3.0;
	if (opt == WIDTH_3_5)
		lineTypeVec[current_group].widht = 3.5;
	if (opt == WIDTH_4)
		lineTypeVec[current_group].widht = 4.0;

	if (opt == RED)
	{
		colours[current_group].red = 252;
		colours[current_group].green = 48;
		colours[current_group].blue = 58;
		int k, j;
		//добавить ломаную в вектор ломаных
		v.push_back(l);
		//"выйти" из режима рисования
		drawing_mode = false;

		//удалить данные о текущей ломаной
		k = l.coord.size();
		for (j = 0; j < k; j++)
		{
			l.coord.pop_back();
		}
	}
	if (opt == GREEN)
	{
		colours[current_group].red = 105;
		colours[current_group].green = 197;
		colours[current_group].blue = 22;
		int k, j;
		//добавить ломаную в вектор ломаных
		v.push_back(l);
		//"выйти" из режима рисования
		drawing_mode = false;

		//удалить данные о текущей ломаной
		k = l.coord.size();
		for (j = 0; j < k; j++)
		{
			l.coord.pop_back();
		}
	}
	if (opt == BLUE)
	{
		colours[current_group].red = 42;
		colours[current_group].green = 128;
		colours[current_group].blue = 245;
		int k, j;
		//добавить ломаную в вектор ломаных
		v.push_back(l);
		//"выйти" из режима рисования
		drawing_mode = false;

		//удалить данные о текущей ломаной
		k = l.coord.size();
		for (j = 0; j < k; j++)
		{
			l.coord.pop_back();
		}
	}
	if (opt == ORANGE)
	{
		colours[current_group].red = 246;
		colours[current_group].green = 138;
		colours[current_group].blue = 42;
		int k, j;
		//добавить ломаную в вектор ломаных
		v.push_back(l);
		//"выйти" из режима рисования
		drawing_mode = false;

		//удалить данные о текущей ломаной
		k = l.coord.size();
		for (j = 0; j < k; j++)
		{
			l.coord.pop_back();
		}
	}
	if (opt == YELLOW)
	{
		colours[current_group].red = 246;
		colours[current_group].green = 224;
		colours[current_group].blue = 42;
		int k, j;
		//добавить ломаную в вектор ломаных
		v.push_back(l);
		//"выйти" из режима рисования
		drawing_mode = false;

		//удалить данные о текущей ломаной
		k = l.coord.size();
		for (j = 0; j < k; j++)
		{
			l.coord.pop_back();
		}
	}
	if (opt == VIOLET)
	{
		colours[current_group].red = 182;
		colours[current_group].green = 97;
		colours[current_group].blue = 183;
		int k, j;
		//добавить ломаную в вектор ломаных
		v.push_back(l);
		//"выйти" из режима рисования
		drawing_mode = false;

		//удалить данные о текущей ломаной
		k = l.coord.size();
		for (j = 0; j < k; j++)
		{
			l.coord.pop_back();
		}
	}
	if (opt == BLACK)
	{
		colours[current_group].red = 0;
		colours[current_group].green = 0;
		colours[current_group].blue = 0;
		int k, j;
		//добавить ломаную в вектор ломаных
		v.push_back(l);
		//"выйти" из режима рисования
		drawing_mode = false;

		//удалить данные о текущей ломаной
		k = l.coord.size();
		for (j = 0; j < k; j++)
		{
			l.coord.pop_back();
		}
	}
	if (opt == WHITE)
	{
		colours[current_group].red = 255;
		colours[current_group].green = 255;
		colours[current_group].blue = 255;
		int k, j;
		//добавить ломаную в вектор ломаных
		v.push_back(l);
		//"выйти" из режима рисования
		drawing_mode = false;

		//удалить данные о текущей ломаной
		k = l.coord.size();
		for (j = 0; j < k; j++)
		{
			l.coord.pop_back();
		}
	}
	glutPostRedisplay();
}

void Menu(int opt)
{
	int main_menu;
	int width_menu;
	int color_menu;
	int type_menu;

	switch (opt)
	{
	case SOLID:
		param(SOLID); break;
	case DOT:
		param(DOT); break;
	case DASH:
		param(DASH); break;
	case DOTDASH:
		param(DOTDASH); break;
	case WIDTH_0_5:
		param(WIDTH_0_5); break;
	case WIDTH_1:
		param(WIDTH_1); break;
	case WIDTH_1_5:
		param(WIDTH_1_5); break;
	case WIDTH_2:
		param(WIDTH_2); break;
	case WIDTH_2_5:
		param(WIDTH_2_5); break;
	case WIDTH_3:
		param(WIDTH_3); break;
	case WIDTH_3_5:
		param(WIDTH_3_5); break;
	case WIDTH_4:
		param(WIDTH_4); break;
	case RED:
		param(RED); break;
	case GREEN:
		param(GREEN); break;
	case BLUE:
		param(BLUE); break;
	case ORANGE:
		param(ORANGE); break;
	case YELLOW:
		param(YELLOW); break;
	case VIOLET:
		param(VIOLET); break;
	case BLACK:
		param(BLACK); break;
	case WHITE:
		param(WHITE); break;

	default:
		//подменю тип линии
		type_menu = glutCreateMenu(Menu);
		glutAddMenuEntry("Solid", SOLID);//добавить пункты подменю
		glutAddMenuEntry("Dot", DOT);
		glutAddMenuEntry("Dash", DASH);
		glutAddMenuEntry("Dotdash", DOTDASH);

		//подменю толщина линии
		width_menu = glutCreateMenu(Menu);
		glutAddMenuEntry("0.5", WIDTH_0_5);//добавить пункты подменю
		glutAddMenuEntry("1.0", WIDTH_1);
		glutAddMenuEntry("1.5", WIDTH_1_5);
		glutAddMenuEntry("2.0", WIDTH_2);
		glutAddMenuEntry("2.5", WIDTH_2_5);
		glutAddMenuEntry("3.0", WIDTH_3);
		glutAddMenuEntry("3.5", WIDTH_3_5);
		glutAddMenuEntry("4.0", WIDTH_4);

		//подменю цвет
		color_menu = glutCreateMenu(Menu);
		glutAddMenuEntry("Red", RED);//добавить пункты подменю
		glutAddMenuEntry("Blue", BLUE);
		glutAddMenuEntry("Green", GREEN);
		glutAddMenuEntry("Orange", ORANGE);
		glutAddMenuEntry("Yellow", YELLOW);
		glutAddMenuEntry("Violet", VIOLET);
		glutAddMenuEntry("Black", BLACK);
		glutAddMenuEntry("White", WHITE);

		//главное меню
		main_menu = glutCreateMenu(Menu);
		glutAddSubMenu("Line type", type_menu);//добавить подменю
		glutAddSubMenu("Line thickness", width_menu);
		glutAddSubMenu("Line color", color_menu);
		glutAttachMenu(GLUT_RIGHT_BUTTON);//прикрепить меню к правой кнопке мыши

	}
}

void Mouse(int button, int state, int x, int y)
{
	char str[50];
	if (state != GLUT_DOWN) return;		//если кнопка мыши не была отпущена

	if (button == GLUT_LEFT_BUTTON)
	{
		if (!drawing_mode)
		{
			//включить режим отрисовки
			drawing_mode = true;
			//новая ломаная принадлежит текущему набору
			l.group = current_group;
			sprintf(str, "%i groups", max_group + 1);
			glutSetWindowTitle(str);
		}

		//получить координаты точки
		point p = point((GLint)x, height - (GLint)y);
		//добавить их в ломаную
		l.coord.push_back(p);
		//указать, что набор не пуст
		prev_space = false;
	}

	glutPostRedisplay();
}

void Disp(void)
{
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT);	//установить цветовой режим буфера
	glEnable(GL_LINE_SMOOTH);		//установить режим сглаживания
	glEnable(GL_LINE_STIPPLE);		//наложение маски для создания штриховых линий

	for (i = 0; i < v.size(); i++)
	{
		if (v[i].group == current_group)//выделить ломаные текущего набора большей толщиной
		{
			glLineWidth(5.0);
		}
		else
		{
			glLineWidth(lineTypeVec[v[i].group].widht);
		}
		glLineStipple(3, lineTypeVec[v[i].group].type); //задать тип маски

		glBegin(GL_LINE_LOOP);//тип примитива - замкнутая ломаная 
		for (j = 0; j < v[i].coord.size(); j++)
		{
			//установить цвет набора
			glColor3ub(colours[v[i].group].red, colours[v[i].group].green, colours[v[i].group].blue);
			//отрисовать ломаную
			glVertex2i(v[i].coord[j].x, v[i].coord[j].y);
		}
		glEnd();
	}
	glDisable(GL_LINE_STIPPLE);	//отключить маску

	//если отрисовывается текущая ломаная
	if (drawing_mode)
	{
		//выделить ее большей толщиной
		glLineWidth(5.0);

		//установить цвет набора и отрисовать ломаную
		glColor3ub(colours[l.group].red, colours[l.group].green, colours[l.group].blue);
		glBegin(GL_LINE_LOOP);
		for (j = 0; j < l.coord.size(); j++)
		{
			glVertex2i(l.coord[j].x, l.coord[j].y);
		}
		glEnd();
	}

	glutSwapBuffers();
}

void ASCII_Keys(unsigned char key, int x, int y)//обработка клавиш
{
	if (!drawing_mode)
	{
		if (key == 32)		//пробел
		{
			New_Grup();
		}

		if (key == 127)		//delete
		{
			Del_Group();
		}

		if (key == 8)		//backspace
		{
			Delete_Last_Prim();
		}
		//обработка движения
		if (key == 'w' || key == 'W')
		{
			MoveUp();
		}
		
		if (key == 's' || key == 'S')
		{
			MoveDown();
		}

		if (key == 'a' || key == 'A')
		{
			MoveLeft();
		}

		if (key == 'd' || key == 'D')
		{
			MoveRight();
		}

		if (key == 'e' || key == 'E')
		{
			MoveUpR();
		}

		if (key == 'q' || key == 'Q')
		{
			MoveUpL();
		}

		if (key == 'c' || key == 'C')
		{
			MoveDownR();
		}

		if (key == 'z' || key == 'Z')
		{
			MoveDownL();
		}
		//обработка изменения цветов (+/-1)
		if (key == 'r')
		{
			Color('r');
		}
		if (key == 'g')
		{
			Color('g');
		}
		if (key == 'b')
		{
			Color('b');
		}

		if (key == 'R')
		{
			Color('R');
		}
		if (key == 'G')
		{
			Color('G');
		}
		if (key == 'B')
		{
			Color('B');
		}
	}
	//обработка изменения цветов (+/-10)
	if (key == 'j' || key == 'J')
	{
		Minus_Red();
	}

	if (key == 'k' || key == 'K')
	{
		Minus_Green();
	}

	if (key == 'l' || key == 'L')
	{
		Minus_Blue();
	}

	if (key == 'u' || key == 'U')
	{
		Plus_Red();
	}

	if (key == 'i' || key == 'I')
	{
		Plus_Green();
	}

	if (key == 'o' || key == 'O')
	{
		Plus_Blue();
	}
	if (key == 'x' || key == 'X')
	{
		Save_Prim();
	}
	char v[50]; sprintf(v, "Текущий цвет всех точек: R=%.3d G=%.3d B=%.3d", colours[current_group].red, colours[current_group].green, colours[current_group].blue);
	glutSetWindowTitle(v);
	glutPostRedisplay();
}
void NASCII_Keys(int key, int x, int y)
{
	if (!drawing_mode)
	{
		if (key == GLUT_KEY_UP)
		{
			Up_Grup();
		}

		if (key == GLUT_KEY_DOWN)
		{
			Down_Grup();
		}
	}

	glutPostRedisplay();
}

void Init()
{
	//установить белый цвет экрана
	glClearColor(1.0, 1.0, 1.0, 1);
	//установить проекцию
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0.0, width, 0.0, height);

	//установить начальные значения
	drawing_mode = false;
	max_group = 0;
	current_group = 0;
	prev_space = false;

	//добавить "цвет по умолчанию" для нулевого набора
	colours.push_back(colour(0, 0, 0, current_group));
	//добавить "тип линии и толщину по умолчанию" для нулевого набора
	lineTypeVec.push_back(linetype(1.0, 0xFFFF, current_group));
}

void Resh(GLint w, GLint h)
{
	//изменить размеры окна
	width = w;
	height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0.0, width, 0.0, height);
}
