// material.h

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Particle/particle.h"
#include "../XSTable/xstable.h"
#include <string>
#include <vector>

// Define material (non-radioactive)
class Material {
    private:
        std::string name;
        short int atomicMass;
        double density;
        XSTable xstable;
    public:
        Material(std::string name, short int atomicMass, double density,
          XSTable xstable) : xstable(xstable) {
            this->name = name;
            this->atomicMass = atomicMass;
            this->density = density;
        }

        std::string getName();
        short int getAMass();
        double getDensity();
        double getXS(EventType et, Particle incP);
};

#endif
