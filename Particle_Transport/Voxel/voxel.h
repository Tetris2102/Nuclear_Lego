// voxel.h

#ifndef VOXEL_H
#define VOXEL_H

// TODO: maybe add void processParticleGroup(ParticleGroup* pg); for managing
// large fluxes, i.e. treat one particle as many having the same parameters

#include "../Material/material.h"
#include "../IsotopeSample/isotopeSample.h"
#include "../Vector3/vector3.h"
#include <array>
#include <vector>
#include <cassert>
#include <mutex>
#include <atomic>
#include <utility>

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
        Material material;
        IsotopeSample sample;
        // std::vector<Particle> particlesAbsorbed;  // For DETECTOR Voxels

        // Probability of any interaction happening
        float getTotalIntProb(const ParticleGroup& pg, float travelDist);
        XSRecord chooseEventAndXS(const ParticleGroup& pg, std::mt19937& gen);
        float chooseIntDistAlong(float xs, float tmin, float tmax,
          std::uniform_real_distribution<float>& dist, std::mt19937& gen);
        Vector3 getScatterMomentum(const Vector3& oldMom, float energy,
          std::uniform_real_distribution<float>& dist, std::mt19937& gen);

        mutable std::mutex voxelMutex;
        // std::atomic<int> nAlphaAbsorbed;
        // std::atomic<int> nBetaAbsorbed;
        // std::atomic<int> nGammaAbsorbed;
        //
        // std::vector<ParticleType> detects = {ALPHA, BETA, GAMMA};

        // std::atomic<int> nPartsAbsorbed{0};
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
        // Returns std::pair of vector of particles created + vector
        // of particles absorbed (if any)
        std::pair<std::vector<ParticleGroup>, std::vector<ParticleGroup>> processParticle(
          ParticleGroup& pg, const Vector3& voxelPosition, float voxelHalfSide,
          std::uniform_real_distribution<float>& dist, std::mt19937& gen);

        // For multithreading, output the same as from processParticle()
        // std::pair<std::vector<Particle>, std::vector<Particle>> processParticleThreadSafe(
        //   Particle& p, const Vector3& voxelPosition, float voxelHalfSide,
        //   std::uniform_real_distribution<float>& dist, std::mt19937& gen);

        void moveToExit(ParticleGroup& pg, float voxelHalfSide,
          const Vector3& voxelPos) const;
        bool intersects(const ParticleGroup& pg, float voxelHalfSide,
          const Vector3& position) const;
        std::array<float, 2> intersectParams(const ParticleGroup& pg,
          float voxelHalfSide, const Vector3& position) const;  // returns [tmin, tmax]
        VoxelType getType() const;
        // void setPosition(const Vector3& newPosition);
        // Vector3 getPosition() const;
        void setMaterial(const Material& newMat);
        Material getMaterial();
        std::vector<ParticleGroup> getPartsEmittedList(float timeElapsed,
          const Vector3& position, std::uniform_real_distribution<float>& dist,
          std::mt19937& gen);
        // Get mutex by reference
        std::mutex& getMtxRef();

    // Instrumentation helpers (global across all voxels)
    struct VoxelStats {
      int passes;
      int scatters;
      int absorbs;
      int created;
    };

    static VoxelStats getGlobalStats();
    static void resetGlobalStats();
        // int getPartsEmitted(float time,
        //   std::uniform_real_distribution<float>& dist, std::mt19937& gen);
        // std::vector<Particle> getPartsAbsorbedList();
        // int getPartsAbsorbed();
        // void clearPartsAbsorbed();
};

#endif
