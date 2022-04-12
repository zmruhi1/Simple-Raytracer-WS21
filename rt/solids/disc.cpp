#include <rt/solids/disc.h>

namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
{
    /* TODO */
	this->mOrigin = center;
	this->mNormal = normal.normalize();
	setMaterial(material);
	setCoordMapper(texMapper);
	this->mRadius = radius;

	float x = sqrt(mNormal.y * mNormal.y + mNormal.z * mNormal.z);
	float y = sqrt(mNormal.x * mNormal.x + mNormal.z * mNormal.z);
	float z = sqrt(mNormal.y * mNormal.y + mNormal.x * mNormal.x);

	Vector e = Vector(x, y, z);
	Point min = mOrigin - mRadius * e;
	Point max = mOrigin + mRadius * e;

	mBBox = BBox(min, max);
}

BBox Disc::getBounds() const {
    /* TODO */ 
	/* implemented above */
	return mBBox;
}

Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {
    /* TODO */ 
	float denom = dot(ray.d, this->mNormal);
	if (denom == 0.0) return Intersection::failure(); 

	float t = -dot(ray.o - mOrigin, mNormal) / denom;

	if (t > previousBestDistance || t < 0) return Intersection::failure();

	Point surfacePoint = ray.getPoint(t);

	if ((surfacePoint - mOrigin).length() > mRadius)
		return Intersection::failure();

	return Intersection(t, ray, this, mNormal, surfacePoint);
}

Solid::Sample Disc::sample() const {
    NOT_IMPLEMENTED;
}

float Disc::getArea() const {
	return pi * mRadius * mRadius;
}

}
