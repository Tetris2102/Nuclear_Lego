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

double Material::getADensity() {
		return atomicDensity;
}

double Material::getXS(EventType et, const Particle& incP) {
		return xstable.findRecord(et, incP).xs;
}

std::array<double, 3> Material::getEventXSs(const Particle& incP) {
		std::array<double, 3> xss;
		std::array<XSRecord, 3> records = xstable.findEventRecords(incP);
		for(int i = 0; i<3; i++) {
				xss[i] = records[i].xs;
		}
		return xss;
}

std::array<XSRecord, 3> Material::getEventRecord(const Particle& incP) {
		return xstable.findEventRecords(incP);
}
