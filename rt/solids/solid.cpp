#include <rt/solids/solid.h>

namespace rt {

Solid::Solid(CoordMapper* texMapper, Material* material)
{
    /* TODO */
	this->setCoordMapper(texMapper);
	this->setMaterial(material);
}

}
