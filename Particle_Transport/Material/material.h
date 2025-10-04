// material.h

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Particle/particle.h"
#include "../XSTable/xstable.h"
#include "../IsotopeSample/isotopeSample.h"
#include "../Data/data.h"
#include <string>
#include <vector>
#include <array>

// Define material (non-radioactive)
class Material {
    private:
        const float AVOGADRO = 6.022e23;
        std::string name;
        short int atomicMass;
        double density;
        double atomicDensity;  // atoms/cm^3
        XSTable xstable;
        IsotopeSample isotopeSample = IsotopeSample{"notRadioactive", NONE, 0.0};
    public:
        Material(std::string name, short int atomicMass, double density,
          XSTable xstable) : xstable(xstable) {
            this->name = name;
            this->atomicMass = atomicMass;
            this->density = density;
            this->atomicDensity = (density / atomicMass) * AVOGADRO;
        }

        std::string getName();
        short int getAMass();
        double getDensity();
        double getADensity();
        double getXS(EventType et, const Particle& incP);
        std::array<double, 3> getEventXSs(const Particle& incP);
        std::array<XSRecord, 3> getEventRecord(const Particle& incP);
};

#endif
