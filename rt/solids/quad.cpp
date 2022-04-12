#include <rt/solids/quad.h>

namespace rt {

Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material)
{
    /* TODO */
    setMaterial(material);
    setCoordMapper(texMapper);
    this->v1 = origin;
    this->mSpan1 = span1;
    this->mSpan2 = span2;
    mNormal = cross(mSpan1, mSpan2).normalize();

    v2 = v1 + mSpan1;
    v3 = v1 + mSpan1 + mSpan2;
    v4 = v1 + mSpan2;

    mCenter = v1 + (0.5 * mSpan1) + (0.5 * mSpan2);

    float minx = min(v1.x, v2.x, min(v3.x, v4.x));
    float miny = min(v1.y, v2.y, min(v3.y, v4.y));
    float minz = min(v1.z, v2.z, min(v3.z, v4.z));

    float maxx = max(v1.x, v2.x, max(v3.x, v4.x));
    float maxy = max(v1.y, v2.y, max(v3.y, v4.y));
    float maxz = max(v1.z, v2.z, max(v3.z, v4.z));

    Point bmin = Point(minx, miny, minz);
    Point bmax = Point(maxx, maxy, maxz);

    mBBox = BBox(bmin, bmax);
}

BBox Quad::getBounds() const {
    /* TODO */
    /* implemented above*/
    return mBBox;
}

Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {
    /* TODO */ 
    float denominator = dot(ray.d, this->mNormal);
    if (denominator == 0.0) return Intersection::failure(); // no solution

    float t = dot(mCenter - ray.o, mNormal) / denominator;

    if (t > previousBestDistance || t < 0.001) return Intersection::failure();

    Point surfacePoint = ray.getPoint(t);

    bool test1 = dot(cross(v2 - v1, surfacePoint - v1), mNormal) >= 0;
    bool test2 = dot(cross(v3 - v2, surfacePoint - v2), mNormal) >= 0;
    bool test3 = dot(cross(v4 - v3, surfacePoint - v3), mNormal) >= 0;
    bool test4 = dot(cross(v1 - v4, surfacePoint - v4), mNormal) >= 0;

    if (test1 && test2 && test3 && test4)
        return Intersection(t, ray, this, mNormal, surfacePoint);
    else
        return Intersection::failure();
}

Solid::Sample Quad::sample() const {
    /* TODO */  NOT_IMPLEMENTED;
}

float Quad::getArea() const {
    /* TODO */
    Vector norm = cross(mSpan1, mSpan2);
    return norm.length();
}

}
