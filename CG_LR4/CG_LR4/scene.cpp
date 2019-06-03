#include"scene.h"
//описание методов класса Color

Light _Light;
Color::Color()
{
	red = green = blue = 0;
}
Color::Color(GLfloat _red, GLfloat _green, GLfloat _blue)
{
	red = _red;
	green = _green;
	blue = _blue;
}
void Color::add(GLfloat _red, GLfloat _green, GLfloat _blue)
{
	red += _red;
	green += _green;
	blue += _blue;
}
void Color::add(Color colr, Color refl)
{
	red += colr.red * refl.red;
	green += colr.green * refl.green;
	blue += colr.blue * refl.blue;
}
void Color::add(Color colr)
{
	red += colr.red;
	green += colr.green;
	blue += colr.blue;
}

//описание методов класса Light
Light::Light()
{

}
void Light::setPosition(Vector3f pos)
{
	position = Vector3f(pos.x, pos.y, pos.z);
}
void Light::setColor(Color col)
{
	color = Color(col.red, col.green, col.blue);
}

//описание методов класса Sphere
Sphere::Sphere()
{

}
Sphere::Sphere(GLfloat _radius, Vector3f _center, Color _color)
{
	radius = _radius;
	center_coord = _center;
	color = _color;
	FrameColor = Color(255, 0, 0);

	display = true;

	//свойства материала по умолчанию
	Properties.Ambient[0] = 0.2f; Properties.Ambient[1] = 0.2f; Properties.Ambient[2] = 0.2f; Properties.Ambient[3] = 1.0f;
	Properties.Diffuse[0] = 0.8f; Properties.Diffuse[1] = 0.8f; Properties.Diffuse[2] = 0.8f; Properties.Diffuse[3] = 1.0f;
	Properties.Specular[0] = 0.0f; Properties.Specular[1] = 0.0f; Properties.Specular[2] = 0.0f; Properties.Specular[3] = 1.0f;
	/*Properties.Emission[0] = 0.0f; Properties.Emission[1] = 0.0f; Properties.Emission[2] = 0.0f; Properties.Emission[3] = 1.0f;*/
	Properties.Shininess = 0.0f;
}
void Sphere::Draw() 
{
	GLUquadricObj *quadObj;

	//создаем новый объект для создания сферы
	quadObj = gluNewQuadric();

	//сохраняем текущую матрицу
	glPushMatrix();
	
	//перемещаемся в центр фигуры
	glTranslated(center_coord.x, center_coord.y, center_coord.z);

	glColor3f(color.red, color.green, color.blue);

	//устанавливаем сплошной стиль объекта
	gluQuadricDrawStyle(quadObj, GLU_FILL); 

	//рисуем сферу
	gluSphere(quadObj, radius, 10, 10);
                                  
	//восстанавливаем матрицу
	glPopMatrix();

	//удаляем объект
	gluDeleteQuadric(quadObj);
}
void Sphere::DrawFrame()
{
	GLUquadricObj *quadObj;

	//создаем новый объект для создания сферы
	quadObj = gluNewQuadric();

	//сохраняем текущую матрицу
	glPushMatrix();
	
	//перемещаемся в центр фигуры
	glTranslated(center_coord.x, center_coord.y, center_coord.z);

	glColor3f(FrameColor.red, FrameColor.green, FrameColor.blue);

	//устанавливаем сплошной стиль объекта
	gluQuadricDrawStyle(quadObj, GLU_LINE); 

	//рисуем сферу
	gluSphere(quadObj, radius*1.01, 10, 10);
                                  
	//восстанавливаем матрицу
	glPopMatrix();

	//удаляем объект
	gluDeleteQuadric(quadObj);
}
bool Sphere::hit(Ray ray, Intersection &inter)
{
	Ray genRay;

	//создаем базовый луч
	genRay = transformRay(ray, center_coord, radius, radius, radius);

	double A, B, C;
	A = scal(genRay.dir, genRay.dir);
	B = scal(genRay.start, genRay.dir);
	C = scal(genRay.start, genRay.start) - 1.0;

	//вычисляем дискриминант
	double discrim = B*B - A*C;
	
	//нет соударения с объектом
	if(discrim < 0)
		return false;

	int num = 0; //число соударений на данный момент

	double discRoot = sqrt(discrim);
	double t1 = (-B - discRoot)/A;	//более раннее соударение
	
	//если соударение находится впереди глаза
	if(t1 > 0.00001)
	{
		//формируем запись о соударении
		//номер объекта будет установлен в вызывающем методе
		inter.hit[0].hitTime = t1;
		inter.hit[0].objectType = 0;
		inter.hit[0].surface = 0;
		inter.hit[0].isEntering = true;

		//мировые координаты точки соударения
		Vector3f P(rayPos(ray, t1));
		inter.hit[0].hitPoint = Vector3f(P.x, P.y, P.z);

		//коорднаты нормали в точке соударения
		P = Vector3f(rayPos(genRay, t1));
		inter.hit[0].hitNormal = Vector3f(P.x, P.y, P.z);
		num = 1;
	}

	double t2 = (-B + discRoot)/A;	//более позднее соударение
	if(t2 > 0.00001)
	{
		//формируем запись о соударении
		//номер объекта будет установлен в вызывающем методе
		inter.hit[num].hitTime = t2;
		inter.hit[num].objectType = 0;
		inter.hit[num].surface = 0;
		inter.hit[num].isEntering = false;

		//мировые координаты точки соударения
		Vector3f P(rayPos(ray, t2));
		inter.hit[num].hitPoint = Vector3f(P.x, P.y, P.z);

		//коорднаты нормали в точке соударения
		P = Vector3f(rayPos(genRay, t2));
		inter.hit[num].hitNormal = Vector3f(P.x, P.y, P.z);
		num++;
	}

	inter.numHits = num;
	return (num > 0);	
}
bool Sphere::hit(Ray ray)
{
	Ray genRay;

	//создаем базовый луч
	genRay = transformRay(ray, center_coord, radius, radius, radius);

	double A, B, C;
	A = scal(genRay.dir, genRay.dir);
	B = scal(genRay.start, genRay.dir);
	C = scal(genRay.start, genRay.start) - 1.0;

	//вычисляем дискриминант
	double discrim = B*B - A*C;
	
	//нет соударения с объектом
	if(discrim < 0)
		return false;

	double discRoot = sqrt(discrim);

	double t1 = (-B - discRoot)/A;	//более раннее соударение
	if(t1 >= 0.00001 && t1 <= 1.0)
		return true;

	double t2 = (-B + discRoot)/A;	//более позднее соударение
	if(t2 >= 0.00001 && t2 <= 1.0)
		return true;

	return false;
}

//описание методов класса Tetrahedron
Tetrahedron::Tetrahedron()
{

}
Tetrahedron::Tetrahedron(Vector3f _coord[], Vector3f _center, Color _color)
{
	coord[0] = Vector3f(_coord[0].x, _coord[0].y, _coord[0].z);
	coord[1] = Vector3f(_coord[1].x, _coord[1].y, _coord[1].z);
	coord[2] = Vector3f(_coord[2].x, _coord[2].y, _coord[2].z);
	coord[3] = Vector3f(_coord[3].x, _coord[3].y, _coord[3].z);
	center_coord = _center;
	color = _color;
	FrameColor = Color(255, 0, 0);

	display = true;

	//свойства материала по умолчанию
	Properties.Ambient[0] = 0.2f; Properties.Ambient[1] = 0.2f; Properties.Ambient[2] = 0.2f; Properties.Ambient[3] = 1.0f;
	Properties.Diffuse[0] = 0.8f; Properties.Diffuse[1] = 0.8f; Properties.Diffuse[2] = 0.8f; Properties.Diffuse[3] = 1.0f;
	Properties.Specular[0] = 0.0f; Properties.Specular[1] = 0.0f; Properties.Specular[2] = 0.0f; Properties.Specular[3] = 1.0f;
	/*Properties.Emission[0] = 0.0f; Properties.Emission[1] = 0.0f; Properties.Emission[2] = 0.0f; Properties.Emission[3] = 1.0f;*/
	Properties.Shininess = 0.0f;
}
void Tetrahedron::Draw()
{
	Vector3f normal;
	//сохраняем текущую матрицу
	glPushMatrix();

	//перемещаемся в центр фигуры
	glTranslated(center_coord.x, center_coord.y, center_coord.z);

	//рисуем тетраэдр
	normal = Normal(coord[0], coord[1], coord[2]);
	glBegin(GL_POLYGON);
	glNormal3f(normal.x, normal.y, normal.z);
		glColor3f(color.red, color.green, color.blue);
		glVertex3f(coord[0].x, coord[0].y, coord[0].z);
		glVertex3f(coord[1].x, coord[1].y, coord[1].z);
		glVertex3f(coord[2].x, coord[2].y, coord[2].z);
	glEnd();

	normal = Normal(coord[1], coord[3], coord[2]);
	glBegin(GL_POLYGON);
	glNormal3f(normal.x, normal.y, normal.z);
		glColor3f(color.red, color.green, color.blue);
		glVertex3f(coord[1].x, coord[1].y, coord[1].z);
		glVertex3f(coord[3].x, coord[3].y, coord[3].z);
		glVertex3f(coord[2].x, coord[2].y, coord[2].z);
	glEnd();

	normal = Normal(coord[3], coord[0], coord[2]);
	glBegin(GL_POLYGON);
	glNormal3f(normal.x, normal.y, normal.z);
		glColor3f(color.red, color.green, color.blue);
		glVertex3f(coord[3].x, coord[3].y, coord[3].z);
		glVertex3f(coord[0].x, coord[0].y, coord[0].z);
		glVertex3f(coord[2].x, coord[2].y, coord[2].z);
	glEnd();

	normal = Normal(coord[1], coord[0], coord[3]);
	glBegin(GL_POLYGON);
	glNormal3f(normal.x, normal.y, normal.z);
		glColor3f(color.red, color.green, color.blue);
		glVertex3f(coord[1].x, coord[1].y, coord[1].z);
		glVertex3f(coord[0].x, coord[0].y, coord[0].z);
		glVertex3f(coord[3].x, coord[3].y, coord[3].z);
	glEnd();

	//восстанавливаем матрицу
	glPopMatrix();
}
void Tetrahedron::DrawFrame()
{
	//сохраняем текущую матрицу
	glPushMatrix();

	//перемещаемся в центр фигуры
	glTranslated(center_coord.x, center_coord.y, center_coord.z);

	//рисуем тетраэдр
	glBegin(GL_LINE_LOOP);
		glLineWidth(5);
		glColor3f(FrameColor.red, FrameColor.green, FrameColor.blue);
		glVertex3f(coord[0].x, coord[0].y, coord[0].z);
		glVertex3f(coord[1].x, coord[1].y, coord[1].z);
		glVertex3f(coord[2].x, coord[2].y, coord[2].z);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3f(FrameColor.red, FrameColor.green, FrameColor.blue);
		glVertex3f(coord[1].x, coord[1].y, coord[1].z);
		glVertex3f(coord[3].x, coord[3].y, coord[3].z);
		glVertex3f(coord[2].x, coord[2].y, coord[2].z);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3f(FrameColor.red, FrameColor.green, FrameColor.blue);
		glVertex3f(coord[3].x, coord[3].y, coord[3].z);
		glVertex3f(coord[0].x, coord[0].y, coord[0].z);
		glVertex3f(coord[2].x, coord[2].y, coord[2].z);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor3f(FrameColor.red, FrameColor.green, FrameColor.blue);
		glVertex3f(coord[1].x, coord[1].y, coord[1].z);
		glVertex3f(coord[0].x, coord[0].y, coord[0].z);
		glVertex3f(coord[3].x, coord[3].y, coord[3].z);
	glEnd();

	//восстанавливаем матрицу
	glPopMatrix();
}
bool Tetrahedron::hit(Ray ray, Intersection &inter)
{
	float timeIn[4];	//время соударения с каждой плоскостью
	Vector3f N[4];		//нормаль к каждой плоскости

	//число пересечений равно 0
	inter.numHits = 0;

	//луч в базовых координатах
	Ray genRay = transformRay(ray, center_coord, 1, 1, 1);

	//если нет пересечения с первой плоскостью (0 1 2)
	if(!hitPlane(genRay, 0, 1, 2, N[0], timeIn[0]))
		timeIn[0] = -10000.0;
	else
		inter.numHits++;

	//если нет пересечения со второй плоскостью (1 3 2)
	if(!hitPlane(genRay, 2, 3, 1, N[1], timeIn[1]))
		timeIn[1] = -10000.0;
	else
		inter.numHits++;

	//если нет пересечения с третьей плоскостью (3 0 2)
	if(!hitPlane(genRay, 3, 0, 2, N[2], timeIn[2]))
		timeIn[2] = -10000.0;
	else
		inter.numHits++;

	//если нет пересечения с четвертой плоскостью (1 0 3)
	if(!hitPlane(genRay, 3, 0, 1, N[3], timeIn[3]))
		timeIn[3] = -10000.0;
	else
		inter.numHits++;

	//случай, что точка обзора(начало луча) находится в тетраэдре не рассматривается
	//если соударений меньше двух, соударений нет
	if(inter.numHits < 2)
		return false;
	else
	{
		//определить точку входа и точку выхода из фигуры
		int max;
		int min;

		max = 0;

		for(int i = 1; i < 4; i++)
			if(timeIn[i] > timeIn[max])
				max = i;

		min = max;
		for(int i = 0; i < 4; i++)
		{
			if(timeIn[i] < timeIn[min] && timeIn[i] > 0.0)
				min = i;
		}

		//если максимальное время отрицательно - соударения нет
		if(timeIn[max] < 0.0)
			return false;

		//формируем записи о соударениях
		inter.hit[0].objectType = 1;
		inter.hit[0].hitTime = timeIn[min];
		inter.hit[0].isEntering = true;
		inter.hit[0].surface = min;
		
		//мировые координаты точки соударения
		Vector3f P(rayPos(ray, timeIn[min]));
		inter.hit[0].hitPoint = Vector3f(P.x, P.y, P.z);
		
		//нормаль к точке соударения
		inter.hit[0].hitNormal = N[min];

		inter.hit[1].objectType = 1;
		inter.hit[1].hitTime = timeIn[max];
		inter.hit[1].isEntering = false;
		inter.hit[1].surface = max;

		//мировые координаты точки соударения
		Vector3f C(rayPos(ray, timeIn[max]));
		inter.hit[1].hitPoint = Vector3f(C.x, C.y, C.z);

		//нормаль к точке соударения 
		inter.hit[1].hitNormal = N[max];

		return true;
	}
}
bool Tetrahedron::hit(Ray ray)
{
	float timeIn;	//время соударения с каждой плоскостью
	Vector3f N[4];	//нормаль к каждой плоскости

	//число пересечений равно 0
	int num = 0;

	//луч в базовых координатах
	Ray genRay = transformRay(ray, center_coord, 1, 1, 1);

	//если пересечение с первой плоскостью (0 1 2)
	if(hitPlane(genRay, 0, 1, 2, N[0], timeIn))
		if( timeIn >= 0.0 &&  timeIn <= 1.0 )
			return true;

	//если пересечение со второй плоскостью (2 3 1)
	if(hitPlane(genRay, 2, 3, 1, N[1], timeIn))
		if( timeIn >= 0.0 &&  timeIn <= 1.0 )
			return true;

	//если пересечение с третьей плоскостью (3 0 2)
	if(hitPlane(genRay, 3, 0, 2, N[2], timeIn))
		if( timeIn >= 0.0 &&  timeIn <= 1.0 )
			return true;

	//если  пересечение с четвертой плоскостью (3 0 1)
	if(hitPlane(genRay, 3, 0, 1, N[3], timeIn))
		if( timeIn >= 0.0 &&  timeIn <= 1.0 )
			return true;

	return false;
}
bool Tetrahedron::hitPlane(Ray ray, int ver1, int ver2, int ver3, Vector3f &_N, float &time)
{
	Vector3f P0;		//начальная точка луча
	Vector3f Dir;		//направление луча
	Vector3f A, B, C;	//вершины треугольника
	Vector3f N;			//нормаль треугольника

	Vector3f O;		//точка пересечения

	P0 = ray.start;
	Dir = ray.dir;
	A = coord[ver1];
	B = coord[ver2];
	C = coord[ver3];
	N = Normal(A, B, C);

	float vn = scalar(Dir, N);

	//находим точку соударения
	Vector3f V = P0 - A;
	float t = - scalar(V, N)/vn;
	V = Dir*t;
	O = P0 + V;

	//определить принадлежность треугольнику
	//выбираем плоскость для проекции YZ
	if( fabs(N.x) >= fabs(N.y) && fabs(N.x) >= fabs(N.z) )
	{
	  if( (O.z-A.z)*(B.y-A.y)-(O.y-A.y)*(B.z-A.z) < 0) return false; 
	  if( (O.z-B.z)*(C.y-B.y)-(O.y-B.y)*(C.z-B.z) < 0) return false; 
	  if( (O.z-C.z)*(A.y-C.y)-(O.y-C.y)*(A.z-C.z) < 0) return false; 

	}
	//выбираем плоскость для проекции XY
	else if( fabs(N.z) >= fabs(N.y) ) 
	{
	  if( (O.y-A.y)*(B.x-A.x)-(O.x-A.x)*(B.y-A.y) < 0) return false; 
	  if( (O.y-B.y)*(C.x-B.x)-(O.x-B.x)*(C.y-B.y) < 0) return false; 
	  if( (O.y-C.y)*(A.x-C.x)-(O.x-C.x)*(A.y-C.y) < 0) return false; 
	}
	//выбираем плоскость для проекции XZ
	else
	{
	  if( (O.z-A.z)*(B.x-A.x)-(O.x-A.x)*(B.z-A.z) < 0) return false; 
	  if( (O.z-B.z)*(C.x-B.x)-(O.x-B.x)*(C.z-B.z) < 0) return false; 
	  if( (O.z-C.z)*(A.x-C.x)-(O.x-C.x)*(A.z-C.z) < 0) return false; 
	}
	_N = N;
	time = t;
	return true;
}

//описание методов класс Square
Square::Square()
{

}
Square::Square(Vector3f _center, GLfloat _scaleX, GLfloat _scaleZ, Color _color)
{
	center_coord = _center;
	scaleX = _scaleX;
	scaleZ = _scaleZ;
	color = _color;

	//свойства материала по умолчанию
	Properties.Ambient[0] = 0.2f; Properties.Ambient[1] = 0.2f; Properties.Ambient[2] = 0.2f; Properties.Ambient[3] = 1.0f;
	Properties.Diffuse[0] = 0.8f; Properties.Diffuse[1] = 0.8f; Properties.Diffuse[2] = 0.8f; Properties.Diffuse[3] = 1.0f;
	Properties.Specular[0] = 0.2f; Properties.Specular[1] = 0.2f; Properties.Specular[2] = 0.2f; Properties.Specular[3] = 1.0f;
	/*Properties.Emission[0] = 0.0f; Properties.Emission[1] = 0.0f; Properties.Emission[2] = 0.0f; Properties.Emission[3] = 1.0f;*/
	Properties.Shininess = 0.6f;
}
void Square::Draw()
{
	Vector3f normal = Normal(Vector3f(-1, 0, -1), Vector3f(-1, 0, 1), Vector3f(1, 0, 1));
	//сохраняем текущую матрицу
	glPushMatrix();

	//масштабируем квадрат
	glScalef(scaleX, 1, scaleZ);

	//перемещаемся в центр фигуры
	glTranslated(center_coord.x, center_coord.y, center_coord.z);

	//рисуем квадрат
	glBegin(GL_POLYGON);
	glNormal3f(normal.x, normal.y, normal.z);
		glColor3f(color.red, color.green, color.blue);
		glVertex3f(-1, 0, -1);
		glVertex3f(-1, 0, 1);
		glVertex3f(1, 0, 1);
		glVertex3f(1, 0, -1);
	glEnd();

	//восстанавливаем матрицу
	glPopMatrix();
}
bool Square::hit(Ray ray, Intersection &inter)
{
	Ray genRay;

	//переход к базовому лучу
	genRay = transformRay(ray, center_coord, scaleX, 1, scaleZ);

	inter.numHits = 0;

	double denom = genRay.dir.y;

	//луч параллелен плоскости
	if(fabs(denom) < 0.0001) return false;

	double time = -genRay.start.y/denom;	//время соударения

	//квадрат лежит позади вгзляда
	if(time <= 0.0) return false;

	double hx = genRay.start.x + genRay.dir.x*time;
	double hz = genRay.start.z + genRay.dir.z*time;

	//проходит мимо в направлении х
	if(hx > 1.0 || hx < -1.0) return false;

	//проходит мимо в направлении y
	if(hz > 1.0 || hz < -1.0) return false;

	//есть соударение
	inter.numHits = 1;
	inter.hit[0].objectType = 10;
	inter.hit[0].hitTime = time;
	inter.hit[0].isEntering = true;
	inter.hit[0].surface = 0;

	//точка соударения в мировых координатах
	Vector3f P(rayPos(ray, time));
	inter.hit[0].hitPoint = Vector3f(P.x, P.y, P.z);

	//нормаль в базовых
	inter.hit[0].hitNormal = Vector3f(0, 1, 0);

	return true;
}
bool Square::hit(Ray ray)
{
	Ray genRay;

	//переход к базовому лучу
	genRay = transformRay(ray, center_coord, scaleX, 1, scaleZ);

	double denom = genRay.dir.y;

	//луч параллелен плоскости
	if(fabs(denom) < 0.0001) return false;

	double time = -genRay.start.y/denom;	//время соударения

	//квадрат лежит позади вгзляда
	if(time <= 0.0) return false;

	double hx = genRay.start.x + genRay.dir.x*time;
	double hz = genRay.start.z + genRay.dir.z*time;

	//проходит мимо в направлении х
	if(hx > 1.0 || hx < -1.0) return false;

	//проходит мимо в направлении y
	if(hz > 1.0 || hz < -1.0) return false;

	if(time >=0 && time <= 1)
		return true;

	return false;
}
//описание методов класса Scene
Scene::Scene()
{
	lighting_num = 1;
	set_l();
	set_data();
	add_del = false;
	sphere_mod = false;
	tetrahedron_mod = false;
	active_Sphere = 0;
	active_Tetrahedron = 0;
}

Light Scene::set_l()
{
	//источники освещения

	vector_Light.clear();
	if (lighting_num == 1)
	{
		_Light.position = Vector3f(-5, 2, -5);
		_Light.color = Color(1, 1, 1);
		vector_Light.push_back(_Light);
	}

	if (lighting_num == 2)
	{
		_Light.position = Vector3f(-13.0, 1.0, 8.0);
		_Light.color = Color(1, 0, 0);
		vector_Light.push_back(_Light);
	}

	if (lighting_num == 3)
	{
		_Light.position = Vector3f(0, 15, 0);
		_Light.color = Color(1, 0, 1);
		vector_Light.push_back(_Light);
	}
	if (lighting_num == 4)
	{
		_Light.position = Vector3f(0, 15, 0);
		_Light.color = Color(0, 0, 1);
		vector_Light.push_back(_Light);
	}

	return _Light;
}


void Scene::set_material()
{
	char str[10] = "mat1S.txt";

	for (int  j= 0; j < vector_Sphere.size(); j++)
	{
		FILE *in_mat = fopen(str, "r");
		int is;

		fscanf(in_mat, "%d", &is);
		//если указано фоновое отражение
		if(is)
			for(int i=0; i<4; i++)
				fscanf(in_mat, "%f", &vector_Sphere[j].Properties.Ambient[i]);

		fscanf(in_mat, "%d", &is);
			//если указано рассеянное отражение
		if(is)
			for(int i=0; i<4; i++)
				fscanf(in_mat, "%f", &vector_Sphere[j].Properties.Diffuse[i]);

		fscanf(in_mat, "%d", &is);
		//если указано зеркальное отражение
		if(is)
			for(int i=0; i<4; i++)
				fscanf(in_mat, "%f", &vector_Sphere[j].Properties.Specular[i]);

		//fscanf(in_mat, "%d", &is);
		////если указано собственное излучение
		//if(is)
		//	for(int i=0; i<4; i++)
		//		fscanf(in_mat, "%f", &vector_Sphere[j].Properties.Emission[i]);

		fscanf(in_mat, "%d", &is);
		//если указан коэффициент блеска
		if(is)
			fscanf(in_mat, "%f", &vector_Sphere[j].Properties.Shininess);

		str[3]++;
		fclose(in_mat);
	}

	char str_[10] = "mat1T.txt";

	for (int j = 0; j < vector_Tetrahedron.size(); j++)
	{
		FILE *in_mat = fopen(str_, "r");
		int is;

		fscanf(in_mat, "%d", &is);
		//если указано фоновое отражение
		if (is)
			for (int i = 0; i < 4; i++)
				fscanf(in_mat, "%f", &vector_Tetrahedron[j].Properties.Ambient[i]);

		fscanf(in_mat, "%d", &is);
		//если указано рассеянное отражение
		if (is)
			for (int i = 0; i < 4; i++)
				fscanf(in_mat, "%f", &vector_Tetrahedron[j].Properties.Diffuse[i]);

		fscanf(in_mat, "%d", &is);
		//если указано зеркальное отражение
		if (is)
			for (int i = 0; i < 4; i++)
				fscanf(in_mat, "%f", &vector_Tetrahedron[j].Properties.Specular[i]);

		//fscanf(in_mat, "%d", &is);
		////если указано собственное излучение
		//if (is)
		//	for (int i = 0; i < 4; i++)
		//		fscanf(in_mat, "%f", &vector_Tetrahedron[j].Properties.Emission[i]);

		fscanf(in_mat, "%d", &is);
		//если указан коэффициент блеска
		if (is)
			fscanf(in_mat, "%f", &vector_Tetrahedron[j].Properties.Shininess);

		str_[3]++;
		fclose(in_mat);
	}
}

void Scene::set_data()
{
	FILE *in, *in_mat;
	Sphere _Sphere;
	Tetrahedron _Tetrahedron;
	int type;

	in = fopen("scene_objects.txt", "r");
	
	while(!feof(in))
	{
		fscanf(in, "%d", &type);
		switch (type)
		{
			//сфера
		case 0:
			{
				float _radius;
				Vector3f _center;
				Color _color;

				fscanf(in, "%f", &_radius);
				fscanf(in, "%f%f%f", &_center.x, &_center.y, &_center.z);
				fscanf(in, "%f%f%f", &_color.red, &_color.green, &_color.blue);

				_Sphere = Sphere(_radius, _center, _color);
				vector_Sphere.push_back(_Sphere);
			}
			break;

			//тетраэдр
		case 1:
			{
				Vector3f _coord[4];
				Vector3f _center;
				Color _color;

				for(int i=0; i<4; i++)
					fscanf(in, "%f%f%f", &_coord[i].x, &_coord[i].y, &_coord[i].z);
				fscanf(in, "%f%f%f", &_center.x, &_center.y, &_center.z);
				fscanf(in, "%f%f%f", &_color.red, &_color.green, &_color.blue);

				_Tetrahedron = Tetrahedron(_coord, _center, _color);
				vector_Tetrahedron.push_back(_Tetrahedron);
			}
			break;
		}
	}
	fclose(in);
	//fclose(in_mat);
	set_material();
	//плоскость
	square = Square(Vector3f(0,-2,0), 10, 10, Color(0,0,0));

	
}
void Scene::switch_forward()
{
	//работа со сферами
	if(sphere_mod)
	{
		active_Sphere++;
		if(active_Sphere == vector_Sphere.size())
			active_Sphere = 0;
	}

	//работа с тетраэдрами
	if(tetrahedron_mod)
	{
		active_Tetrahedron++;
		if(active_Tetrahedron == vector_Tetrahedron.size())
			active_Tetrahedron = 0;
	}
}
void Scene::switch_backward()
{
	//работа со сферами
	if(sphere_mod)
	{
		active_Sphere--;
		if(active_Sphere == -1)
			active_Sphere = vector_Sphere.size()-1;
	}

	//работа с тетраэдрами
	if(tetrahedron_mod)
	{
		active_Tetrahedron--;
		if(active_Tetrahedron == -1)
			active_Tetrahedron = vector_Tetrahedron.size()-1;
	}
}


void Scene::Draw()
{
	for(int i=0; i<vector_Sphere.size(); i++)
	{
		if(vector_Sphere[i].display)
			vector_Sphere[i].Draw();

		if(add_del && sphere_mod && i == active_Sphere)
				vector_Sphere[i].DrawFrame();
	}

	for(int i=0; i<vector_Tetrahedron.size(); i++)
	{
		if(vector_Tetrahedron[i].display)
			vector_Tetrahedron[i].Draw();

		if(add_del && tetrahedron_mod && i == active_Tetrahedron)
				vector_Tetrahedron[i].DrawFrame();
	}

	square.Draw();
}
bool Scene::isInShadow(Ray ray)
{
	for(int i=0; i<vector_Sphere.size(); i++)
		if(vector_Sphere[i].display)
			if(vector_Sphere[i].hit(ray))
				return true;
	for(int i=0; i<vector_Tetrahedron.size(); i++)
		if(vector_Tetrahedron[i].display)
			if(vector_Tetrahedron[i].hit(ray))
				return true;
	if(square.hit(ray))
		return true ;
	return false;
}

//=============Для преобразования луча к базовому виду==============
//И все остальные вспомогательные подпрограммы

//определить матрицу трансформации
void makeTransformMatrix(double **TMatrix, Vector3f shift, GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
	TMatrix[0][0] = scaleX;	TMatrix[0][1] = 0;
	TMatrix[0][2] = 0;		TMatrix[0][3] = shift.x;
	TMatrix[1][0] = 0;		TMatrix[1][1] = scaleY;
	TMatrix[1][2] = 0;		TMatrix[1][3] = shift.y;
	TMatrix[2][0] = 0;		TMatrix[2][1] = 0;
	TMatrix[2][2] = scaleZ;	TMatrix[2][3] = shift.z;
	TMatrix[3][0] = 0;		TMatrix[3][1] = 0;
	TMatrix[3][2] = 0;		TMatrix[3][3] = 1;
}

//приведение луча к базовому виду
Ray transformRay(Ray _ray, Vector3f shift, GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
	double **TMatrix;
	TMatrix = new double *[4];
	for(int i = 0; i<4; i++)
		TMatrix[i] = new double [4];

	//определить матрицу трансформации
	makeTransformMatrix(TMatrix, shift, scaleX, scaleY, scaleZ);

	//определить начало луча и направление в однородных координатах
	double point[4] = {_ray.start.x, _ray.start.y, _ray.start.z, 1.0f};
	double dir[4] = {_ray.dir.x, _ray.dir.y, _ray.dir.z, 0.0f};

	double sol_point[4];
	double sol_dir[4];

	//найти базовые координаты начала луча и его направления
	sol_gauss(TMatrix, point, sol_point, 4);
	sol_gauss(TMatrix, dir, sol_dir, 4);

	//перевести в обычные координаты
	Vector3f newPoint = Vector3f(sol_point[0], sol_point[1], sol_point[2]);
	Vector3f newDir = Vector3f(sol_dir[0], sol_dir[1], sol_dir[2]);

	//создать новый луч
	Ray newRay;
	newRay.SetStart(newPoint);
	newRay.SetDir(newDir);

	for(int i = 0; i <4; i++)
		delete [] TMatrix[i];
	delete [] TMatrix;

	//вернуть новый луч
	return newRay;
}

//скалярное произведение
double scal(Vector3f vec1, Vector3f vec2)
{
	return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}

//определить позицию луча
Vector3f rayPos(Ray ray, double t)
{
	Vector3f vec;
	vec = Vector3f(ray.start.x + ray.dir.x * t,
				ray.start.y + ray.dir.y * t,
				ray.start.z + ray.dir.z * t);
	return vec;
}