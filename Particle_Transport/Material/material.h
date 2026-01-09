// material.h

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../ParticleGroup/particleGroup.h"
#include "../XSTable/xstable.h"
#include <string>
#include <array>

const float AVOGADRO = 6.022e23;

// Define material (non-radioactive)
class Material {
    private:
        std::string name;
        short int atomicMass;
        float density;
        float atomicDensity;  // atoms/cm^3
        XSTable xstable;
    public:
        Material(std::string _name, short int _atomicMass, float _density,
          const XSTable& _xstable) : xstable(_xstable) {
            name = _name;
            atomicMass = _atomicMass;
            density = _density;
            atomicDensity = (_density / _atomicMass) * AVOGADRO;
        }

        std::string getName();
        short int getAMass();
        float getDensity();
        float getADensity();
        float getXS(EventType et, const ParticleGroup& incP);
        std::array<float, 3> getEventXSs(const ParticleGroup& incP);
        std::array<XSRecord, 3> getEventRecords(const ParticleGroup& incP);
};

#endif
