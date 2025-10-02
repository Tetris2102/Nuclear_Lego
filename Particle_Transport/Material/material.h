// material.h

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../XSTable/xstable.h"
#include <string>

// Define material (non-radioactive)
class Material {
    private:
        std::string name;
        short int atomicMass;
        double density;

        std::vector<XSTable> XSTables;
    public:
        Material(std::string name, short int atomicMass, double density) {
            this->name = name;
            this->atomicMass = atomicMass;
            this->density = density;
        }

        std::string getName();
        short int getAMass();
        double getDensity();
        void addXSTable(XSTable table);
        double getXS(Particle incParticle);
};

#endif
