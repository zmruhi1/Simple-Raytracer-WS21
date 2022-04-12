#include <rt/solids/aabox.h>

namespace rt {

AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material)
{
    /* TODO */
	this->setCoordMapper(texMapper);
	this->setMaterial(material);
	this->mCorner1 = corner1;
	this->mCorner2 = corner2;

	Point minCorner = min(mCorner1, mCorner2);
	Point maxCorner = max(mCorner1, mCorner2);

	mBBox = BBox(minCorner, maxCorner);
}

BBox AABox::getBounds() const {
    /* TODO */ 
	/* implemented above */
	return mBBox;
}

Solid::Sample AABox::sample() const {
    NOT_IMPLEMENTED;
}

float AABox::getArea() const {
    /* TODO */ 
	float a = abs(mCorner1.x - mCorner2.x);
	float b = abs(mCorner1.y - mCorner2.y);
	float c = abs(mCorner1.z - mCorner2.z);

	return 2 * (a * b + b * c + c * a);
}

Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const {
    /* TODO */ 
	Point minCorner = min(mCorner1, mCorner2);
	Point maxCorner = max(mCorner1, mCorner2);


	if (ray.d.x == 0)
		if (ray.o.x < minCorner.x || ray.o.x > maxCorner.x) return Intersection::failure();

	if (ray.d.y == 0)
		if (ray.o.y < minCorner.y || ray.o.y > maxCorner.y) return Intersection::failure();

	if (ray.d.z == 0)
		if (ray.o.z < minCorner.z || ray.o.z > maxCorner.z) return Intersection::failure();

	float t0x = (mCorner1.x - ray.o.x) / ray.d.x;
	float t1x = (mCorner2.x - ray.o.x) / ray.d.x;
	if (t0x > t1x) std::swap(t0x, t1x);

	float t0y = (mCorner1.y - ray.o.y) / ray.d.y;
	float t1y = (mCorner2.y - ray.o.y) / ray.d.y;
	if (t0y > t1y) std::swap(t0y, t1y);

	float t0z = (mCorner1.z - ray.o.z) / ray.d.z;
	float t1z = (mCorner2.z - ray.o.z) / ray.d.z;
	if (t0z > t1z) std::swap(t0z, t1z);

	float maxt0 = max(max(t0x, t0y), t0z);
	Vector normal0;
	if (maxt0 == t0x) normal0 = Vector(-1, 0, 0);
	if (maxt0 == t0y) normal0 = Vector(0, -1, 0);
	if (maxt0 == t0z) normal0 = Vector(0, 0, -1);

	Vector normal1;
	float mint1 = min(min(t1x, t1y), t1z);
	if (mint1 == t1x) normal1 = Vector(1, 0, 0);
	if (mint1 == t1y) normal1 = Vector(0, 1, 0);
	if (mint1 == t1z) normal1 = Vector(0, 0, 1);


	if (maxt0 >= mint1) return Intersection::failure();


	bool t1Valid = maxt0 <= previousBestDistance && maxt0 >= 0;
	bool t2Valid = mint1 <= previousBestDistance && mint1 >= 0;

	if (!t1Valid && !t2Valid) return Intersection::failure();

	float t;
	Vector normal;

	if (t1Valid && t2Valid)
	{
		t = min(maxt0, mint1);
		if (t == mint1) normal = normal1;
		else normal = normal0;
	}

	if (!t1Valid && t2Valid)
	{
		t = mint1;
		normal = normal1;
	}

	if (t1Valid && !t2Valid)
	{
		t = maxt0;
		normal = normal0;
	}

	Point surfacePoint = ray.getPoint(t);
	return Intersection(t, ray, this, normal, surfacePoint);
}

}
