#include <rt/cameras/perspective.h>
#include <cmath>

namespace rt {


PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle)
{
	c = center;
	f = forward;
	u = up;
	v = verticalOpeningAngle;
	h = horizontalOpeningAngle;
	
	ZAxis = f.normalize();
	XAxis = cross(ZAxis, u.normalize()).normalize();
	YAxis = cross(XAxis, ZAxis).normalize();
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
	Vector vec = ZAxis + YAxis * y * tan(v / 2.0) + XAxis * x * tan(h / 2.0);
	vec = vec.normalize();
	Ray r = Ray(c, vec);

	return r;

}

}