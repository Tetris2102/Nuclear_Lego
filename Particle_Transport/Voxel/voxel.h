#ifndef VOXEL_H
#define VOXEL_H

// TODO: maybe add void processParticleGroup(ParticleGroup* pg); for managing
// large fluxes, i.e. treat one particle as many having the same parameters

#include "../Material/material.h"
#include "../IsotopeSample/isotopeSample.h"
#include "../Vector3/vector3.h"
#include <array>
#include <vector>
#include <random>

enum VoxelType {
    SOURCE,
    MATTER,
    DETECTOR
};

// IsotopeSample for stable voxels (MATTER or DETECTOR)
IsotopeSample stable();

// Reference value of elastic scattering angle at 1 MeV in radians
// unit: radians * sqrt(MeV)
#define REF_THETA 0.349  // approx. 10 degrees * sqrt(MeV)
#define TWO_PI 6.283

// Voxels are treated as spaces for cubes, not actual objects,
// so that they remain when cube's position or material is changed
class Voxel {
    private:
        VoxelType type;
        double halfSide;  // sidelength / 2
        Vector3 position;
        Material material;
        IsotopeSample sample = stable;
        std::vector<Particle> particlesAbsorbed;  // For DETECTOR Voxels

        // Probability of any interaction happening
        double getTotalIntProb(const Particle& p, double travelDist);
        EventType chooseEventAndXS(const Particle& p);
        double chooseIntDistAlong(const Particle& p, double xs,
          double tmin, double tmax);
        Vector3 getScatterMomentum(const Vector3& oldMom, double energy);

        // RNG for
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> uniform_real_dist(0.0, 1.0);
    public:

        Voxel(double _side, Vector3 _position, const Material& _m
          const IsotopeSample& _s=stable) : material(_m), sample(_s) {
            halfSide = _side / 2;
            position = _position;
            isotopeSample = _isotopeSample;
        }

        // Manage reactions and their probabilities for a particle
        // Returns vector of particles created (if any)
        std::vector<Particle> processParticle(Particle& p);
        bool intersects(const Particle& p);
        std::array<double, 2> intersectParams(const Particle& p);  // returns [tmin, tmax]
        Vector3 getPosition();
        void setMaterial(const Material& m);
        Material getMaterial();
        std::vector<Particle> getParticlesAbsorbed();
        int getParticlesAbsorbed();
        std::vector<Particle> getAndEraseParticlesAbsorbed();
        int getAndEraseParticlesAbsorbed();
};

#endif
