// material.cpp

#include "material.h"

std::string Material::getName() {
		return name;
}

short int Material::getAMass() {
		return atomicMass;
}

float Material::getDensity() {
		return density;
}

float Material::getADensity() {
		return atomicDensity;
}

float Material::getXS(EventType et, const Particle& incP) {
		return xstable.findRecord(et, incP).xs;
}

std::array<float, 3> Material::getEventXSs(const Particle& incP) {
		std::array<float, 3> xss;
		std::array<XSRecord, 3> records = xstable.findEventRecords(incP);
		xss[0] = records[0].xs;
		xss[1] = records[1].xs;
		xss[2] = records[2].xs;
		return xss;
}

std::array<XSRecord, 3> Material::getEventRecords(const Particle& incP) {
		return xstable.findEventRecords(incP);
}
