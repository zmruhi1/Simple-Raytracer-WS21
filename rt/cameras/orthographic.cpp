#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {

	OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY)
	{
		c = center;
		f = forward;
		u = up;
		sX = scaleX;
		sY = scaleY;

		ZAxis = f.normalize();
		XAxis = cross(ZAxis, u.normalize()).normalize();
		YAxis = cross(XAxis, ZAxis).normalize();
	}

	Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
		Point newCenter = c + (x * sX / 2.0f * XAxis + y * sY / 2.0f * YAxis);
		return Ray(newCenter, f);
	}

}
