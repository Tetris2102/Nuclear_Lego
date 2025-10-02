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

void Material::addXSTable(XSTable table) {
		XSTables.push_back(table);
}

double Material::getXS(Particle incParticle) {
		// Check if table is empty
		if(XSTables.size() == 0) return 0.0;

		bool isFound = false;
		int i = 0;
		ParticleType type = incParticle.getType();
		while(!isFound) {
				// Check if particle type matches
				if(XSTables[i].getIncParticleType() == type) {
						return XSTables[i].getXS(incParticle.getEnergy());
				}

				// Increment i if there are more elements
				if(i < XSTables.size()) {
						i++;
				} else {
						break;
				}
		}
		return 0.0;
}
