#include"tracer.h"

//описание методов класса Raytracer
Raytracer::Raytracer()
{

}
Raytracer::Raytracer(Vector3f _eye, Scene _scene, Camera _camera, float _aspect, int _nCols, int _nRows, int _blockSize)
{
	eye = _eye;
	scene = _scene;
	camera = _camera;
	aspect = _aspect;
	nCols = _nCols;
	nRows = _nRows;
	blockSize = _blockSize;
}

void Raytracer::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, nCols, 0, nRows);
	glDisable(GL_LIGHTING);
	for (int i = 0; i < PIX.size(); i++)
	{
		pix temp = PIX[i];
		//закрашиваем блок пикселей полученным цветом
		glColor3f(temp.color.red, temp.color.green, temp.color.blue);
		glRecti(temp.p.x1, temp.p.y1, temp.p.x2, temp.p.y2);
	}
}

void Raytracer::Raytrace()
{
	Ray ray;		//луч
	Color color;	//цвет
	float tetha = 60*(M_PI/180.);	//угол охвата
	float N = 0.1;					//расстояние от точки взгляда, до ближней плоскости
									//параметры из этой ф-и gluPerspective(60, ratio, 0.1f, 100.0f);

	//расчитываем новую систему координат, связанную с наблюдателем
	Vector3f n = camera.Position-camera.View;
	Vector3f u = Cross(camera.UpVector, n);
	Vector3f v = Cross(n, u);

	//нормализуем эту систему
	n = Normalize(n);
	u = Normalize(u);
	v = Normalize(v);

	float H = N*tan(tetha/2.0);
	float W = H*aspect;

	//установить координаты начала луча
	ray.SetStart(eye);

	//текущий уровень рекурсии
	ray.recurseLevel = 0;

	for(int row = 0; row < nRows; row += blockSize)
	{
		for(int col = 0; col < nCols; col += blockSize)
		{
			float x = -W + (col*2*W)/(float)nCols;
			float y = -H + (row*2*H)/(float)nRows;
			Vector3f direction;
			float coeff = 0.0001;	//коэффицент отклонения луча
			//начальный цвет черный
			color = Color(0, 0, 0);
			//посылаем  луч из двух точек 
			direction = Vector3f(-N*n.x + x*u.x + y*v.x + coeff,
									-N*n.y + x*u.y + y*v.y - coeff,
									-N*n.z + x*u.z + y*v.z - coeff);
			direction = Normalize(direction);
			ray.SetDir(direction);
			color.add(Shade(ray));
			direction = Vector3f(-N * n.x + x * u.x + y * v.x + coeff,
				-N * n.y + x * u.y + y * v.y + coeff,
				-N * n.z + x * u.z + y * v.z - coeff);
			direction = Normalize(direction);
			ray.SetDir(direction);
			color.add(Shade(ray));
			//вычисляем усредненный цвет
			color = Color(color.red/2., color.green/2., color.blue/2.);
			sq temp(col, row, col + blockSize, row + blockSize);
			pix t(color, temp);
			PIX.push_back(t);
		}
	}
}
Intersection Raytracer::getFirstHit(Ray ray)
{
	Intersection inter;	//запись пересечений
	Intersection best;
	best.numHits = 0;
	//цикл по всем сферам
	for(int i = 0; i < scene.vector_Sphere.size(); i++)
	{
		//если сфера должна отображаться
		if(scene.vector_Sphere[i].display)
		{
			//если нет соударения
			if(!scene.vector_Sphere[i].hit(ray, inter)) 
				continue;
			//если соударение было
			//записываем номер объекта соударения
			for(int j=0; j<inter.numHits; j++)
				inter.hit[j].objectNum = i;
			//если в best еще нет соударений или полученное соударение лучше,
			//чем соударение в best
			if(best.numHits == 0 || inter.hit[0].hitTime < best.hit[0].hitTime)
				//копируем inter в best
				best.set(inter);
		}
	}

	//цикл по всем тетраэдрам
	for(int i = 0; i < scene.vector_Tetrahedron.size(); i++)
	{
		//аналогично для тетраэдров
		if(scene.vector_Tetrahedron[i].display)
		{
			//если нет соударения
			if(!scene.vector_Tetrahedron[i].hit(ray, inter)) 
				continue;
			//если соударение было
			//записываем номер объекта соударения
			for(int j=0; j<inter.numHits; j++)
				inter.hit[j].objectNum = i;
			//если в best еще нет соударений или полученное соударение лучше,
			//чем соударение в best
			if(best.numHits == 0 || inter.hit[0].hitTime < best.hit[0].hitTime)
			//копируем inter в best
			best.set(inter);
		}
	}

	//для плоскости
	if(scene.square.hit(ray, inter))
	{
		//если в best еще нет соударений или полученное соударение лучше,
		//чем соударение в best
		if(best.numHits == 0 || inter.hit[0].hitTime < best.hit[0].hitTime)
			//копируем inter в best
			best.set(inter);
	}

	return best;
}

double getDistance(Light light, HitInfo col)//расчёт расстояния от источника света до точки фигуры
{
	double x = abs(pow((light.position.x - col.hitPoint.x), 2));
	double y = abs(pow((light.position.y - col.hitPoint.y), 2));
	double z = abs(pow((light.position.z - col.hitPoint.z), 2));
	return sqrt(x + y + z);
}

Color Raytracer::Shade(Ray ray)
{
	Color color;		//возвращаемый цвет данного луча
	Intersection best;	
	best = getFirstHit(ray);
	//копия данных о первом соударении
	HitInfo h = best.hit[0];
	//направление на наблюдателя
	Vector3f v = Vector3f(-ray.dir.x, -ray.dir.y, -ray.dir.z);
	v = Normalize(v);
	int typeObj = best.hit[0].objectType;	//тип объекта
	int numObj = best.hit[0].objectNum;		//номер объекта
	//если луч прошел мимо всех объектов, вернуть фоновый цвет
	//по умолчанию цвет - черный
	if (best.numHits == 0)
	{	
		Color buf;
		buf.red = scene.vector_Light[0].color.red - color.red;
		buf.green = scene.vector_Light[0].color.green - color.green;
		buf.blue = scene.vector_Light[0].color.blue - color.blue;
		return  scene.vector_Light[0].color;
	}
	//соударение со сферой
	if(typeObj == 0)
	{
		//установить цвет объекта
		if(scene.clsh == true)
			color = scene.vector_Sphere[numObj].color;
		//нормаль в точке соударения
		Vector3f normal;
		normal = h.hitNormal;
		normal = Normalize(normal);
		float eps = 0.0001;	
		Ray feeler;			
		feeler.start = h.hitPoint - ray.dir*eps;	
		feeler.recurseLevel = 1;					
		//цикл по всем источникам освещения
		for(int i = 0; i < scene.vector_Light.size(); i++)
		{
			//добавить фоновое освещение
			double d = getDistance(scene.vector_Light[i], h);
			d = 1 / (0.01 + 0.04 * d);
			if (scene.ambi == true)
			{
				Color ambientCol = Color(scene.vector_Sphere[numObj].Properties.Ambient[0] * scene.vector_Light[i].color.red,
					scene.vector_Sphere[numObj].Properties.Ambient[1] * scene.vector_Light[i].color.green,
					scene.vector_Sphere[numObj].Properties.Ambient[2] * scene.vector_Light[i].color.blue);
				color.add(ambientCol.red, ambientCol.green, ambientCol.blue);
			}
			//обработка тени
			feeler.dir = scene.vector_Light[i].position - h.hitPoint;	
			//если точка в тени, диффузная и зеркальная компоненты не учитываются
			if(scene.isInShadow(feeler))	continue;
			//вектор от точки соударения до источника
			Vector3f s = scene.vector_Light[i].position - h.hitPoint;
			s = Normalize(s);
			float mDotS = scal(s, normal);
			//если точка соударения повернута к свету
			if(mDotS > 0.0)
			{
				Color c = Color(mDotS*scene.vector_Sphere[numObj].Properties.Diffuse[0]*scene.vector_Light[i].color.red / d,
					mDotS*scene.vector_Sphere[numObj].Properties.Diffuse[1]*scene.vector_Light[i].color.green / d,
					mDotS*scene.vector_Sphere[numObj].Properties.Diffuse[2]*scene.vector_Light[i].color.blue / d);

				//добавить диффузную составляющую
				color.add(c.red, c.green, c.blue);
			}
			Vector3f _h = v + s;
			_h = Normalize(_h);
			float mDotH = scal(_h, normal);
			if(mDotH > 0)
			{
				float phong = pow(mDotH, scene.vector_Sphere[numObj].Properties.Shininess);
				Color specColor = Color(phong*scene.vector_Sphere[numObj].Properties.Specular[0]*scene.vector_Light[i].color.red / d,
					phong*scene.vector_Sphere[numObj].Properties.Specular[1]*scene.vector_Light[i].color.green / d,
					phong*scene.vector_Sphere[numObj].Properties.Specular[2]*scene.vector_Light[i].color.blue / d);
				//добавить зеркальную составляющую
				color.add(specColor.red, specColor.green, specColor.blue);
			}
			//если достигнут максимальный уровень рекурсии
			if(ray.recurseLevel == MAX)
				return color;
			//если объект достаточно блестящий
			if(scene.vector_Sphere[numObj].Properties.Shininess > 0.5)
			{
				Ray reflectedRay;	//отраженный луч
				Vector3f reflDir = ray.dir - normal*scalar(ray.dir, normal)*2;	//направление отраженного луча
				reflectedRay.start = h.hitPoint - ray.dir*eps;					//стартовая точка отраженного луча
				reflDir = Normalize(reflDir);
				reflectedRay.SetDir(reflDir);
				reflectedRay.recurseLevel = ray.recurseLevel + 1;				//увеличить уровень рекурсии
				Color c = Color(scene.vector_Sphere[numObj].Properties.Specular[0], scene.vector_Sphere[numObj].Properties.Specular[1], scene.vector_Sphere[numObj].Properties.Specular[2]);
				
				//добавить отраженный свет
				color.add(Shade(reflectedRay), c);
			}
		}
	}

	//соударение с тетраэдром
	if(typeObj == 1)
	{
		//установить цвет объекта
		if (scene.cltr == true)
			color = scene.vector_Tetrahedron[numObj].color;
		//нормаль в точке соударения
		Vector3f normal;
		normal = h.hitNormal;
		normal = Normalize(normal);
		float eps = 0.0001;	
		Ray feeler;			
		feeler.start = h.hitPoint - ray.dir*eps;	
		feeler.recurseLevel = 1;
		//цикл по всем источникам освещения
		for(int i = 0; i < scene.vector_Light.size(); i++)
		{
			double d = getDistance(scene.vector_Light[i], h);
			d = 1 / (0.01 + 0.04 * d);
			//добавить фоновое освещение
			if (scene.ambi == true)
			{
				Color ambientCol = Color(scene.vector_Sphere[numObj].Properties.Ambient[0] * scene.vector_Light[i].color.red,
					scene.vector_Sphere[numObj].Properties.Ambient[1] * scene.vector_Light[i].color.green,
					scene.vector_Sphere[numObj].Properties.Ambient[2] * scene.vector_Light[i].color.blue);
				color.add(ambientCol.red, ambientCol.green, ambientCol.blue);
			}
			//обработка тени
			feeler.dir = scene.vector_Light[i].position - h.hitPoint;	//направление щупа
			//если точка в тени, диффузная и зеркальная компоненты не учитываются
			if(scene.isInShadow(feeler))	continue;
			//вектор от точки соударения до источника
			Vector3f s = scene.vector_Light[i].position - h.hitPoint;
			s = Normalize(s);
			float mDotS = scal(s, normal);
			//если точка соударения повернута к свету
			if(mDotS > 0.0)
			{
				Color c = Color(mDotS*scene.vector_Tetrahedron[numObj].Properties.Diffuse[0]*scene.vector_Light[i].color.red / d,
					mDotS*scene.vector_Tetrahedron[numObj].Properties.Diffuse[1]*scene.vector_Light[i].color.green / d,
					mDotS*scene.vector_Tetrahedron[numObj].Properties.Diffuse[2]*scene.vector_Light[i].color.blue / d);
				//добавить диффузную составляющую
				color.add(c.red, c.green, c.blue);
			}
			Vector3f _h = v + s;
			_h = Normalize(_h);
			float mDotH = scal(_h, normal);
			if(mDotH > 0)
			{
				float phong = pow(mDotH, scene.vector_Tetrahedron[numObj].Properties.Shininess);
				Color specColor = Color(phong*scene.vector_Tetrahedron[numObj].Properties.Specular[0]*scene.vector_Light[i].color.red / d,
					phong*scene.vector_Tetrahedron[numObj].Properties.Specular[1]*scene.vector_Light[i].color.green / d,
					phong*scene.vector_Tetrahedron[numObj].Properties.Specular[2]*scene.vector_Light[i].color.blue / d);
				//добавить зеркальную составляющую
				color.add(specColor.red, specColor.green, specColor.blue);
			}
			//если достигнут максимальный уровень рекурсии
			if(ray.recurseLevel == MAX)
				return color;
			//_Light.color;
			//если объект достаточно блестящий
			if(scene.vector_Tetrahedron[numObj].Properties.Shininess > 0.5)
			{
				Ray reflectedRay;
				Vector3f reflDir = ray.dir - normal*scalar(ray.dir, normal)*2.0;//направление отраженного луча
				reflectedRay.start = h.hitPoint - ray.dir*eps;					//стартовая точка отраженного луча
				reflDir = Normalize(reflDir);
				reflectedRay.SetDir(reflDir);
				reflectedRay.recurseLevel = ray.recurseLevel + 1;
				Color c = Color(scene.vector_Tetrahedron[numObj].Properties.Specular[0], scene.vector_Tetrahedron[numObj].Properties.Specular[1], scene.vector_Tetrahedron[numObj].Properties.Specular[2]);
				//добавить отраженную составляющую
				color.add(Shade(reflectedRay), c);
			}
		}
	}

	//соударение с плоскостью
	if(typeObj == 10)
	{
		//установить цвет объекта
		color.red = scene.square.color.red;
		color.green = scene.square.color.green;
		color.blue = scene.square.color.blue;
		//нормаль в точке соударения
		Vector3f normal;
		normal = h.hitNormal;
		normal = Normalize(normal);
		float eps = 0.0001;	
		Ray feeler;			
		feeler.start = h.hitPoint - ray.dir*eps;	
		feeler.recurseLevel = 1;
		//цикл по всем источникам освещения
		for(int i = 0; i < scene.vector_Light.size(); i++)
		{
			double d = getDistance(scene.vector_Light[i], h);
			//добавить фоновое освещение
			Color ambientCol = Color(scene.square.Properties.Ambient[0]*scene.vector_Light[i].color.red,
								scene.square.Properties.Ambient[1]*scene.vector_Light[i].color.green,
								scene.square.Properties.Ambient[2]*scene.vector_Light[i].color.blue);
			color.add(ambientCol.red, ambientCol.green, ambientCol.blue);
			//обработка тени
			feeler.dir = scene.vector_Light[i].position - h.hitPoint;	
			//если точка в тени, диффузная и зеркальная компоненты не учитываются
			if(scene.isInShadow(feeler))	continue;
			//вектор от точки соударения до источника
			Vector3f s = scene.vector_Light[i].position - h.hitPoint;
			s = Normalize(s);
			//член Ламберта
			float mDotS = scal(s, normal);
			//если точка соударения повернута к свету
			if(mDotS > 0.0)
			{
				Color с = Color(mDotS*scene.square.Properties.Diffuse[0]*scene.vector_Light[i].color.red / d,
					mDotS*scene.square.Properties.Diffuse[1]*scene.vector_Light[i].color.green / d,
					mDotS*scene.square.Properties.Diffuse[2]*scene.vector_Light[i].color.blue / d);
				//добавить диффузную составляющую
				color.add(с.red, с.green, с.blue);
			}
			Vector3f _h = v + s;
			_h = Normalize(_h);
			float mDotH = scal(_h, normal);
			if (mDotH <= 0) continue;
			float phong = pow(mDotH, scene.square.Properties.Shininess);
			Color specColor = Color(phong*scene.square.Properties.Specular[0]*scene.vector_Light[i].color.red / d,
				phong*scene.square.Properties.Specular[1]*scene.vector_Light[i].color.green / d,
				phong*scene.square.Properties.Specular[2]*scene.vector_Light[i].color.blue / d);
			//добавить зеркальную составляющую
			color.add(specColor.red, specColor.green, specColor.blue);
			//если достигнут максимальный уровень рекурсии
			if(ray.recurseLevel == MAX)
				return color;
			//если объект достаточно блестящий
			if(scene.square.Properties.Shininess > 0.5)
			{
				Ray reflectedRay;
				Vector3f reflDir = ray.dir - normal*scalar(ray.dir, normal)*2;
				reflectedRay.start = h.hitPoint - ray.dir*eps;
				reflDir = Normalize(reflDir);
				reflectedRay.SetDir(reflDir);
				reflectedRay.recurseLevel = ray.recurseLevel + 1;
				Color c = Color(scene.square.Properties.Specular[0], scene.square.Properties.Specular[1], scene.square.Properties.Specular[2]);
				//добавить отраженную составляющую
				color.add(Shade(reflectedRay), c);
			}
		}
	}
	return color;
}