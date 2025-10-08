// material.h

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../Particle/particle.h"
#include "../XSTable/xstable.h"
#include <string>
#include <array>

const double AVOGADRO = 6.022e23;

// Define material (non-radioactive)
class Material {
    private:
        std::string name;
        short int atomicMass;
        double density;
        double atomicDensity;  // atoms/cm^3
        XSTable xstable;
    public:
        Material(std::string _name, short int _atomicMass, double _density,
          const XSTable& _xstable) : xstable(_xstable) {
            name = _name;
            atomicMass = _atomicMass;
            density = _density;
            atomicDensity = (_density / _atomicMass) * AVOGADRO;
        }

        std::string getName();
        short int getAMass();
        double getDensity();
        double getADensity();
        double getXS(EventType et, const Particle& incP);
        std::array<double, 3> getEventXSs(const Particle& incP);
        std::array<XSRecord, 3> getEventRecords(const Particle& incP);
};

#endif
