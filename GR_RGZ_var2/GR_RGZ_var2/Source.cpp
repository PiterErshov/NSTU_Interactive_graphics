#include <windows.h> 
#include "gl\glut.h"
#include <string>
#include <vector>

using namespace std;

class Point//структура ТОЧКА
{
public: double x, y;
		Point()
		{
			x = 0;
			y = 0;
		}
		~Point() {}
public: void SetCoordinates(double x2, double y2)
{
	x = x2;
	y = y2;
}
};

vector <Point> points;
vector <Point> l_coord;	// точки полинома Лангранжа
Point shift;
bool newPoint = false;
int width = 1200;
int height = 800;
double scale = 2;


void drawBitmapText(string str, double x, double y, double z)
{
	glRasterPos3f(x, y, z);		// Определяет положение растра для операций с пикселями
	for (int i = 0; i < str.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
	}
}

void drawDot(int x, int y)//отрисовка точки
{
	glColor3f(0.3, 0.5, 0.7);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void drawLine(Point p1, Point p2)//отрисовка линии
{
	glColor3f(0.7, 0.4, 0.3);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void drawGrid()//отрисовка сетки
{
	glLineWidth(1);
	glColor3d(0, 0, 0);
	{
		glBegin(GL_LINES);
		for (int i = 0; i < 10000; i += 100 / scale)
		{
			glVertex2f(i, -10000);
			glVertex2f(i, 10000);
			glVertex2f(-10000, i);
			glVertex2f(10000, i);
		}
		glEnd();
		glBegin(GL_LINES);
		for (int i = 0; i > -10000; i -= 100 / scale)
		{
			glVertex2f(i, -10000);
			glVertex2f(i, 10000);
			glVertex2f(-10000, i);
			glVertex2f(10000, i);

		}
		glEnd();

		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2f(0, -10000);
		glVertex2f(0, 10000);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(-10000, 0);
		glVertex2f(10000, 0);
		glEnd();
	}

	char number[4];
	glLineWidth(3);
	drawBitmapText("0", 10, -10, 0);
	for (double i = 100 / scale; i < 10000; i += 100 / scale)		// OX
	{
		glBegin(GL_LINES);
		glVertex2f(i, 5);
		glVertex2f(i, -5);
		glEnd();
		drawBitmapText(to_string(i / 100.0), i + 3, -20 / scale, 0);
	}
	for (double i = -100 / scale; i > -10000; i -= 100 / scale)
	{
		glBegin(GL_LINES);
		glVertex2f(i, 5);
		glVertex2f(i, -5);
		glEnd();
		drawBitmapText(to_string(i / 100.0), i + 3, -20 / scale, 0);
	}
	glEnd();

	for (double i = 100 / scale; i < 10000; i += 100 / scale)
	{
		glBegin(GL_LINES);
		glVertex2f(-5, i);
		glVertex2f(5, i);
		glEnd();
		drawBitmapText(to_string(i / 100.0), 15 / scale, i + 3, 0);
	}
	for (double i = -100 / scale; i > -10000; i -= 100 / scale)
	{
		glBegin(GL_LINES);
		glVertex2f(-5, i);
		glVertex2f(5, i);
		glEnd();
		drawBitmapText(to_string(i / 100.0), 15 / scale, i + 3, 0);
	}
}

void Lagrange()//Расчёт сплайна
{
	l_coord.clear();

	double a = 0, b = 0, c = 0, d = 0, y = 0, ab = 0, bb = 0, cb = 0, h = 0;
	double beta = 0, gamma = 0;
	double c_x = 0;

	if (points.size() > 1)
		for (c_x = points[0].x; c_x != points[1].x; c_x += 1 / 8.000)
		{
			h = points[1].x - points[0].x;
			a = 0;
			b = 0;
			c = (points[1].y - points[0].y) / h;
			d = points[0].y - c * points[0].x;

			y = c * c_x + d;
			Point cur;
			cur.x = c_x;
			cur.y = y;
			l_coord.push_back(cur);
		}
	ab = a;
	bb = b;
	cb = c;
	if (points.size() > 1)
	{
		for (int i = 1; i < points.size() - 1; i++)
		{
			ab /= 1.5;
			bb /= 1.5;
			cb /= 1.5;
			for (c_x = points[i].x; c_x != points[i + 1].x;c_x += 1 / 8.000)
			{
				//*
				h = points[i + 1].x - points[i].x;
				
				beta = 3 * ab * pow(points[i].x, 2) + 2* bb * points[i].x + cb;
				gamma = 3 * ab * points[i].x + bb;

				a = ((points[i + 1].y - points[i].y) / pow(h, 3)) - (beta / pow(h, 2)) - gamma / h;
				b = gamma - 3 * a * points[i].x;
				c = beta - 2 * b * points[i].x - 3 * a * pow(points[i].x, 2);
				d = points[i].y - c * points[i].x - b * pow(points[i].x, 2) - a * pow(points[i].x, 3);

				y = a * pow(c_x, 3) + b * pow(c_x, 2) + c * c_x + d;
				ab = a;
				bb = b;
				cb = c;
				Point cur;
				cur.x = c_x;
				cur.y = y; 
				l_coord.push_back(cur);
			}
		}
	}
	
}

void del()//очистить вектор точек
{
	points.clear();
}

void repos_point()//сортировка точек
{
	for (int i = points.size() - 1; i > 0; i--)
	{
		if (points[i].x < points[i - 1].x)
		{
			Point b;
			b.x = points[i].x;
			b.y = points[i].y;
			points[i] = points[i - 1];
			points[i - 1] = b;
		}
	}
}

bool x_not_eq(int x)//закпрет на добавление точки на уже занятую координату Х
{
	int i = 0;
	bool f = true;
	while (i < points.size())
	{
		if (points[i].x == x)
		{
			f = false;
			break;
		}
		i++;
	}
	return f;
}

void MouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		Point p;
		p.SetCoordinates((double)(x - width / 2 - shift.x) / scale, (double)(height / 2 - y - shift.y) / scale);
		
		if (x_not_eq(p.x) != false)
		{
			points.push_back(p);
			repos_point();
			Lagrange();
		}
	}
}

void Keyboard(GLubyte key, int x, int y)
{
	switch (key)
	{
	case 'w':
	{
		shift.y -= 25;	break;
	}
	case 'a':
	{
		shift.x += 25;	break;
	}
	case 's':
	{
		shift.y += 25; break;
	}
	case 'd':

	{
		shift.x -= 25; break;
	}
	case '=':
	{
		scale += 0.1; break;
	}
	case '-':
	{
		scale -= 0.1; break;
	}
	case 'z'://удалить кривую
	{
		del();
	}
	}
	glutPostRedisplay();
}

void Reshape(GLint w, GLint h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

void Display()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
	glShadeModel(GL_SMOOTH);

	glLoadIdentity();
	glTranslated(shift.x, shift.y, 0);
	glScalef(scale, scale, scale);

	glLineWidth(1);
	drawGrid();
	glLineWidth(3);
	glPointSize(6);

	for (int i = 0; i < points.size(); i++)
	{
		drawDot(points[i].x, points[i].y);
	}
	if (points.size() > 1)
	{
		Point from, to;	// Инициализируем начальную и конечную точку для отрисовки отрезка
		from.x = 0;
		from.y = 0;
		to.x = 0;
		to.y = 0;
		for (int i = 1; i < l_coord.size(); i++)
		{
			from.x = l_coord[i - 1].x;
			from.y = l_coord[i - 1].y;
			to.x = l_coord[i].x;
			to.y = l_coord[i].y;
			drawLine(from, to);
		}
		from.x = l_coord[l_coord.size() - 1].x;
		from.y = l_coord[l_coord.size() - 1].y;
		drawLine(from, points[points.size() - 1]);
	}
	glFinish();
}

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(width / 10, height / 6);
	glutCreateWindow("Интерполяционный сплайн на основе полиномов Лагранжа степени 3");
	glutMouseFunc(MouseClick);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutMainLoop();
}