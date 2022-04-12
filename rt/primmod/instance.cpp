#include <rt/primmod/instance.h>

namespace rt {

Instance::Instance(Primitive* content)
{
    /* TODO */
    reset();
    mPrimitive = content;
}

Primitive* Instance::content() {
    /* TODO */ 
    return mPrimitive;
}

void Instance::reset() {
    /* TODO */ 
    mStack = Matrix::identity();
    mStackInverse = mStack.invert();
}

void Instance::translate(const Vector& t) {
    /* TODO */ 
	Float4 v1(1, 0, 0, t.x);
	Float4 v2(0, 1, 0, t.y);
	Float4 v3(0, 0, 1, t.z);
	Float4 v4(0, 0, 0, 1);

	Matrix translationMatrix(v1, v2, v3, v4);
	stackMatrices(translationMatrix);

}

void Instance::rotate(const Vector& nnaxis, float angle) {
    /* TODO */ 
	Vector r;
	Vector s;

	float axMin = min(abs(nnaxis.x), min(abs(nnaxis.y), abs(nnaxis.z)));
	Matrix transMatrix2 = Matrix::identity();
	Matrix rotMatrix;

	if (abs(nnaxis.x) == axMin)
	{
		r = Vector(0, nnaxis.y, nnaxis.z).normalize();
		s = Vector(0, -nnaxis.z, nnaxis.y).normalize();

		float axMin2 = min(abs(r.y), abs(r.z));

		if (abs(r.y) == axMin2) 
		{
			Vector r2(0, 0, r.z);
			r2 = r2.normalize();
			Vector s2(0, -r.z, 0);
			s2 = s2.normalize();

			Vector t2 = cross(r2, s2).normalize();
			transMatrix2 = Matrix::system(r2, s2, t2);

			rotMatrix = getRotZMatrix(angle);

		}
		else	
		{
			Vector r2(0, r.y, 0);
			r2 = r2.normalize();
			Vector s2(-r.y, 0, 0);
			s2 = s2.normalize();

			Vector t2 = cross(r2, s2).normalize();
			transMatrix2 = Matrix::system(r2, s2, t2);

			rotMatrix = getRotYMatrix(angle);
		}



	}

	if (abs(nnaxis.y) == axMin)
	{
		r = Vector(nnaxis.x, 0, nnaxis.z).normalize();
		s = Vector(-nnaxis.z, 0, nnaxis.x).normalize();

		float axMin2 = min(abs(r.x), abs(r.z));

		if (abs(r.x) == axMin2) 
		{
			Vector r2(0, 0, r.z);
			r2 = r2.normalize();
			Vector s2(0, -r.z, 0);
			s2 = s2.normalize();

			Vector t2 = cross(r2, s2).normalize();
			transMatrix2 = Matrix::system(r2, s2, t2);

			rotMatrix = getRotZMatrix(angle);
				
		}
		else	
		{
			Vector r2(r.x, 0, 0);
			r2 = r2.normalize();
			Vector s2(0, -r.x, 0);
			s2 = s2.normalize();

			Vector t2 = cross(r2, s2).normalize();
			transMatrix2 = Matrix::system(r2, s2, t2);

			rotMatrix = getRotXMatrix(angle);
		}
		}
	if (abs(nnaxis.z) == axMin)
	{
		r = Vector(nnaxis.x, nnaxis.y, 0).normalize();
		s = Vector(-nnaxis.y, nnaxis.x, 0).normalize();

		float axMin2 = min(abs(r.x), abs(r.y));

		if (abs(r.x) == axMin2) 
		{
			Vector r2(0, r.y, 0);
			r2 = r2.normalize();
			Vector s2(-r.y, 0, 0);
			s2 = s2.normalize();

			Vector t2 = cross(r2, s2).normalize();
			transMatrix2 = Matrix::system(r2, s2, t2);

			rotMatrix = getRotYMatrix(angle);

		}
		else	
		{
			Vector r2(r.x, 0, 0);
			r2 = r2.normalize();
			Vector s2(0, -r.x, 0);
			s2 = s2.normalize();

			Vector t2 = cross(r2, s2).normalize();
			transMatrix2 = Matrix::system(r2, s2, t2);

			rotMatrix = getRotXMatrix(angle);
		}
	}

	r = r.normalize();
	s = s.normalize();

	Vector t = cross(r, s).normalize();

	Matrix transMatrix = Matrix::system(r, s, t);

	Matrix rotationMatrix = product(transMatrix.invert(), transMatrix2.invert());
	rotationMatrix = product(rotationMatrix, rotMatrix);
	rotationMatrix = product(rotationMatrix, transMatrix2);
	rotationMatrix = product(rotationMatrix, transMatrix);
	stackMatrices(rotationMatrix);
}

void Instance::scale(float f) {
    /* TODO */ 
	Vector sc(f, f, f);
	this->scale(sc);
}

void Instance::scale(const Vector& s) {
    /* TODO */ 
	Float4 sc1(s.x, 0, 0, 0);
	Float4 sc2(0, s.y, 0, 0);
	Float4 sc3(0, 0, s.z, 0);
	Float4 sc4(0, 0, 0, 1);

	Matrix scaleMatrix(sc1, sc2, sc3, sc4);
	stackMatrices(scaleMatrix);
}

void Instance::setMaterial(Material* m) {
    /* TODO */ NOT_IMPLEMENTED;
}

void Instance::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const {
    /* TODO */ 
	Matrix inverseStack = mStack.invert();
	Float4 o4 = mStackInverse * Float4(ray.o);
	Float4 d4 = mStackInverse * Float4(ray.d);

	Point o3(o4);
	Vector d3(d4);

	Ray newRay(o3, d3.normalize());

	float normalizationFactor = d3.length() / ray.d.length();

	Intersection isect = mPrimitive->intersect(newRay);

	if (isect)
	{
		float dist = isect.distance / normalizationFactor;

		if (dist > 0 && dist < previousBestDistance)
		{
			Point hitPoint = ray.getPoint(dist);

			const Solid* returnPrimitive = isect.solid;
			Point returnPoint = hitPoint;
			return Intersection(dist, ray, returnPrimitive, (mStackInverse.transpose() * isect.normal()).normalize(), returnPoint);
		}
		else
			return Intersection::failure();
	}

	return Intersection::failure();
}

BBox Instance::getBounds() const {
    /* TODO */ 
    return mBBox;
}


Matrix Instance::getRotXMatrix(float angle)
{
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	Float4 rot1(1, 0, 0, 0);
	Float4 rot2(0, cosTheta, -sinTheta, 0);
	Float4 rot3(0, sinTheta, cosTheta, 0);
	Float4 rot4(0, 0, 0, 1);

	return Matrix(rot1, rot2, rot3, rot4);
}

Matrix Instance::getRotYMatrix(float angle)
{
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	Float4 rot1(cosTheta, 0, sinTheta, 0);
	Float4 rot2(0, 1, 0, 0);
	Float4 rot3(-sinTheta, 0, cosTheta, 0);
	Float4 rot4(0, 0, 0, 1);

	return Matrix(rot1, rot2, rot3, rot4);
}

Matrix Instance::getRotZMatrix(float angle)
{
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	Float4 rot1(cosTheta, -sinTheta, 0, 0);
	Float4 rot2(sinTheta, cosTheta, 0, 0);
	Float4 rot3(0, 0, 1, 0);
	Float4 rot4(0, 0, 0, 1);

	return Matrix(rot1, rot2, rot3, rot4);
}


void Instance::newBBox()  /* applying transformation to the bbox */
{
	BBox bbox = mPrimitive->getBounds();


	Float4 points[8];

	points[0] = Float4(bbox.min);
	points[1] = Float4(bbox.min.x, bbox.min.y, bbox.max.z, 1);
	points[2] = Float4(bbox.min.x, bbox.max.y, bbox.min.z, 1);
	points[3] = Float4(bbox.min.x, bbox.max.y, bbox.max.z, 1);
	points[4] = Float4(bbox.max.x, bbox.min.y, bbox.min.z, 1);
	points[5] = Float4(bbox.max.x, bbox.min.y, bbox.max.z, 1);
	points[6] = Float4(bbox.max.x, bbox.max.y, bbox.min.z, 1);
	points[7] = Float4(bbox.max.x, bbox.max.y, bbox.max.z, 1);

	Matrix stackInverse = mStack.invert();

	for (int i = 0; i < 8; ++i)
		points[i] = stackInverse * points[i];

	Point min = Point(points[0]);
	Point max = Point(points[0]);

	for (int i = 0; i < 8; ++i)
	{
		if (min.x > points[i].x) min.x = points[i].x;
		if (min.y > points[i].y) min.y = points[i].y;
		if (min.z > points[i].z) min.z = points[i].z;

		if (max.x < points[i].x) max.x = points[i].x;
		if (max.y < points[i].y) max.y = points[i].y;
		if (max.z < points[i].z) max.z = points[i].z;
	}


	mBBox = BBox(min, max);

}

void Instance::stackMatrices(Matrix& m)
{
    mStack = product(m, mStack);
    mStackInverse = mStack.invert();
    newBBox();
}

}