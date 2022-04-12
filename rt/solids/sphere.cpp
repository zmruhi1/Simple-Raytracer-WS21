#include <rt/solids/sphere.h>

namespace rt {

Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material)
{
    /* TODO */
	this->material = material;
	this->texMapper = texMapper;
	this->mCenter = center;
	this->mR = radius;

	Point min = mCenter - Vector(mR, mR, mR);
	Point max = mCenter + Vector(mR, mR, mR);

	mBBox = BBox(min, max);
}

BBox Sphere::getBounds() const {
    /* TODO */ 
	/* implemented above*/
	return mBBox;
}

Intersection Sphere::intersect(const Ray& ray, float previousBestDistance) const {
	Vector origCenterDist = ray.o - mCenter;
	float a = dot(ray.d, ray.d);
	float b = 2 * dot(ray.d, origCenterDist);
	float c = dot(origCenterDist, origCenterDist) - mR * mR;

	float t0, t1;
	bool isInstersect = solveQuardaticEquation(a, b, c, t0, t1);
	if (isInstersect && (t0 < previousBestDistance)) {
		Point hitPnt = ray.getPoint(t0);
		Vector sphereNormal = (hitPnt - mCenter).normalize();
		Intersection sphereIntersection(t0, ray, this, sphereNormal, hitPnt);

		return sphereIntersection;
	}
	
	return Intersection::failure();
}

Solid::Sample Sphere::sample() const {
	NOT_IMPLEMENTED;
}

float Sphere::getArea() const {
    /* TODO */ 
	return 4 * pi * mR * mR;
}

bool Sphere::solveQuardaticEquation(float a, float b, float c, float &t0, float &t1) const {
	float disc = b * b - 4 * a * c;
	if (disc < 0) return false;
	else if (disc == 0) {
		t0 = -b / (2 * a);
		t1 = t0;
	} else {
		float q = (b > 0) ? -(b + sqrt(disc)) / 2 : -(b - sqrt(disc)) / 2;
		t0 = q / a;
		t1 = c / q;
	}
	
	if (t0 > t1) {
		std::swap(t0, t1);
	}

	return true;
}

}
