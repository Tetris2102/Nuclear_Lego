// material.cpp

#include "material.h"

std::string Material::getName() {
		return name;
}

short int Material::getAMass() {
		return atomicMass;
}

double Material::getDensity() {
		return density;
}

double Material::getXS(EventType et, Particle incP) {
		return xstable.findRecord(et, incP).xs;
}
