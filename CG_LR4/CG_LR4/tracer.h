//������������ �����

#include"scene.h"


const int MAX = 4;		//������������ ������� ��������

struct sq
{
	int x1;
	int y1; 
	int x2;
	int y2;

	sq(int _1, int _2, int _3, int _4)
	{
		x1 = _1;
		y1 = _2;
		x2 = _3;
		y2 = _4;
	}
	sq(){};
};

struct pix
{
	Color color;
	sq p;

	pix(Color c, sq p_)
	{
		color = c;
		p = p_;
	}

	pix() {};
};

extern vector<pix> PIX;

//������������ �����
class Raytracer
{
public:

	Vector3f eye;	//����� ������ �����/��������� ������/
	Scene scene;	//�����
	Camera camera;	//������
	float aspect;
	int blockSize;	//������ ����� ��������
	int nCols;
	int nRows;

	Raytracer();	//����������� �� ���������
	Raytracer(Vector3f _eye, Scene _scene, Camera _camera, float _aspect, int _nCols, int _nRows, int _blockSize);	//�����������
	void draw();
	void Raytrace();		//�����������
	Color Shade(Ray ray);	//���� �����������
	Intersection getFirstHit(Ray ray);	//������� ������, � ������� ��� ����������� ������
};