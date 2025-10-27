// voxel.h

#ifndef VOXEL_H
#define VOXEL_H

// TODO: maybe add void processParticleGroup(ParticleGroup* pg); for managing
// large fluxes, i.e. treat one particle as many having the same parameters

#include "../Material/material.h"
#include "../IsotopeSample/isotopeSample.h"
#include "../Vector3/vector3.h"
// #include <utility>
#include <array>
#include <vector>
#include <cassert>
// #include <random>
// #include <mutex>

enum VoxelType {
    SOURCE,
    MATTER,
    DETECTOR
};

// Reference value of elastic scattering angle at 1 MeV in radians
// unit: radians * sqrt(MeV)
#define REF_THETA 0.349  // approx. 10 degrees * sqrt(MeV)

// Voxels are treated both as spaces for phyical
// material and physical objects (see ../World/world.cpp)
class Voxel {
    private:
        VoxelType type;
        Vector3 position;
        Material material;
        IsotopeSample sample;
        std::vector<Particle> particlesAbsorbed;  // For DETECTOR Voxels

        // Probability of any interaction happening
        float getTotalIntProb(const Particle& p, float travelDist);
        XSRecord chooseEventAndXS(const Particle& p, std::mt19937& gen);
        float chooseIntDistAlong(float xs, float tmin, float tmax,
          std::uniform_real_distribution<float>& dist, std::mt19937& gen);
        Vector3 getScatterMomentum(const Vector3& oldMom, float energy,
          std::uniform_real_distribution<float>& dist, std::mt19937& gen);

        // RNG pointers
        // std::random_device rd;
        // std::mt19937 gen;
        // Distribution should be in (0, 1) range
        // std::uniform_real_distribution<float> uniform_real_dist;
        // mutable std::mutex voxelMutex;
    public:

        // For MATTER and DETECTOR
        Voxel(VoxelType _type, const Material& _m) :
          material(_m), sample{} {
            assert(_type != SOURCE);
            type = _type;
        }

        // For SOURCE
        Voxel(VoxelType _type, const Material& _m,
          const IsotopeSample& _s) :
          material(_m), sample(_s) {
            assert(_type == SOURCE);
            type = _type;
        }

        // Manage reactions and their probabilities for a particle
        // Returns vector of particles created (if any)
        std::vector<Particle> processParticle(Particle& p, float voxelHalfSide,
          std::uniform_real_distribution<float>& dist, std::mt19937& gen);

        // For multithreading
        std::vector<Particle> processParticleThreadSafe(Particle& p,
          float voxelHalfSide, std::uniform_real_distribution<float>& dist,
          std::mt19937& gen);

        void moveToExit(Particle& p, float voxelHalfSide) const;
        bool intersects(const Particle& p, float voxelHalfSide) const;
        std::array<float, 2> intersectParams(const Particle& p,
          float voxelHalfSide) const;  // returns [tmin, tmax]
        VoxelType getType() const;
        void setPosition(const Vector3& newPosition);
        Vector3 getPosition() const;
        void setMaterial(const Material& newMat);
        Material getMaterial();
        std::vector<Particle> getPartsEmittedList(float timeElapsed,
          std::uniform_real_distribution<float>& dist, std::mt19937& gen);
        int getPartsEmitted(float time,
          std::uniform_real_distribution<float>& dist, std::mt19937& gen);
        std::vector<Particle> getPartsAbsorbedList();
        int getPartsAbsorbed();
        void clearPartsAbsorbed();
};

#endif
