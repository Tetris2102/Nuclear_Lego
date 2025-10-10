// voxel.h

#ifndef VOXEL_H
#define VOXEL_H

// TODO: maybe add void processParticleGroup(ParticleGroup* pg); for managing
// large fluxes, i.e. treat one particle as many having the same parameters

#include "../Material/material.h"
#include "../IsotopeSample/isotopeSample.h"
#include "../Vector3/vector3.h"
#include <utility>
#include <array>
#include <vector>
#include <cassert>
#include <random>

enum VoxelType {
    SOURCE,
    MATTER,
    DETECTOR
};

// Reference value of elastic scattering angle at 1 MeV in radians
// unit: radians * sqrt(MeV)
#define REF_THETA 0.349  // approx. 10 degrees * sqrt(MeV)
#define TWO_PI 6.283

// Voxels are treated both as spaces for phyical
// material and physical objects (see ../World/world.cpp)
class Voxel {
    private:
        VoxelType type;
        float halfSide;  // sidelength / 2
        Vector3 position;
        Material material;
        IsotopeSample sample;
        std::vector<Particle> particlesAbsorbed;  // For DETECTOR Voxels

        // Probability of any interaction happening
        float getTotalIntProb(const Particle& p, float travelDist);
        XSRecord chooseEventAndXS(const Particle& p);
        float chooseIntDistAlong(float xs, float tmin, float tmax);
        Vector3 getScatterMomentum(const Vector3& oldMom, float energy);

        // RNG pointers
        // std::random_device rd;
        std::mt19937* gen_ptr = nullptr;
        // Distribution should be in (0, 1) range
        std::uniform_real_distribution<float>* uniform_real_dist_ptr = nullptr;
    public:

        // For MATTER and DETECTOR
        Voxel(float _side, VoxelType _type, Vector3 _position,
          const Material& _m)
          : material(_m), sample{} {
            halfSide = _side / 2;
            assert(_type != SOURCE);
            type = _type;
            position = _position;
        }

        // For SOURCE
        Voxel(float _side, VoxelType _type, Vector3 _position,
          const Material& _m, const IsotopeSample& _s) :
          material(_m), sample(_s) {
            halfSide = _side / 2;
            assert(_type == SOURCE);
            type = _type;
            position = _position;
        }

        // Manage reactions and their probabilities for a particle
        // Returns vector of particles created (if any)
        void setRNG(std::uniform_real_distribution<float>& dist,
          std::mt19937& gen);
        std::vector<Particle> processParticle(Particle& p);
        void moveToExit(Particle& p) const;
        bool intersects(const Particle& p);
        std::array<float, 2> intersectParams(const Particle& p) const;  // returns [tmin, tmax]
        VoxelType getType() const;
        void setPosition(const Vector3& newPos);
        Vector3 getPosition();
        void setMaterial(const Material& newMat);
        Material getMaterial();
        std::vector<Particle> getPartsEmittedList(float time);
        int getPartsEmitted(float time);
        std::vector<Particle> getPartsAbsorbedList();
        int getPartsAbsorbed();
        void clearPartsAbsorbed();
};

#endif
