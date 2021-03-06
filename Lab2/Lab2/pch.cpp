#include "pch.h"

void Change_Mode()//включение выключение режима редактирования точек
{
	char str[50];
	if (dot_mode)//если режим редактирования точек был включен
	{
		sprintf(str, "dot mod OFF");
		glutSetWindowTitle(str);
		dot_mode = false;			//выключить режим редактирования точек
		glutSetMenu(main_menu1);	//изменить идентификатор меню
		glutAttachMenu(GLUT_RIGHT_BUTTON);//назначить меню на ПКМ
	}
	else
	{
		current_dot = 0;
		sprintf(str, "dot mod ON");
		glutSetWindowTitle(str);
		dot_mode = true;			//включить режим редактирования точек
		glutSetMenu(main_menu2);	//изменить идентификатор меню
		glutAttachMenu(GLUT_RIGHT_BUTTON);//назначить меню на ПКМ
	}
}
void On_Up_Key()//передвижение по всем шестиугольникам, либо по всем точкам в режиме редактирования точек
{
	char str[50];
	if (!dot_mode)	//если не режим редактирования точек
	{
		//перейти к следующему шестиугольнику
		current_figure++;
		//перейти к нулевому, если был достигнут последний добавленный шестиугольник
		current_figure = current_figure % (max_figure);

		sprintf(str, "Current figure: %i", current_figure + 1);
		glutSetWindowTitle(str);
	}
	else
	{
		current_dot++;					//перейти к следующей точке
		current_dot = current_dot % N;	//перейти к нулевой точке, если была достигнута последняя точка в шестиугольнике

		sprintf(str, "Current dot: %i", current_dot + 1);
		glutSetWindowTitle(str);
	}
}
void On_Down_Key()//передвижение по всем шестиугольникам, либо по всем точкам в режиме редактирования точек
{
	char str[50];
	if (!dot_mode)
	{
		//перейти к предыдущему шестиугольнику
		current_figure--;
		//перейти к последнему добавленному шестиугольнику, если был пройден нулевой шестиугольник
		if (current_figure < 0) current_figure = max_figure - 1;

		sprintf(str, "Current figure: %i", current_figure + 1);
		glutSetWindowTitle(str);
	}
	else
	{
		current_dot--;						//перейти к предыдущей точке
		if (current_dot < 0) current_dot = 5;	//перейти к последней точке, если была достигнута нулевая точка в шестиугольнике

		sprintf(str, "Current dot: %i", current_dot + 1);
		glutSetWindowTitle(str);
	}

}

void Del_Group()
{
	char str[50];
	v.erase(v.begin() + current_figure);
	max_figure--;
	sprintf(str, "%i groups", max_figure + 1);
	glutSetWindowTitle(str);
}

void Clear()
{
	char str[50];
	v.clear();
	max_figure = 0;
	sprintf(str, "%i groups", max_figure + 1);
	glutSetWindowTitle(str);
}

void MoveUp()
{
	if (v[current_figure].Oy + v[current_figure].ScaleOy <= height)
		v[current_figure].Oy += 10;
}
void MoveDown()
{
	if (v[current_figure].Oy - v[current_figure].ScaleOy >= 0)
		v[current_figure].Oy -= 10;
}
void MoveLeft()
{
	if (v[current_figure].Ox - v[current_figure].ScaleOx >= 0)
		v[current_figure].Ox -= 10;
}
void MoveRight()
{
	if (v[current_figure].Ox + v[current_figure].ScaleOx <= width)
		v[current_figure].Ox += 10;
}
void Increase(double zoom)
{
	v[current_figure].ScaleOx += zoom;
	v[current_figure].ScaleOy += zoom;
}
void Reduce(double zoom)
{
	if (v[current_figure].ScaleOx - zoom > 0 && v[current_figure].ScaleOy - zoom > 0)
	{
		v[current_figure].ScaleOx -= zoom;
		v[current_figure].ScaleOy -= zoom;
	}
}
void RotateLeft()
{
	v[current_figure].Angle += 5;
	if (v[current_figure].Angle > 360)
		v[current_figure].Angle %= 360;
}
void RotateRight()
{
	v[current_figure].Angle -= 5;
	if (v[current_figure].Angle < 360)
		v[current_figure].Angle %= 360;
}

bool equality(point one, point two)//проверка равенства двух точек
{
	if (one.x == two.x && one.y == two.y)
		return true;
	return false;
}
void Finish_Primitive()
{
	char str[50];

	//отключить режим рисования
	drawing_mode = false;

	//если координаты центра и вершины не совпадают
	if (!equality(point(cen_ver.center_x, cen_ver.center_y), point(cen_ver.vertex_x, cen_ver.vertex_y)))
	{
		current_figure = max_figure;

		//установка атрибутов фигуры
		drawing_polygon.number = current_figure;

		//сдвиг фигуры от начала координат равен центру фигуры
		drawing_polygon.Ox = cen_ver.center_x;
		drawing_polygon.Oy = cen_ver.center_y;

		//масштаб = не масштабирована
		drawing_polygon.ScaleOx = 1;
		drawing_polygon.ScaleOy = 1;

		//угол поворота = 0
		drawing_polygon.Angle = 0;

		//текстура отсутствует
		drawing_polygon.texture_number = -1;
		//тип наложения текстуры
		drawing_polygon.overlay_type_tex = 1;

		for (int i = 0; i < drawing_v.size(); i++)
		{
			//поместить текущий цвет вершин в отрисовываемый шестиугольник
			drawing_polygon.color_point.push_back(drawing_color);

			//поместить текущие вершины в отрисовываемый шестиугольник
			drawing_polygon.coord_polygon.push_back(drawing_v[i]);
		}
		v.push_back(drawing_polygon);//поместить отрисовываемый шестиугольник в вектор

		//очистка векторов
		drawing_v.erase(drawing_v.begin(), drawing_v.end());
		drawing_polygon.color_point.erase(drawing_polygon.color_point.begin(), drawing_polygon.color_point.end());
		drawing_polygon.coord_polygon.erase(drawing_polygon.coord_polygon.begin(), drawing_polygon.coord_polygon.end());

		sprintf(str, "Current figure: %i", current_figure + 1);
		glutSetWindowTitle(str);

		max_figure++;
	}
	else //если ткнули два раза в одну точку, добавлять нечего
		current_figure = max_figure - 1;	//вернуться к предыдущему шестиугольнику

}

void calc_polygon_coord(point center, point vertex)//вычисление координат шестиугольника
{
	int n = N;							//число вершин
	double radius;						//радиус описанной окружности
	double A0;							//угловая координата первой вершины
	double centr_x = double(center.x);
	double centr_y = double(center.y);
	double vert_x = double(vertex.x);
	double vert_y = double(vertex.y);
	double cur_x, cur_y;

	drawing_v.erase(drawing_v.begin(), drawing_v.end());	//очистить вектор

	A0 = atan((vert_x - centr_x) / (vert_y - centr_y));
	if (vert_y < centr_y)
		A0 += M_PI;

	radius = sqrt(pow((vert_x - centr_x), 2.0) + pow((vert_y - centr_y), 2.0));

	for (int i = 0; i < n; i++)
	{
		//координаты рассчитываются от начала координатной оси (0,0)
		cur_x = radius * sin(A0 + 2 * M_PI*i / n);
		cur_y = radius * cos(A0 + 2 * M_PI*i / n);

		point p = point(cur_x, cur_y);
		drawing_v.push_back(p);
	}
}
void set_texture_coord()//расчет текстурных координат
{
	double A = M_PI / N;
	pointf coord = pointf(0.5, 1);
	texture_coord.push_back(coord);
	coord = pointf(0.5 + 0.5*cos(A), 0.75);
	texture_coord.push_back(coord);
	coord = pointf(0.5 + 0.5*cos(A), 0.25);
	texture_coord.push_back(coord);
	coord = pointf(0.5, 0);
	texture_coord.push_back(coord);
	coord = pointf(0.5 - 0.5*cos(A), 0.25);
	texture_coord.push_back(coord);
	coord = pointf(0.5 - 0.5*cos(A), 0.75);
	texture_coord.push_back(coord);
	coord = pointf(0.5, 1);
	texture_coord.push_back(coord);
	coord = pointf(0.5 - 0.5*cos(A), 0.75);
	texture_coord.push_back(coord);
}

void processColorMenu(int option)//обработчик подменю цветов mainmenu1
{
	if (!drawing_mode && max_figure != 0)
	{
		colour color = colour(0, 0, 0);
		switch (option)
		{
		case 1:
			color.red = 255;
			color.green = 0;
			color.blue = 0;
			for (int i = 0; i < v[current_figure].coord_polygon.size(); i++)
				v[current_figure].color_point[i] = color;
			break;
		case 3:
			color.red = 10;
			color.green = 240;
			color.blue = 10;
			for (int i = 0; i < v[current_figure].coord_polygon.size(); i++)
				v[current_figure].color_point[i] = color;
			break;
		case 2:
			color.red = 0;
			color.green = 0;
			color.blue = 255;
			for (int i = 0; i < v[current_figure].coord_polygon.size(); i++)
				v[current_figure].color_point[i] = color;
			break;
		case 4:
			color.red = 246;
			color.green = 138;
			color.blue = 42;
			for (int i = 0; i < v[current_figure].coord_polygon.size(); i++)
				v[current_figure].color_point[i] = color;
			break;
		case 5:
			color.red = 246;
			color.green = 224;
			color.blue = 42;
			for (int i = 0; i < v[current_figure].coord_polygon.size(); i++)
				v[current_figure].color_point[i] = color;
			break;
		case 6:
			color.red = 182;
			color.green = 97;
			color.blue = 183;
			for (int i = 0; i < v[current_figure].coord_polygon.size(); i++)
				v[current_figure].color_point[i] = color;
			break;
		case 7:
			color.red = 0;
			color.green = 0;
			color.blue = 0;
			for (int i = 0; i < v[current_figure].coord_polygon.size(); i++)
				v[current_figure].color_point[i] = color;
			break;
		case 8:
			color.red = 255;
			color.green = 255;
			color.blue = 255;
			for (int i = 0; i < v[current_figure].coord_polygon.size(); i++)
				v[current_figure].color_point[i] = color;
			break;
		}
		glutPostRedisplay();
	}
}
void processColorMenu2(int option)//обработчик подменю цветов mainmenu2
{
	if (!drawing_mode && max_figure != 0)
	{
		colour color = colour(0, 0, 0);
		switch (option)
		{
		case 1:
			color.red = 252;
			color.green = 48;
			color.blue = 58;
			v[current_figure].color_point[current_dot] = color;
			break;
		case 3:
			color.red = 105;
			color.green = 197;
			color.blue = 22;
			v[current_figure].color_point[current_dot] = color;
			break;
		case 2:
			color.red = 42;
			color.green = 128;
			color.blue = 245;
			v[current_figure].color_point[current_dot] = color;
			break;
		case 4:
			color.red = 246;
			color.green = 138;
			color.blue = 42;
			v[current_figure].color_point[current_dot] = color;
			break;
		case 5:
			color.red = 246;
			color.green = 224;
			color.blue = 42;
			v[current_figure].color_point[current_dot] = color;
			break;
		case 6:
			color.red = 182;
			color.green = 97;
			color.blue = 183;
			v[current_figure].color_point[current_dot] = color;
			break;
		case 7:
			color.red = 0;
			color.green = 0;
			color.blue = 0;
			v[current_figure].color_point[current_dot] = color;
			break;
		case 8:
			color.red = 255;
			color.green = 255;
			color.blue = 255;
			v[current_figure].color_point[current_dot] = color;
			break;
		}
		glutPostRedisplay();
	}
}
void processScaleMenu(int option)
{
	if (!drawing_mode && max_figure != 0)
	{
		switch (option)
		{
		case 1:
			Increase(0.3);
			break;
		case 2:
			Reduce(0.3);
			break;
		}
		glutPostRedisplay();
	}
}
void processTextureMenuMain(int option)
{
	//пусто
	//
}
void processTextureMenu(int option)
{
	if (!drawing_mode && max_figure != 0)
	{
		switch (option)
		{
		case 1:
			v[current_figure].texture_number = 0;
			break;
		case 2:
			v[current_figure].texture_number = 1;
			break;
		case 3:
			v[current_figure].texture_number = 2;
			break;
		case 4:
			v[current_figure].texture_number = 3;
			break;
		case 5:
			v[current_figure].texture_number = 4;
			break;
		case 6:
			v[current_figure].texture_number = 5;
			break;
		case 7:
			v[current_figure].texture_number = 6;
			break;
		case 8:
			v[current_figure].texture_number = 7;
			break;
		case 9:
			v[current_figure].texture_number = -1;
			v[current_figure].overlay_type_tex = 1;
			break;
		}
		glutPostRedisplay();
	}
}
void processOverlayMenu(int option)
{
	if (!drawing_mode && max_figure != 0)
	{
		switch (option)
		{
		case 1:
			v[current_figure].overlay_type_tex = 1;
			break;
		case 2:
			v[current_figure].overlay_type_tex = 2;
			break;
		}
		glutPostRedisplay();
	}
}
void processLogicMenu(int option)
{
	char str[50];
	if (!drawing_mode && !dot_mode && max_figure != 0)
	{
		colour_shuffle_mode = option;
		glutPostRedisplay();
		sprintf(str, "Current logic operation: %s", colour_shuffle_mode ? (colour_shuffle_mode == 1 ? "AND" : "NAND") : "none");
		glutSetWindowTitle(str);
	}
}
void processRasterMenu(int option)
{
}
void processRasterModeMenu(int option)
{
	char str[50];

	if (!drawing_mode && !dot_mode)
	{
		raster_mode = option;
		glutPostRedisplay();
		sprintf(str, "Current rasterization: %s", raster_mode ? (raster_mode == 1 ? "contour" : "fill") : "turned off");
		glutSetWindowTitle(str);
	}
}
void processNetMenu(int option)
{
	char str[50];

	if (!drawing_mode && !dot_mode && raster_mode)
	{
		netsize = option;
		glutPostRedisplay();
		sprintf(str, "Current net size: %i", netsize);
		glutSetWindowTitle(str);
	}
}

void processMainMenu(int option)
{
	//пусто
	//все основные действия в подменю
};
void createMenu()
{
	int color_menu1;
	int color_menu2;
	int scale_menu;
	int texture_menu_main;
	int texture_menu;
	int overlay_menu;
	int logic_menu;
	int raster_menu;
	int raster_mode_menu;
	int raster_size_menu;

	//подменю цвет
	color_menu1 = glutCreateMenu(processColorMenu);
	glutAddMenuEntry("Red", 1);//добавить пункты подменю
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Orange", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Violet", 6);
	glutAddMenuEntry("Black", 7);
	glutAddMenuEntry("White", 8);

	//подменю цвет для работы с точками
	color_menu2 = glutCreateMenu(processColorMenu2);
	glutAddMenuEntry("Red", 1);//добавить пункты подменю
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Orange", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Violet", 6);
	glutAddMenuEntry("Black", 7);
	glutAddMenuEntry("White", 8);

	//подменю масштаб
	scale_menu = glutCreateMenu(processScaleMenu);
	glutAddMenuEntry("Zoom in", 1);//добавить пункты подменю
	glutAddMenuEntry("Zoom out", 2);

	//подподменю типа текстуры
	texture_menu = glutCreateMenu(processTextureMenu);
	glutAddMenuEntry("Texture 1", 1);//добавить пункты подменю
	glutAddMenuEntry("Texture 2", 2);
	glutAddMenuEntry("Texture 3", 3);
	glutAddMenuEntry("Texture 4", 4);
	glutAddMenuEntry("Texture 5", 5);
	glutAddMenuEntry("Texture 6", 6);
	glutAddMenuEntry("Texture 7", 7);
	glutAddMenuEntry("Texture 8", 8);
	glutAddMenuEntry("Delete texture", 9);

	//подподменю типа наложения текстуры
	overlay_menu = glutCreateMenu(processOverlayMenu);
	glutAddMenuEntry("Multiplication", 1);//добавить пункты подменю
	glutAddMenuEntry("Firm texture", 2);

	//подменю текстуры
	texture_menu_main = glutCreateMenu(processTextureMenuMain);
	glutAddSubMenu("Type texture", texture_menu);
	glutAddSubMenu("Type overlay", overlay_menu);

	//главное меню в режиме редактирования точек
	main_menu2 = glutCreateMenu(processMainMenu);
	glutAddSubMenu("Color", color_menu2);

	//подменю логических операций
	logic_menu = glutCreateMenu(processLogicMenu);
	glutAddMenuEntry("none", 0);
	glutAddMenuEntry("AND", 1);
	glutAddMenuEntry("NAND", 2);

	//подподменю типа режима растеризации
	raster_mode_menu = glutCreateMenu(processRasterModeMenu);
	glutAddMenuEntry("Turn off", 0);
	glutAddMenuEntry("Contour", 1);
	glutAddMenuEntry("Fill", 2);

	//подподменю типа размера сетки
	raster_size_menu = glutCreateMenu(processNetMenu);
	glutAddMenuEntry("6", 6);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("14", 14);
	glutAddMenuEntry("18", 18);
	glutAddMenuEntry("20", 22);

	//подменю растеризации
	raster_menu = glutCreateMenu(processRasterMenu);
	glutAddSubMenu("Rasterization mode", raster_mode_menu);
	glutAddSubMenu("Net size", raster_size_menu);

	//главное меню в режиме редактирования шестиугольников
	main_menu1 = glutCreateMenu(processMainMenu);
	glutAddSubMenu("Color", color_menu1);
	glutAddSubMenu("Scale", scale_menu);
	glutAddSubMenu("Texture", texture_menu_main);
	glutAddSubMenu("Logic operations", logic_menu);
	glutAddSubMenu("Rasterization", raster_menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);//прикрепить меню к правой кнопке мыши
}

//unsigned int names_tex[5]; //имена текстур 
void loadTexture()
{
	FILE *F;
	char str[20] = "1.bmp";
	char num = '1';
	auto widthT = width;
	auto heightT = height;
	glGenTextures(8, names_tex); //создание имен текстур 
	for (int i = 0; i < 8; i++)
	{
		str[0] = num;
		num++;
		if ((F = fopen(str, "rb")) != NULL)
		{
			/*Перемещаемся в bmp-файле на нужную позицию, и считываем ширину и длинну */
			fseek(F, 18, SEEK_SET);
			fread(&(widthT), 2, 1, F);
			fseek(F, 2, SEEK_CUR);
			fread(&(heightT), 2, 1, F);

			/* Выделяем память под изображение. Если память не выделилась, закрываем файл и выходим с ошибкой */
			auto pixels = new GLubyte[3 * widthT * heightT];

			/* Считываем изображение в память по 3 бита, то бишь RGB для каждого пикселя */
			fseek(F, 30, SEEK_CUR);
			fread(pixels, 3, widthT * heightT, F);

			glBindTexture(GL_TEXTURE_2D, names_tex[i]); //выбрать текущую текстуру 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, widthT, heightT, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

			free(pixels);
			fclose(F);
		}
	}
}

void SwitchColourShuffleMode()
{
	char str[50];
	colour_shuffle_mode = (colour_shuffle_mode + 1) % 3;
	sprintf(str, "Current logic operation: %s", colour_shuffle_mode ? (colour_shuffle_mode == 1 ? "AND" : "NAND") : "none");
	glutSetWindowTitle(str);
}
void SwitchRasterMode()
{
	char str[50];
	raster_mode = (raster_mode + 1) % 4;
	sprintf(str, "Current rasterization: %i", raster_mode);
	glutSetWindowTitle(str);
}
void SwitchNetSize()
{
	char str[50];
	netsize = netsize + 2;
	if (netsize > 9) netsize = 4;
	sprintf(str, "Current net size: %i", netsize);
	glutSetWindowTitle(str);
}

//сеткель - элемент искусственной растровой сетки, для простоты дальнейших комментариев
bool point_in_net(int x0, int y0, int * xi, int * yj)	//координаты искомой точки, координаты сеткеля
{
	bool found = false;
	int fal_width, fal_height;
	int i, j;
	fal_width = 2.2 * width;		//точки шестиугольников могут уходить за видимые пределы окна
	fal_height = 2.2 * height;		//поэтому поиск точки происходит в значительно бо'льших пределах
	for (i = -fal_width / netsize - 1; i < fal_width / netsize + 1 && !found; i++)
	{
		for (j = -fal_height / netsize - 1; j < fal_height / netsize + 1 && !found; j++)
		{
			if (x0 >= i * netsize && x0 < (i + 1)*netsize)
			{
				if (y0 >= j * netsize && y0 < (j + 1)*netsize)
				{
					found = true;
				}
			}
		}
	}
	*xi = i;
	*yj = j;
	return (found);
}

void quater_net(int x, int y, int i, int j, int * xn, int * yn)
{
	switch (netsize)
	{
	case 4:
		*xn = i * 4 - 2;
		*yn = j * 4 - 2;
		break;
	case 6:
		if (x > i * 6 - 3)
			*xn = i * 6 - 2;
		else
			if (x < i * 6 - 3)
				*xn = i * 6 - 4;

		if (y > j * 6 - 3)
			*yn = j * 6 - 2;
		else
			if (y < j * 6 - 3)
				*yn = j * 6 - 4;
		break;
	case 8:
		if (x > i * 8 - 4)
			*xn = i * 6 - 3;
		else
			*xn = i * 6 - 5;

		if (y > j * 8 - 4)
			*yn = j * 6 - 3;
		else
			*yn = j * 6 - 5;
		break;
	case 10:
		if (x > i * 10 - 5)
			*xn = i * 10 - 4;
		else
			*xn = i * 10 - 6;

		if (y > j * 10 - 5)
			*yn = j * 10 - 4;
		else
			*yn = j * 10 - 6;
		break;
	}
}

int contour_Bresenham(int x_0, int y_0, int x_k, int y_k, vector <pair<int, int>> * v, int cx, int cy)
{
	int octant, i, j;
	int K;
	bool found = false;
	double coef_angle;			//угловой коэффициент
	int temp;
	int x0, y0, xk, yk;			//переписать координаты отрезка в локальные переменные, страховки ради
	x0 = x_0;
	y0 = y_0;
	xk = x_k;
	yk = y_k;
	coef_angle = (double)(yk - y0) / (double)(xk - x0);		//угловой коэффициент кривой
	if (xk > x0)			//расчет октанта, в котором расположен отрезок, от октанта зависят переменные
	{
		if (yk > y0)
		{
			if (abs(coef_angle) > 1.0L)
			{
				octant = 8;
			}
			else
			{
				octant = 7;
			}
		}
		else
		{
			if (abs(coef_angle) > 1.0L)
			{
				octant = 5;
			}
			else
			{
				octant = 6;
			}
		}
	}
	else
	{
		if (yk > y0)
		{
			if (abs(coef_angle) > 1.0L)
			{
				octant = 1;
			}
			else
			{
				octant = 2;
			}
		}
		else
		{
			if (abs(coef_angle) > 1.0L)
			{
				octant = 4;
			}
			else
			{
				octant = 3;
			}
		}
	}
	found = point_in_net(x0, y0, &i, &j);
	int k = -1;
	if (found)		//определена принадлежность точки начала отрезка сеткелю [i, j]
	{
		double t;
		int dx, dy, sy, sx;
		dx = abs(xk - x0);
		dy = abs(yk - y0);

		if (xk > x0) sx = 1;
		else sx = -1;
		if (yk > y0) sy = 1;
		else sy = -1;

		if (dy > dx)		//одно из направлений меняется на 1 на каждом шаге, поэтому кол-во сеткелей равно кол-ву отрезков ширины сеткеля, которое можно уложить в этом направлении (+1 потому что первая точка (вершина) кладется сразу, без расчетов)
		{
			K = dy / netsize + 1;
		}
		else
		{
			K = dx / netsize + 1;
		}
		int x, y;

		int error2;
		x = x0;
		y = y0;
		int fini, finj;
		k = 0;
		if (xk % netsize < 1)
		{
			if (xk < cx) xk += 1;
			else xk -= 1;
		}
		if (yk % netsize < 1)
		{
			if (yk < cy) yk += 1;
			else yk -= 1;
		}
		if (x0 % netsize < 1)
		{
			if (x0 < cx) x0 += 1;
			else x0 -= 1;
		}
		if (y0 % netsize < 1)
		{
			if (y0 < cy) y0 += 1;
			else y0 -= 1;
		}
		found = point_in_net(xk, yk, &fini, &finj);
		found = point_in_net(x0, y0, &i, &j);
		//if((x % netsize) < 2) x = i * netsize - netsize/2;
		//if((y % netsize) < 2) y = j * netsize - netsize/2;
		//if((xk % netsize) < 2) xk = fini * netsize - netsize/2;
		//if((yk % netsize) < 2) yk = finj * netsize - netsize/2;
		//if(abs(x - i * netsize) < 1)  x = i * netsize - netsize/2;
		//if(abs(y - j * netsize)  < 1) y = j * netsize - netsize/2;
		//if(abs(xk - fini * netsize)  < 1) xk = fini * netsize - netsize/2;
		//if(abs(yk - finj * netsize)  < 1) yk = finj * netsize - netsize/2;
		int x0i, y0j, xki, ykj;
		x0i = x0;
		y0j = y0;
		xki = xk;
		ykj = yk;
		//quater_net(x0, y0, i, j, &x0i, &y0j);
		//quater_net(xk, yk, fini, finj, &xki, &ykj);
		dx = abs(xki - x0i);
		dy = abs(ykj - y0j);
		int error = dx - dy;
		while ((i != fini || j != finj) && k < K)
		{
			v->push_back(make_pair(i, j));
			//printf("%d %d %d\n", i, j, error);
			error2 = error * 2;
			if (error2 > -dy)
			{
				error -= dy;
				i += sx;
			}
			if (error2 < dx)
			{
				error += dx;
				j += sy;
			}
			k++;
		}

		found = point_in_net(xk, yk, &i, &j);
		v->push_back(make_pair(i, j));


	}
	else return 0;

	return (k + 1);
}

bool pred(const pair<int, int> &a, const pair <int, int> &b)
{
	return a.second > b.second;
}

//сортировка по возрастанию внутри группы
bool group(const pair<int, int> &a, const pair <int, int> &b)
{
	if (a.second != b.second) return a.second > b.second;
	else
		return a.first < b.first;
}

int rasterize(const polygon & n, vector <pair<int, int>> * v)
{
	vector <pair<int, int>> nodes;	//узлы в пикселях
	vector <pair<int, int>> w;		//все скидывается сюда, в конце подпрограммы переписывается в *v
	vector <pair<int, int>> nodes_net;	//узлы в квадратах растной сетки
	int i, j;
	int x, y, ymin, ycur;
	int temp;
	int K;	//число добавляемых квадратов растной сетки
	for (i = 0; i < N; i++)	//получили узлы фигуры
	{
		x = n.coord_polygon[i].x + n.Ox;
		y = n.coord_polygon[i].y + n.Oy;
		nodes.push_back(make_pair(x, y));
	}
	sort(nodes.begin(), nodes.end(), pred);	//отсортировали по y

	if (nodes[N - 1].second == nodes[N - 2].second && nodes[N - 1].first < nodes[N - 2].first)	//если нижнее ребро горизонтально, надо убедится, что левый узел действительно левее правого
	{
		temp = nodes[N - 1].first;
		nodes[N - 1].first = nodes[N - 2].first;
		nodes[N - 2].first = temp;
	}

	for (i = 0; i < N; i++)	//создали массив узлов в квадратах растной сетки
	{
		point_in_net(nodes[i].first, nodes[i].second, &x, &y);
		nodes_net.push_back(make_pair(x, y));
	}

	int k;
	k = (nodes_net[0].second - nodes_net[N - 1].second) / netsize;	//кол-во квадратов растной сетки по y (на каждом шаге спускаемся вниз на 1 по y ~ строка)
	k++;
	vector <active_edge> active_edges;
	ycur = nodes_net[0].second;	//начальный y
	ymin = nodes_net[N - 1].second; //конечный y

	w.push_back(make_pair(nodes_net[0].first, nodes_net[0].second));	//первый узел
	do
	{
		for (j = 0; j < nodes.size(); j++)
		{
			if (nodes_net[j].second == ycur)	//если на строке есть узел 
			{
				switch (j)	//в зависимости от номера узла
				{
				case 0:
					if (abs(nodes[1].second - nodes[0].second) < netsize / 2)
					{
						w.push_back(make_pair(nodes_net[1].first, nodes_net[1].second));	//если ребро горизонтально, добавили вторую вершину в массив частей растовой сетки
					}
					else
					{
						//иначе первая строка описывается только одной вершиной
						w.push_back(make_pair(nodes_net[0].first, nodes_net[0].second));
						//создаем активное ребро от 0 до 1 вершины
						//угловой коэффициент
						a.k = (double)(nodes[1].second - nodes[0].second) / (double)(nodes[1].first - nodes[0].first);
						//откуда идем
						a.x = nodes[0].first;
						a.y = nodes[0].second;
						//куда
						a.yk = nodes[1].second;
						a.xk = nodes[1].first;
						//приращение по y - идем вниз, каждый раз добавляя 1 часть растовой сетки
						a.dy = -netsize;
						//приращение по x = приращение по y / угловой коэффициент
						a.dx = (int)((double)a.dy / a.k);
						a.dy = -netsize;
						//если приращение посчиталось больше, чем сама сторона
						//например в случае почти горизонтального ребра
						//уменьшить приращение до размера стороны
						if (abs(a.dx) > abs(a.xk - a.x)) a.dx = a.xk - a.x;
						//на какой строке ребро закончится
						a.yfin = nodes_net[1].second;
						active_edges.push_back(a);
					}
					//создаем активное ребро от 0 до 2 вершины, в любом случае
					a.k = (double)(nodes[2].second - nodes[0].second) / (double)(nodes[2].first - nodes[0].first);
					a.x = nodes[0].first;
					a.yk = nodes[2].second;
					a.y = nodes[0].second;
					a.xk = nodes[2].first;
					a.dy = -netsize;
					//приращение по x = приращение по y / угловой коэффициент
					a.dx = (int)((double)a.dy / a.k);
					a.dy = -netsize;
					if (abs(a.dx) > abs(a.xk - a.x)) a.dx = a.xk - a.x;
					a.yfin = nodes_net[2].second;
					active_edges.push_back(a);

					break;
				case 1:
				case 2:
				case 3:
					//создаем активное ребро от 1 до 3 вершины или от 2 до 4 вершины или от 6 до 7 вершины
					a.k = (double)(nodes[j + 2].second - nodes[j].second) / (double)(nodes[j + 2].first - nodes[j].first);
					a.x = nodes[j].first;
					a.yk = nodes[j + 2].second;
					a.y = nodes[j].second;
					a.xk = nodes[j + 2].first;
					a.dy = -netsize;
					//приращение по x = приращение по y / угловой коэффициент
					a.dx = (int)((double)a.dy / a.k);
					a.dy = -netsize;
					if (abs(a.dx) > abs(a.xk - a.x)) a.dx = a.xk - a.x;
					a.yfin = nodes_net[j + 2].second;
					active_edges.push_back(a);
					break;
				case 4:
					//если нижнее ребро горизонтально
					if (nodes[N - 2].second == nodes[N - 1].second)
					{
						w.push_back(make_pair(nodes[N - 2].first, nodes[N - 2].second));
					}
					else
					{						//создаем активное ребро от 6 до 7 вершины 
						a.k = (double)(nodes[N - 1].second - nodes[N - 2].second) / (double)(nodes[N - 1].first - nodes[N - 2].first);
						a.x = nodes[N - 2].first;
						a.yk = nodes[N - 1].second;
						a.y = nodes[N - 2].second;
						a.xk = nodes[N - 1].first;
						a.dy = -netsize;
						//приращение по x = приращение по y / угловой коэффициент
						a.dx = (int)((double)a.dy / a.k);
						if (abs(a.dx) > abs(a.xk - a.x)) a.dx = a.xk - a.x;
						a.yfin = nodes_net[N - 1].second;
						active_edges.push_back(a);
					}
					break;
				}
			}
		}

		//на строке
		for (i = 0; i < active_edges.size(); i++)
		{
			//посчитали новые координаты
			active_edges[i].x += active_edges[i].dx;
			active_edges[i].y += active_edges[i].dy;
			//нашли часть растовой сетки, содержащий точку, добавили его
			if (point_in_net(active_edges[i].x, active_edges[i].y, &x, &y)) w.push_back(make_pair(x, y));
		}

		//проходим по активным ребрам
		k = active_edges.size();
		for (i = 0; i < k; i++)
		{
			//если какое-то ребро должно закончиться
			if (y <= active_edges[i].yfin)
			{
				//убрали его
				if (active_edges.size() == 1) active_edges.erase(active_edges.begin(), active_edges.end());
				else active_edges.erase(active_edges.begin() + i);
				k = active_edges.size();
				i--;
			}
		}
		ycur--;
	} while (ycur > ymin - 1);	//пока не дошли до нижней строки
	//добавили последний узел еще раз
	w.push_back(make_pair(nodes_net[N - 1].first, nodes_net[N - 1].second));
	//отсортировали по x внутри строк
	sort(w.begin(), w.end(), group);
	//K - кол-во добавляемых квадратов растной сетки
	K = w.size();

	//добавили квадраты растной сетки текущей фигуры в общий массив
	for (i = 0; i < K; i++)
	{
		(*v).push_back(w[i]);
	}
	w.erase(w.begin(), w.end());
	return (K);
}

void Process_Mouse_Move(int x, int y)
{
	point p = point((GLint)x, height - (GLint)y);
	if (cen_ver.cent_stat == true)	//если центр установлен
	{
		cen_ver.vertex_x = p.x;
		cen_ver.vertex_y = p.y;
		cen_ver.cord_stat = true;	//разрешить расчет координат шестиугольника
	}
	glutPostRedisplay();
}
void Process_Mouse(int button, int state, int x, int y)
{
	if (state != GLUT_DOWN) return;		//если кнопка мыши не была отпущена

	if (button == GLUT_LEFT_BUTTON)
	{
		if (dot_mode)
			Change_Mode();//отключить режим редактирования точек

		if (!drawing_mode)
			drawing_mode = true;

		point p = point((GLint)x, height - (GLint)y);
		if (cen_ver.cent_stat == false)	//если центр еще не был установлен
		{
			current_figure = -1;
			cen_ver.center_x = p.x;
			cen_ver.center_y = p.y;
			cen_ver.cent_stat = true;	//установить центр
		}
		else
		{
			cen_ver.vertex_x = p.x;
			cen_ver.vertex_y = p.y;
			cen_ver.cent_stat = false;	//координаты определены
			cen_ver.cord_stat = false;	//и рассчитаны

			Finish_Primitive();
		}
	}
}
void Display(void)
{
	int i, j, k;
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	if (colour_shuffle_mode)		//если включен режим смешения цветов
	{
		glEnable(GL_COLOR_LOGIC_OP);
		if (colour_shuffle_mode == 1) glLogicOp(GL_AND);
		if (colour_shuffle_mode == 2) glLogicOp(GL_NAND);
	}
	vector <pair<int, int>> net_line;	//массив сеткелей (см. "Rasterization_algorithms.h")

	if (raster_mode == 1 || raster_mode == 2)	//построить контуры фигур
	{
		raster_size.push_back(0);
		k = 0;
		for (i = 0; i < v.size(); i++)
		{
			for (int j = 0; j < v[i].coord_polygon.size(); j++)
			{
				//каждое ребро - отрезок
				if (j != N - 1) k += contour_Bresenham(v[i].coord_polygon[j].x + v[i].Ox, v[i].coord_polygon[j].y + v[i].Oy, v[i].coord_polygon[j + 1].x + v[i].Ox, v[i].coord_polygon[j + 1].y + v[i].Oy, &net_line, v[i].Ox, v[i].Oy);

				//+последняя сторона от 0 до 6 координаты
				if (j == N - 1) k += contour_Bresenham(v[i].coord_polygon[N - 1].x + v[i].Ox, v[i].coord_polygon[N - 1].y + v[i].Oy, v[i].coord_polygon[0].x + v[i].Ox, v[i].coord_polygon[0].y + v[i].Oy, &net_line, v[i].Ox, v[i].Oy);
			}
			raster_size.push_back(k);
		}
	}

	if (raster_mode == 3)	//построить заливку фигур
	{
		k = 0; 
		raster_size.push_back(0);
		for (i = 0; i < v.size(); i++)
		{
			k += rasterize(v[i], &net_line);
			raster_size.push_back(k);
		}
	}


	for (i = 0; i < v.size(); i++)
	{

		glPushMatrix();								//сохранить текущую матрицу
		glTranslatef(v[i].Ox, v[i].Oy, 0);			//сдвиг от начала координат
		glScalef(v[i].ScaleOx, v[i].ScaleOy, 1);	//масштабирование вдоль осей
		glRotatef(v[i].Angle, 0, 0, 1);				//вращение фигуры

		if (v[i].texture_number != -1)	//если шестиугольник с текстурой
		{
			switch (v[i].overlay_type_tex) //определить тип наложения текстуры
			{
			case 1:
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//результирующий цвет получается умножением
				break;
			case 2:
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//цвет объекта не учитывается
				break;
			}
			glBindTexture(GL_TEXTURE_2D, names_tex[v[i].texture_number]);	//выбрать текстуру
			glEnable(GL_TEXTURE_2D);	//разрешить текстурирование
		}

		//отрисовать шестиугольник

		glBegin(GL_POLYGON);
		for (j = 0; j < v[i].coord_polygon.size(); j++)
		{
			glColor3ub(v[i].color_point[j].red, v[i].color_point[j].green, v[i].color_point[j].blue);
			glTexCoord2f(texture_coord[j].x, texture_coord[j].y);
			glVertex2i(v[i].coord_polygon[j].x, v[i].coord_polygon[j].y);
		}
		
		glEnd();

		if (v[i].texture_number != -1)
		{
			glDisable(GL_TEXTURE_2D);//запретить текстурирование
		}

		//обвести зеленым цветом (текущий)
		if (v[i].number == current_figure)
		{
			if (!raster_mode)
			{
				glBegin(GL_LINE_LOOP);
				glLineWidth(3.0);
				glColor3ub(0, 240, 40);
				for (j = 0; j < v[i].coord_polygon.size(); j++)
				{
					glVertex2i(v[i].coord_polygon[j].x, v[i].coord_polygon[j].y);
				}
				glEnd();
			}

			if (dot_mode && !raster_mode)//если включен режим редактирования точек, выделить текущую точку красным
			{
				glPointSize(10);
				glColor3ub(225, 0, 0);
				glBegin(GL_POINTS);
				glVertex2i(v[i].coord_polygon[current_dot].x, v[i].coord_polygon[current_dot].y);
				glEnd();
			}
		}


		glPopMatrix();	//восстановить матрицу

		//растеризируем многоугольники на сетке в режиме "контур"
		if (raster_mode == 1)
		{
			glBegin(GL_QUADS);
			for (k = raster_size[i]; k < raster_size[i + 1]; k++)
			{
				glColor3ub(30, 30, 30);
				glVertex2i(netsize * (net_line[k].first - 1), netsize * (net_line[k].second - 1));
				glVertex2i(netsize * (net_line[k].first), netsize * (net_line[k].second - 1));
				glVertex2i(netsize * (net_line[k].first), netsize * (net_line[k].second));
				glVertex2i(netsize * (net_line[k].first - 1), netsize * (net_line[k].second));
			}
			glEnd();

			glPushMatrix();								//сохранить текущую матрицу
			glTranslatef(v[i].Ox, v[i].Oy, 0);			//сдвиг от начала координат
			glScalef(v[i].ScaleOx, v[i].ScaleOy, 1);	//масштабирование вдоль осей
			glRotatef(v[i].Angle, 0, 0, 1);				//вращение фигуры

			glBegin(GL_LINE_LOOP);
			glLineWidth(3.0);
			glColor3ub(252, 48, 58);
			for (j = 0; j < v[i].coord_polygon.size(); j++)
			{
				glVertex2i(v[i].coord_polygon[j].x, v[i].coord_polygon[j].y);
			}
			glEnd();

			glPopMatrix();	//восстановить матрицу
		}

		if (raster_mode == 2)
		{
			for (j = 0; j < raster_size.size() - 1; j++)
			{
				glColor3ub(v[j].color_point[0].red, v[j].color_point[0].green, v[j].color_point[0].blue);

				sort(net_line.begin() + raster_size[j], net_line.begin() + raster_size[j + 1], pred);
				sort(net_line.begin() + raster_size[j], net_line.begin() + raster_size[j + 1], group);
				for (k = raster_size[j]; k < raster_size[j + 1] - 1; k++)
				{
					glBegin(GL_QUADS);
					int k2;
					//идем по "строкам" y, заполняя только нужную часть на строке
					for (k2 = net_line[k].first; k2 <= net_line[k + 1].first; k2++)
					{
						glVertex2i(netsize * (k2 - 1), netsize * (net_line[k].second - 1));
						glVertex2i(netsize * (k2), netsize * (net_line[k].second - 1));
						glVertex2i(netsize * (k2), netsize * (net_line[k].second));
						glVertex2i(netsize * (k2 - 1), netsize * (net_line[k].second));
					}
					glEnd();
				}
			}
		}

		//растеризируем многоугольники на сетке в режиме "заливка"
		if (raster_mode == 3)
		{
			for (k = raster_size[i]; k < raster_size[i + 1] - 1; k++)
			{
				glColor3ub(v[i].color_point[0].red, v[i].color_point[0].green, v[i].color_point[0].blue);
				if (net_line[k].second == net_line[k + 1].second)
				{
					glBegin(GL_QUADS);
					//идем по "строкам" y, заполняя только нужную часть на строке
					for (j = net_line[k].first; j <= net_line[k + 1].first; j++)
					{
						glVertex2i(netsize * (j - 1), netsize * (net_line[k].second - 1));
						glVertex2i(netsize * (j), netsize * (net_line[k].second - 1));
						glVertex2i(netsize * (j), netsize * (net_line[k].second));
						glVertex2i(netsize * (j - 1), netsize * (net_line[k].second));
					}
					glEnd();
				}
			}
		}
		//конец растеризации
	}

	if (drawing_mode)
	{
		if (cen_ver.cord_stat == true)	//если можно рассчитывать координаты
		{
			point cen = point(cen_ver.center_x, cen_ver.center_y);
			point ver = point(cen_ver.vertex_x, cen_ver.vertex_y);

			if (!equality(cen, ver))	//если координаты центра и вершины не совпадают
			{
				glPushMatrix();					//сохранить текущую матрицу
				glTranslatef(cen.x, cen.y, 0);	//переместить координаты в установленный центр
				calc_polygon_coord(cen, ver);	//рассчитать координаты шестиугольника

				//отрисовать шестиугольник
				glBegin(GL_POLYGON);
				glColor3ub(drawing_color.red, drawing_color.green, drawing_color.blue);
				for (i = 0; i < drawing_v.size(); i++)
					glVertex2i(drawing_v[i].x, drawing_v[i].y);
				glEnd();

				//обвести красным цветом (отрисовываемый)
				glLineWidth(3.0);
				glColor3ub(225, 0, 0);
				glBegin(GL_LINE_LOOP);
				for (i = 0; i < drawing_v.size(); i++)
				{
					glVertex2i(drawing_v[i].x, drawing_v[i].y);
				}
				glEnd();
				glPopMatrix();	//восстановить матрицу
			}
		}
	}

	//растеризация на сетке
	if (raster_mode)	//включить сетку
	{
		glLineWidth(0.5);
		glColor3ub(70, 70, 70);
		glBegin(GL_LINES);
		for (i = 0; i < width / netsize + 1; i++)
		{
			glVertex2i(i*netsize, 0);
			glVertex2i(i*netsize, height);
		}
		for (j = 0; j < height / netsize + 1; j++)
		{
			glVertex2i(0, j*netsize);
			glVertex2i(width, j*netsize);
		}
		glEnd();
	}

	//очистить массивы сеткелей
	net_line.erase(net_line.begin(), net_line.end());
	raster_size.erase(raster_size.begin(), raster_size.end());

	//если включен режим смешения цветов, отключить его
	if (colour_shuffle_mode != 0) glDisable(GL_COLOR_LOGIC_OP);

	glFinish();
}
void Process_Normal_Keys(unsigned char key, int x, int y)
{
	if (drawing_mode);

	if (!drawing_mode && max_figure != 0)
	{
		if (key == 32)		//пробел
		{
			Change_Mode(); //включить/отключить режим редактирования точек
		}
		if (key == 127)		//delete
		{
			Del_Group();
		}
		if (!dot_mode)
		{
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

			if (key == '+')
			{
				Increase(0.05);
			}

			if (key == '-')
			{
				Reduce(0.05);
			}

			if (key == 'l' || key == 'L')
			{
				SwitchColourShuffleMode();
			}


		}
	}
	if (!drawing_mode && !dot_mode)
	{
		if (key == 'n' || key == 'N')
		{
			SwitchRasterMode();
		}
	}

	if (!drawing_mode && !dot_mode && raster_mode)
	{
		if (key == 'm' || key == 'M')
		{
			SwitchNetSize();
		}
	}

	if (key == 'p' || key == 'P')
	{
		Clear();
	}

	glutPostRedisplay();
}
void Process_Special_Keys(int key, int x, int y)
{
	if (!drawing_mode && max_figure != 0)
	{
		if (key == GLUT_KEY_UP)
		{
			On_Up_Key();
		}

		if (key == GLUT_KEY_DOWN)
		{
			On_Down_Key();
		}
		if (!dot_mode)
		{
			if (key == GLUT_KEY_LEFT)
			{
				RotateLeft();
			}

			if (key == GLUT_KEY_RIGHT)
			{
				RotateRight();
			}
		}
	}
	glutPostRedisplay();
}
void Initialize()
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
	dot_mode = false;
	max_figure = 0;
	current_figure = 0;
	current_dot = 0;

	cen_ver.cent_stat = false;	//центр отрисовываемого шестиугольника не установлен
	cen_ver.cord_stat = false;	//расчет координат шестиугольника запрещен

	raster_mode = 0;	//по умолчанию сетка отключена
	netsize = 6;	//размер сетки по умолчанию

	glutSetMenu(main_menu1);	//установить основное меню

	loadTexture();		//загрузить текстуры
	set_texture_coord();//рассчитать текстурные координаты

	colour_shuffle_mode = 0;	//по умолчанию нет смешения цветов 

	////
	//current_figure = max_figure;
	////установка атрибутов фигуры
	//drawing_polygon.number = current_figure;
	////сдвиг фигуры от начала координат равен центру фигуры
	//drawing_polygon.Ox = 300;
	//drawing_polygon.Oy = 300;
	////масштаб = не масштабирована
	//drawing_polygon.ScaleOx = 1;
	//drawing_polygon.ScaleOy = 1;
	////угол поворота = 0
	//drawing_polygon.Angle = 0;
	//point cen = point(300, 300);
	//point ver = point(340, 200);
	////300, 357, 362
	//calc_polygon_coord(cen, ver);
	////текстура отсутствует
	//drawing_polygon.texture_number = -1;
	////тип наложения текстуры
	//drawing_polygon.overlay_type_tex = 1;
	//for(int i=0; i<drawing_v.size(); i++)
	//{
	//	//поместить текущий цвет вершин в отрисовываемый шестиугольник
	//	drawing_polygon.color_point.push_back(drawing_color);
	//	//поместить текущие вершины в отрисовываемый шестиугольник
	//	drawing_polygon.coord_polygon.push_back(drawing_v[i]);
	//}
	//v.push_back(drawing_polygon);//поместить отрисовываемый шестиугольник в вектор
	////очистка векторов
	//drawing_v.erase(drawing_v.begin(), drawing_v.end());
	//drawing_polygon.color_point.erase(drawing_polygon.color_point.begin(), drawing_polygon.color_point.end());
	//drawing_polygon.coord_polygon.erase(drawing_polygon.coord_polygon.begin(), drawing_polygon.coord_polygon.end());
	//max_figure++;
}
void Reshape(GLint w, GLint h)
{
	//изменить размеры окна
	width = w;
	height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0.0, width, 0.0, height);
}