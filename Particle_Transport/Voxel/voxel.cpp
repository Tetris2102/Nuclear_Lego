// voxel.cpp

#include "voxel.h"
#include "../Helpers/helpers.cpp"
#include "../ParticleGroup/particleGroup.h"
#include <algorithm>
#include <cmath>
// #include <iostream>
#include <atomic>

float Voxel::getTotalIntProb(const ParticleGroup& p, float travelDist) {
    std::array<float, 3> xss = material.getEventXSs(p);
    float macroXSA = material.getADensity() * xss[0];
    float macroXSS = material.getADensity() * xss[1];
    float macroXSR = material.getADensity() * xss[2];

    float totMacroXS = macroXSA + macroXSS + macroXSR;
    return 1.0 - std::exp(-totMacroXS * travelDist);
}

XSRecord Voxel::chooseEventAndXS(const ParticleGroup& p, std::mt19937& gen) {
    std::array<XSRecord, 3> records = material.getEventRecords(p);
    std::array<float, 3> xss = {records[0].xs, records[1].xs, records[2].xs};
    std::discrete_distribution<int> discrete_dist{xss.begin(), xss.end()};
    XSRecord recordChosen = records[discrete_dist(gen)];
    return recordChosen;
}

float Voxel::chooseIntDistAlong(float xs, float tmin, float tmax,
  std::uniform_real_distribution<float>& dist, std::mt19937& gen) {
    float u = dist(gen);
    float sigma = material.getADensity() * xs;  // Macroscopic cross-section
    // Use cumulative distribution function, i.e. probability of interaction
    // increases along particle path
    return tmin - (1/sigma) * std::log(1 - u*(1-std::exp(-sigma*(tmax-tmin))));
}

Vector3 Voxel::getScatterMomentum(const Vector3& oldMom, float energy,
  std::uniform_real_distribution<float>& dist, std::mt19937& gen) {
    float theta_max = REF_THETA / std::sqrt(energy);
    float theta = dist(gen) * theta_max;
    float phi = dist(gen) * 6.283;  // Multiply by 2*pi
    Vector3 randOrthVec = oldMom.randomOrthogonalV3(dist(gen),
      dist(gen), dist(gen));
    Vector3 scatterMom = (oldMom * std::cos(theta) +
      randOrthVec.cross(oldMom) * std::sin(theta));
    scatterMom.normalize();
    return scatterMom;
}

// Global instrumentation counters (updated atomically)
static std::atomic<int> g_passes{0};
static std::atomic<int> g_scatters{0};
static std::atomic<int> g_absorbs{0};
static std::atomic<int> g_created{0};

Voxel::VoxelStats Voxel::getGlobalStats() {
    return Voxel::VoxelStats{
        static_cast<int>(g_passes.load(std::memory_order_relaxed)),
        static_cast<int>(g_scatters.load(std::memory_order_relaxed)),
        static_cast<int>(g_absorbs.load(std::memory_order_relaxed)),
        static_cast<int>(g_created.load(std::memory_order_relaxed))
    };
}

void Voxel::resetGlobalStats() {
    g_passes.store(0, std::memory_order_relaxed);
    g_scatters.store(0, std::memory_order_relaxed);
    g_absorbs.store(0, std::memory_order_relaxed);
    g_created.store(0, std::memory_order_relaxed);
}

std::pair<std::vector<ParticleGroup>, std::vector<ParticleGroup>> Voxel::processParticleGroup(
  ParticleGroup& p, const Vector3& voxelPos, float voxelHalfSide,
  std::uniform_real_distribution<float>& dist, std::mt19937& gen) {
    // Check if particle passes through Voxel
    if(!intersects(p, voxelHalfSide, voxelPos)) {
            // particle does not intersect this voxel
            ::g_passes.fetch_add(1, std::memory_order_relaxed);
            return {{}, {}};
    }
    // Discard deactivated particles
    if(!p.isActive()) return {{}, {}};

    // Check if particle will undergo any event
    float tmin = intersectParams(p, voxelHalfSide, voxelPos)[0];
    float tmax = intersectParams(p, voxelHalfSide, voxelPos)[1];
    float prob = getTotalIntProb(p, tmax-tmin);
    if(dist(gen) > prob) {
        // Same as moveToExit(p);
        p.moveToPointAlong(tmax);  // ParticleGroup just passes through
        ::g_passes.fetch_add(1, std::memory_order_relaxed);
        return {{}, {}};
    }

    // Choose record to use (including EventType, cross-section, etc.)
    XSRecord record = chooseEventAndXS(p, gen);

    std::vector<ParticleGroup> particlesCreated;
    std::vector<ParticleGroup> particlesAbsorbed;

    if(record.event == SCATTER) {
        float intDistAlong = chooseIntDistAlong(record.xs, tmin, tmax, dist, gen);
        p.moveToPointAlong(intDistAlong);
        p.setMomentum(getScatterMomentum(p.getMomentum(), p.getEnergy(), dist, gen));
        float tmaxScatter = intersectParams(p, voxelHalfSide, voxelPos)[1];
        p.moveToPointAlong(tmaxScatter); // Advance particle to exit of Voxel
        ::g_scatters.fetch_add(1, std::memory_order_relaxed);
    } else if(record.event == ABSORB) {
        // No need in two lines below since particle is deactivated anyway
        // float t = getIntDistAlong(record.xs, tmin, tmax);
        // p.moveToPointAlong(t);

        // Detect particle if applicable
        if(type == DETECTOR) {
            particlesAbsorbed.push_back(p);
        }
        ::g_absorbs.fetch_add(1, std::memory_order_relaxed);
        p.deactivate();
    } else {
        float particleEnergy = p.getEnergy() / record.finalParticleGroupCount;
        ParticleGroup newP;
        float intDistAlong = chooseIntDistAlong(record.xs, tmin, tmax, dist, gen);

        // Iterate to create particles
        for(int i = 0; i<record.finalParticleGroupCount; i++) {
            newP.setType(record.finalParticleGroup);
            newP.moveToPointAlong(intDistAlong);
            newP.setMomentum(getScatterMomentum(p.getMomentum(), p.getEnergy(), dist, gen));
            particlesCreated.push_back(newP);
        }

        ::g_created.fetch_add(record.finalParticleGroupCount, std::memory_order_relaxed);

        // Detect particle if applicable
        if(type == DETECTOR) {
            particlesAbsorbed.push_back(p);
        }
        p.deactivate();  // Incident particle absorbed
    }

    return std::pair(particlesCreated, particlesAbsorbed);
}

// std::pair<std::vector<ParticleGroup>, std::vector<ParticleGroup>> Voxel::processParticleGroupThreadSafe(ParticleGroup& p,
//   const Vector3& voxelPos, float voxelHalfSide,
//   std::uniform_real_distribution<float>& dist, std::mt19937& gen) {
//     std::lock_guard<std::mutex> lock(voxelMutex);
//     return processParticleGroup(p, voxelPos, voxelHalfSide, dist, gen);
// }

void Voxel::moveToExit(ParticleGroup& p, float voxelHalfSide,
  const Vector3& voxelPos) const {
    // How far the point of exit is from position along momentum Vector3
    float exitAlongMom = intersectParams(p, voxelHalfSide, voxelPos)[1];
    p.moveToPointAlong(exitAlongMom);
}

bool Voxel::intersects(const ParticleGroup& p, float voxelHalfSide,
  const Vector3& position) const {
    float xmin, ymin, zmin;
    xmin = position.x - voxelHalfSide;
    ymin = position.y - voxelHalfSide;
    zmin = position.z - voxelHalfSide;

    float xmax, ymax, zmax;
    xmax = position.x + voxelHalfSide;
    ymax = position.y + voxelHalfSide;
    zmax = position.z + voxelHalfSide;

    float x = p.getX();
    float y = p.getY();
    float z = p.getZ();

    float dx = p.getMomX();
    float dy = p.getMomY();
    float dz = p.getMomZ();

    // Normalize magnitude
    normalizeVecXYZ(dx, dy, dz);

    if(dx == 0) {
        if(x <= xmin || x >= xmax) {
            return false;
        } else {
            return true;
        }
    }
    if(dy == 0) {
        if(y <= ymin || y >= ymax) {
            return false;
        } else {
            return true;
        }
    }
    if(dz == 0) {
        if(z <= zmin || z >= zmax) {
            return false;
        } else {
            return true;
        }
    }

    float t0x, t1x, t0y, t1y, t0z, t1z;

    if(dx > 0) {
        t0x = (xmin - x) / dx;
        t1x = (xmax - x) / dx;
    } else {
        // Swap t0x and t1x
        t1x = (xmin - x) / dx;
        t0x = (xmax - x) / dx;
    }

    if(dy > 0) {
        t0y = (ymin - y) / dy;
        t1y = (ymax - y) / dy;
    } else {
        // Swap t0y and t1y
        t1y = (ymin - y) / dy;
        t0y = (ymax - y) / dy;
    }

    if(dz > 0) {
        t0z = (zmin - z) / dz;
        t1z = (zmax - z) / dz;
    } else {
        // Swap t0z and t1z
        t1z = (zmin - z) / dz;
        t0z = (zmax - z) / dz;
    }

    float tmin = std::max({t0x, t0y, t0z, 0.0f});
    float tmax = std::min({t1x, t1y, t1z});

    return (tmax > tmin) ? true : false;
}

std::array<float, 2> Voxel::intersectParams(const ParticleGroup& p,
  float voxelHalfSide, const Vector3& position) const {
    float xmin, ymin, zmin;
    xmin = position.x - voxelHalfSide;
    ymin = position.y - voxelHalfSide;
    zmin = position.z - voxelHalfSide;

    float xmax, ymax, zmax;
    xmax = position.x + voxelHalfSide;
    ymax = position.y + voxelHalfSide;
    zmax = position.z + voxelHalfSide;

    float x = p.getX();
    float y = p.getY();
    float z = p.getZ();

    float dx = p.getMomX();
    float dy = p.getMomY();
    float dz = p.getMomZ();

    // Normalize magnitude
    normalizeVecXYZ(dx, dy, dz);

    bool ignoreX, ignoreY, ignoreZ;
    ignoreX = (dx == 0);
    ignoreY = (dy == 0);
    ignoreZ = (dz == 0);

    float t0x, t1x, t0y, t1y, t0z, t1z;

    if(dx > 0) {
        t0x = (xmin - x) / dx;
        t1x = (xmax - x) / dx;
    } else {
        // Swap t0x and t1x
        t1x = (xmin - x) / dx;
        t0x = (xmax - x) / dx;
    }

    if(dy > 0) {
        t0y = (ymin - y) / dy;
        t1y = (ymax - y) / dy;
    } else {
        // Swap t0y and t1y
        t1y = (ymin - y) / dy;
        t0y = (ymax - y) / dy;
    }

    if(dz > 0) {
        t0z = (zmin - z) / dz;
        t1z = (zmax - z) / dz;
    } else {
        // Swap t0z and t1z
        t1z = (zmin - z) / dz;
        t0z = (zmax - z) / dz;
    }

    std::vector<float> tMinArgs;
    std::vector<float> tMaxArgs;
    if(!ignoreX) {
        tMinArgs.push_back(t0x);
        tMaxArgs.push_back(t1x);
    }
    if(!ignoreY) {
        tMinArgs.push_back(t0y);
        tMaxArgs.push_back(t1y);
    }
    if(!ignoreZ) {
        tMinArgs.push_back(t0z);
        tMaxArgs.push_back(t1z);
    }

    auto tminIt = std::max_element(tMinArgs.begin(), tMinArgs.end());
    auto tmaxIt = std::min_element(tMaxArgs.begin(), tMaxArgs.end());

    float tmin = std::max(*tminIt, 0.0f);
    float tmax = *tmaxIt;

    return {tmin, tmax};
}

VoxelType Voxel::getType() const {
    return type;
}

// void Voxel::setPosition(const Vector3& newPosition) {
//     position = newPosition;
// }

// Vector3 Voxel::getPosition() const {
//     return position;
// }

void Voxel::setMaterial(const Material& newMat) {
    material = newMat;
}

Material Voxel::getMaterial() {
    return material;
}

std::vector<ParticleGroup> Voxel::getPartsEmittedList(
  float timeElapsed, const Vector3& position, float partGroupSize,
  std::uniform_real_distribution<float>& dist, std::mt19937& gen) {
    assert(type == SOURCE);
    return sample.generateParticleGroups(
      timeElapsed, position,
      partGroupSize, dist, gen
    );
}

std::mutex& Voxel::getMtxRef() {
    return voxelMutex;
}

// int Voxel::getPartsEmitted(float time,
//   std::uniform_real_distribution<float>& dist, std::mt19937& gen) {
//     assert(type == SOURCE);
//     return sample.generateParticleGroups(time, Vector3{0.0, 0.0, 0.0},
//       dist, gen).size();
// }
//
// std::vector<ParticleGroup> Voxel::getPartsAbsorbedList() {
//     assert(type == DETECTOR);
//     return particlesAbsorbed;
// }

// int Voxel::getPartsAbsorbed() {
//     assert(type == DETECTOR);
//     return particlesAbsorbed.size();
// }

// void Voxel::clearPartsAbsorbed() {
//     assert(type == DETECTOR);
//     particlesAbsorbed = {};
// }
