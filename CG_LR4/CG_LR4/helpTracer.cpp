#include"helpTracer.h"

//описание методов класса Ray
Ray::Ray()
{

}
void Ray::SetStart(Vector3f _start)
{
	start = Vector3f(_start.x, _start.y, _start.z);
}
void Ray::SetDir(Vector3f _dir)
{
	dir = Vector3f(_dir.x, _dir.y, _dir.z);
}

//описание методов класса HitInfo
HitInfo::HitInfo()
{
	objectType = -1;
	objectNum = -1;
	hitTime = -1000;
	surface = -1;
	isEntering = false;
}
void HitInfo::set(HitInfo hI)
{
	hitTime = hI.hitTime;
	objectType = hI.objectType;
	objectNum = hI.objectNum;
	surface = hI.surface;
	hitPoint = Vector3f(hI.hitPoint.x, hI.hitPoint.y, hI.hitPoint.z);
	hitNormal = Vector3f(hI.hitNormal.x, hI.hitNormal.y, hI.hitNormal.z);
}

//описание методов класса Intersection
Intersection::Intersection()
{
	numHits = 0;
}
void Intersection::set(Intersection intr)
{
	numHits = intr.numHits;
	for(int i = 0; i < maxNumHits; i++)
		hit[i].set(intr.hit[i]);
}