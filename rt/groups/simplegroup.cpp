#include <rt/groups/simplegroup.h>

namespace rt {

BBox SimpleGroup::getBounds() const {
	return mBBox;
}

Intersection SimpleGroup::intersect( const Ray& ray, float previousBestDistance) const {
	/* TODO */
	Intersection isec;

	bool anyIntersection = false;
	for (std::vector<Primitive*>::const_iterator it = mPrimitives.begin(); it != mPrimitives.end(); ++it)
	{
		Primitive* p = *it;
		Intersection newIsec = p->intersect(ray, previousBestDistance);

		if (newIsec)
		{
			previousBestDistance = newIsec.distance;
			isec = newIsec;
			anyIntersection = true;
		}
	}

	if (anyIntersection) return isec;
	return Intersection::failure();
}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
	mPrimitives.push_back(p);
	mBBox.extend(p->getBounds());
}


void SimpleGroup::setMaterial(Material* m) {
    /* TODO */ 
	this->setMaterial(m);
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    /* TODO */ 
	this->setCoordMapper(cm);
}

}
