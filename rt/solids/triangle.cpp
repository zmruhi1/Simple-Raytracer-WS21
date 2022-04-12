#include <rt/solids/triangle.h>

namespace rt {

Triangle::Triangle(Point vertices[3], CoordMapper* texMapper, Material* material)
{
    /* TODO */
    setCoordMapper(texMapper);
    setMaterial(material);
    v1 = vertices[0];
    v2 = vertices[1];
    v3 = vertices[2];

    float minx = min(v1.x, v2.x, v3.x);
    float miny = min(v1.y, v2.y, v3.y);
    float minz = min(v1.z, v2.z, v3.z);

    float maxx = max(v1.x, v2.x, v3.x);
    float maxy = max(v1.y, v2.y, v3.y);
    float maxz = max(v1.z, v2.z, v3.z);

    Point bmin = Point(minx, miny, minz);
    Point bmax = Point(maxx, maxy, maxz);

    mBBox = BBox(bmin, bmax); 
}

Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material)
{
    setCoordMapper(texMapper);
    setMaterial(material);
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;

    mVect1 = (v2 - v1).normalize();
    mVect2 = (v3 - v1).normalize();
    mNormal = cross(mVect1, mVect2).normalize();

    float minx = min(v1.x, v2.x, v3.x);
    float miny = min(v1.y, v2.y, v3.y);
    float minz = min(v1.z, v2.z, v3.z);

    float maxx = max(v1.x, v2.x, v3.x);
    float maxy = max(v1.y, v2.y, v3.y);
    float maxz = max(v1.z, v2.z, v3.z);

    Point bmin = Point(minx, miny, minz);
    Point bmax = Point(maxx, maxy, maxz);

    mBBox = BBox(bmin, bmax);
}

BBox Triangle::getBounds() const {
    /* TODO */ 
    /* implemented above already*/
    return mBBox; 
} 

Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {
    Vector v1v2 = v2 - v1;
    Vector v1v3 = v3 - v1;
    Vector planeNormal = cross(v1v2, v1v3);
    float doubleTriangleArea = planeNormal.length();

    float dotProductRayNormal = dot(ray.d, planeNormal);
    if (dotProductRayNormal == 0) {
        return Intersection::failure();
    }


    float denom = dot(ray.d, planeNormal.normalize());
    if (fabsf(denom) <= epsilon)
        return Intersection::failure();
    float t = -dot(ray.o - v1, planeNormal.normalize()) / denom;
    if (t < epsilon) return Intersection::failure();
    
    Point hitPoint = ray.getPoint(t);

    Vector v2v3 = v3 - v2;
    Vector v2P = hitPoint - v2;
    float doubleAreaSubTriangle1 = cross(v2P, v2v3).length();
    float u = doubleAreaSubTriangle1 / doubleTriangleArea;

    Vector v3P = hitPoint - v3;
    float doubleAreaSubTriangle2 = cross(v3P, v1v3).length();
    float v = doubleAreaSubTriangle2 / doubleTriangleArea;

    float w = 1 - u - v;

    if ((u >= 0 && u <= 1) && (v >= 0 && v <= 1) && (w >= 0 && w <= 1) && (t < previousBestDistance)) {
        return Intersection (t, ray, this, planeNormal, hitPoint);
    }

    return Intersection::failure();
}

Solid::Sample Triangle::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float Triangle::getArea() const {
    /* TODO */ 
    Vector norm = cross(v2 - v1, v3 - v1);
    return norm.length() / 2;
}

}