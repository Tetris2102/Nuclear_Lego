#ifndef VOXEL_H
#define VOXEL_H

// TODO: maybe add void processParticleGroup(ParticleGroup* pg); for managing
// large fluxes, i.e. treat one particle as many having the same parameters

#include "../Material/material.h"
#include <array>
#include <vector>

enum VoxelType {
    SOURCE,
    MATTER,
    DETECTOR
};

// Voxels are treated as spaces for cubes, not actual objects,
// so that they remain when cube's position or material is changed
class Voxel {
    private:
        VoxelType type;
        double halfSide;  // sidelength / 2
        std::array<double, 3> position;
        Material material;
    public:

        // For MATTER andf DETECTOR
        Voxel(double side, std::array<double, 3> position,
            const Material& m) : material(m) {
            this->halfSide = side / 2;
            this->position = position;
        }

        // For SOURCE
        Voxel(double side, std::array<double, 3> position,
            const IsotopeSample& i) : IsotopeSample(i) {
            this->halfSide = side / 2;
            this->position = position;
        }

        // Manage reactions and their probabilities for a particle
        // Returns a vector of particles created (if any)
        std::vector<Particle> processParticle(Particle& p);
        bool intersects(Particle p);
        std::array<double, 2> intersectParams(Particle p);  // returns [tmin, tmax]
        std::array<double, 3> getPosition();
        void setMaterial(Material m);
        Material getMaterial();
};

#endif
