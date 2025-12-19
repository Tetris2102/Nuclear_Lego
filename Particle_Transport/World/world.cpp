// world.cpp

#include "world.h"
#include "../config.h"
#include <cmath>
#include <limits>
#include <array>
#include <cassert>
#include <algorithm>  // std::clamp is used (C++17)
#include <utility>
#include <functional>  // for std::ref()

// dir has to be normalized
Vector3 pointAlongVec3(const Vector3& origin, const Vector3& dir, float t) {
    float x = origin.x + t * dir.x;
    float y = origin.y + t * dir.y;
    float z = origin.z + t * dir.z;

    return Vector3{x, y, z};
}

void World::processParticlesRange(int thrIdx, size_t startIdx, size_t endIdx,
  std::vector<Particle>& currentParticles, uint32_t seed) {

    std::mt19937 threadGen(seed);
    std::uniform_real_distribution<float> threadDist(0, 1);

    for(size_t i = startIdx; i<endIdx; i++) {
        Particle& p = currentParticles[i];
        if(!p.isActive()) continue;
        auto vPos = nextVoxelPosVec(p);
                auto* vEntryPtr = voxelEntryAtPos(vPos);

                if(!vEntryPtr) {
                        // Particle has left the world or produced NaN/Inf position; deactivate
                        p.deactivate();
                        continue;
                }

                auto results = vEntryPtr->vPtr->processParticleThreadSafe(p, vPos,
                    voxelHalfSide, threadDist, threadGen);

        if(!results.first.empty()) {
            // append produced particles for this thread instead of overwriting
            auto &out = threadsNewParticles[thrIdx];
            out.insert(out.end(), results.first.begin(), results.first.end());
        }
        if(!results.second.empty()) {
            vEntryPtr->addPartsAbsorbed(results.second);
        }
    }
}

// For float, not short int position
Vector3 World::getVoxelCoordVec3(int index) {
    int x = index % sizeX;
    int y = (index / sizeX) % sizeY;
    int z = index / (sizeX * sizeY);

    return Vector3{
        (x+1) * voxelHalfSide,
        (y+1) * voxelHalfSide,
        (z+1) * voxelHalfSide
    };
}

Vector3 World::getVoxelCoordVec3(short int x, short int y, short int z) {
    return Vector3{
        (x+1) * voxelHalfSide,
        (y+1) * voxelHalfSide,
        (z+1) * voxelHalfSide
    };
}

std::array<short int, 3> World::getVoxelPos(int index) {
    return {
        static_cast<short int>(std::round(index % sizeX)),
        static_cast<short int>(std::round((index / sizeX) % sizeY)),
        static_cast<short int>(std::round(index / (sizeX * sizeY)))
    };
}

void World::updateLists() {
    sources = {};
    matters = {};
    detectors = {};

    for(size_t i = 0; i<scene.size(); i++) {
        VoxelEntry* entry_ptr = &scene[i];
        VoxelType type = entry_ptr->vPtr->getType();
        if(type == SOURCE) {
            sources.push_back(entry_ptr);
        } else if (type == MATTER) {
            matters.push_back(entry_ptr);
        } else {
            detectors.push_back(entry_ptr);
        }
    }
}

// Voxel* World::nextVoxel(Particle& p) {
//     Vector3 pos = p.getPosition();
//     Vector3 dir = p.getMomentum();
//     short int x = std::floor(pos.x / voxelSide);
//     short int y = std::floor(pos.y / voxelSide);
//     short int z = std::floor(pos.z / voxelSide);
//
//     float nextBoundaryX, nextBoundaryY, nextBoundaryZ;
//
//     // Distance along the ray to closest planes along itself
//     if(dir.x != 0) {
//         nextBoundaryX = (((dir.x > 0) ? (x+1) : x) * voxelSide - pos.x) / dir.x;
//     } else {
//         nextBoundaryX = sizeX * voxelSide + 1; // Effectively infinity
//     }
//
//     if(dir.y != 0) {
//         nextBoundaryY = (((dir.y > 0) ? (y+1) : y) * voxelSide - pos.y) / dir.y;
//     } else {
//         nextBoundaryY = sizeY * voxelSide + 1; // Effectively infinity
//     }
//
//     if(dir.z != 0) {
//         nextBoundaryZ = (((dir.z > 0) ? (z+1) : z) * voxelSide - pos.z) / dir.z;
//     } else {
//         nextBoundaryZ = sizeZ * voxelSide + 1; // Effectively infinity
//     }
//
//     float closestDistAlong = std::min({
//       nextBoundaryX, nextBoundaryY, nextBoundaryZ});
//
//     Vector3 point = pointAlongVec3(
//       p.getPosition(), p.getMomentum(), closestDistAlong);
//
//     x = std::floor(point.x / voxelSide);
//     y = std::floor(point.y / voxelSide);
//     z = std::floor(point.z / voxelSide);
//
//     // Check if particle is out of bonds of the world
//     if(x < 0 || x > sizeX - 1) {
//         p.deactivate();
//         return scene[0];
//     }
//     if(y < 0 || y > sizeY - 1) {
//         p.deactivate();
//         return scene[0];
//     }
//     if(z < 0 || z > sizeZ - 1) {
//         p.deactivate();
//         return scene[0];
//     }
//
//     return voxelAt(x, y, z);  // Convert short int indices into
//                               // float absolute positions
// }

Vector3 World::nextVoxelPosVec(Particle& p) {
    Vector3 pos = p.getPosition();
    Vector3 dir = p.getMomentum();
    short int x = std::floor(pos.x / voxelSide);
    short int y = std::floor(pos.y / voxelSide);
    short int z = std::floor(pos.z / voxelSide);

    float nextBoundaryX, nextBoundaryY, nextBoundaryZ;

    // Distance along the ray to closest planes along itself
    if(dir.x != 0) {
        nextBoundaryX = (((dir.x > 0) ? (x+1) : x) * voxelSide - pos.x) / dir.x;
    } else {
        nextBoundaryX = sizeX * voxelSide + 1; // Effectively infinity
    }

    if(dir.y != 0) {
        nextBoundaryY = (((dir.y > 0) ? (y+1) : y) * voxelSide - pos.y) / dir.y;
    } else {
        nextBoundaryY = sizeY * voxelSide + 1; // Effectively infinity
    }

    if(dir.z != 0) {
        nextBoundaryZ = (((dir.z > 0) ? (z+1) : z) * voxelSide - pos.z) / dir.z;
    } else {
        nextBoundaryZ = sizeZ * voxelSide + 1; // Effectively infinity
    }

    float closestDistAlong = std::min({
      nextBoundaryX, nextBoundaryY, nextBoundaryZ});

    Vector3 point = pointAlongVec3(
      p.getPosition(), p.getMomentum(), closestDistAlong);

    x = std::floor(point.x / voxelSide);
    y = std::floor(point.y / voxelSide);
    z = std::floor(point.z / voxelSide);

    // Check if particle is out of bonds of the world
    if(x < 0 || x > sizeX - 1) {
        p.deactivate();
        return Vector3(std::numeric_limits<float>::quiet_NaN(),
                       std::numeric_limits<float>::quiet_NaN(),
                       std::numeric_limits<float>::quiet_NaN());
    }
    if(y < 0 || y > sizeY - 1) {
        p.deactivate();
        return Vector3(std::numeric_limits<float>::quiet_NaN(),
                       std::numeric_limits<float>::quiet_NaN(),
                       std::numeric_limits<float>::quiet_NaN());
    }
    if(z < 0 || z > sizeZ - 1) {
        p.deactivate();
        return Vector3(std::numeric_limits<float>::quiet_NaN(),
                       std::numeric_limits<float>::quiet_NaN(),
                       std::numeric_limits<float>::quiet_NaN());
    }

    // Convert integer voxel indices into world-space coordinates (voxel center)
    return getVoxelCoordVec3(x, y, z);
}

void World::addParticlesEmitted(float time) {
    // vEntPtr = voxelEntryPointer
    for(auto* vEntPtr : sources) {
        Vector3 voxelPosVec = getVoxelCoordVec3(vEntPtr->x, vEntPtr->y, vEntPtr->z);
        auto particlesEmitted = vEntPtr->vPtr->getPartsEmittedList(
          time, voxelPosVec, dist, gen);
        for(auto& p : particlesEmitted) vEntPtr->vPtr->moveToExit(
          p, voxelHalfSide, voxelPosVec);
        particles.insert(particles.end(), particlesEmitted.begin(),
          particlesEmitted.end());
    }
}

void World::cleanParticles(std::vector<ParticleGroup>& partList) {
    // Maximum scene dimensions
    float maxX = voxelSide * sizeX;
    float maxY = voxelSide * sizeY;
    float maxZ = voxelSide * sizeZ;

    auto toRemove = [&] (const Particle& p) {
        bool xEscaped = (p.getX() < 0 || p.getX() >= maxX);
        bool yEscaped = (p.getY() < 0 || p.getY() >= maxY);
        bool zEscaped = (p.getZ() < 0 || p.getZ() >= maxZ);

        return (!p.isActive() || xEscaped || yEscaped || zEscaped);
    };

    partList.erase(
      std::remove_if(partList.begin(), partList.end(), toRemove),
      partList.end()
    );
}

// Voxels in scene are individually numbered from 0, first along x-axis,
// then in rows along y-axis, then in layers along z-axis
size_t World::indexAt(short int x, short int y, short int z) const {
    return static_cast<size_t>(x + sizeX * (y + z * sizeY));
}

VoxelEntry* World::voxelEntryAt(short int x, short int y, short int z) {
    // Defensive: return nullptr for out-of-bounds instead of asserting here.
    // Callers should check the return value and handle invalid positions.
    if(!(x >= 0 && x < sizeX && y >= 0 && y < sizeY && z >= 0 && z < sizeZ)) {
        return nullptr;
    }
    return &scene[indexAt(x, y, z)];
}

Voxel* World::voxelAt(short int x, short int y, short int z) {
    // Use voxelEntryAt which already performs bounds checking and may return nullptr.
    VoxelEntry* e = voxelEntryAt(x, y, z);
    return e ? e->vPtr : nullptr;
}

VoxelEntry* World::voxelEntryAtPos(const Vector3& pos) {
    // Defensive mapping from continuous position to voxel indices.
    // Protect against NaN/Inf and out-of-bounds positions.
    if(!std::isfinite(pos.x) || !std::isfinite(pos.y) || !std::isfinite(pos.z)) {
        return nullptr;
    }

    // Use floor(pos / voxelSide) to map a world position to voxel index.
    // This assumes the world origin (0,0,0) corresponds to the minimum corner
    // of voxel (0,0,0). If your voxel centers are offset, adjust accordingly.
    int ix = static_cast<int>(std::floor(pos.x / voxelSide));
    int iy = static_cast<int>(std::floor(pos.y / voxelSide));
    int iz = static_cast<int>(std::floor(pos.z / voxelSide));

    if(ix < 0 || ix >= sizeX || iy < 0 || iy >= sizeY || iz < 0 || iz >= sizeZ) {
        return nullptr;
    }

    return voxelEntryAt(static_cast<short int>(ix),
                        static_cast<short int>(iy),
                        static_cast<short int>(iz));
}

Voxel& World::voxelAtPos(const Vector3& pos) {
    // Prefer callers to use voxelEntryAtPos and check for nullptr.
    // Keep this helper but make it defensive: assert in debug, and fall back
    // to the first voxel to avoid UB in release builds.
    VoxelEntry* e = voxelEntryAtPos(pos);
    if(!e) {
        assert(false && "voxelAtPos called with invalid/out-of-bounds position");
        return *scene[0].vPtr; // safe fallback (shouldn't be relied upon)
    }
    return *e->vPtr;
}

// void World::simulate(float time) {
//     addParticlesEmitted(time);
//
//     for(auto& p : particles) {
//         nextVoxel(p).processParticle(p, voxelHalfSide);
//     }
//     cleanParticles();
// }

void World::simulate(float time) {
    addParticlesEmitted(time);

    if(particles.size() < 500 || !MULTITHREAD_ON) {
                        // Single-threaded processing: collect newly created particles and
                        // record absorbed particles so behavior matches multithreaded path.
                        std::vector<Particle> singleThreadNewParticles;
                        for(auto& p : particles) {
                                auto voxelPos = nextVoxelPosVec(p);
                                auto* vEntry = voxelEntryAtPos(voxelPos);
                                if(!vEntry) {
                                        // out-of-bounds or invalid position
                                        continue;
                                }
                                auto results = vEntry->vPtr->processParticle(
                                    p, voxelPos, voxelHalfSide, dist, gen);

                                if(!results.first.empty()) {
                                        singleThreadNewParticles.insert(singleThreadNewParticles.end(),
                                            results.first.begin(), results.first.end());
                                }
                                if(!results.second.empty()) {
                                        vEntry->addPartsAbsorbed(results.second);
                                }
                        }

                        cleanParticles(particles);
                        // Merge newly created particles after cleaning existing ones
                        particles.insert(particles.end(), singleThreadNewParticles.begin(),
                            singleThreadNewParticles.end());
    } else {
        std::vector<ParticleGroup> currentParticles;

        const size_t particlesPerThread = particles.size() / N_THREADS;
        std::vector<std::thread> threads;

        // Avoid race condition because of reading
        // and modifying the same vector
        currentParticles = std::move(particles);

        std::array<uint32_t, N_THREADS> seeds;

        for(int i = 0; i<threadsNewParticles.size(); i++) {
            threadsNewParticles[i] = {};
        }

        for(size_t i = 0; i<N_THREADS; i++) {
            size_t startIdx = i * particlesPerThread;
            size_t endIdx = (i == N_THREADS - 1) ?
              currentParticles.size() : startIdx + particlesPerThread;

            threads.emplace_back(&World::processParticlesRange,
              this, i, startIdx, endIdx, std::ref(currentParticles),
              threadSeeds[i]);
        }

        for(auto& t : threads) t.join();

        cleanParticles(currentParticles);
        particles = std::move(currentParticles);
        for(const auto& pList : threadsNewParticles) {
            particles.insert(particles.end(),
              pList.begin(), pList.end());
        }
    }
}

void World::setScene(std::vector<Voxel*>& newScene, short int newX,
  short int newY, short int newZ) {
    if(newX != 0) sizeX = newX;
    if(newY != 0) sizeY = newY;
    if(newZ != 0) sizeZ = newZ;

    scene = {};
    for(size_t i = 0; i<newScene.size(); i++) {
        scene.push_back(VoxelEntry{newScene[i], getVoxelPos(i)});
    }

    updateLists();
}

std::vector<VoxelEntry*> World::getDetectorEntries() {
    return detectors;
}

int World::detectorCountAt(short int x, short int y, short int z) {
    auto* detectorEntryPtr = voxelEntryAt(x, y, z);
    assert(detectorEntryPtr->vPtr->getType() == DETECTOR);
    return detectorEntryPtr->getNPartsAbsorbed();
}

std::vector<ParticleGroup> World::detectorPartListAt(short int x,
  short int y, short int z) {
    auto detectorEntryPtr = voxelEntryAt(x, y, z);
    assert(detectorEntryPtr->vPtr->getType() == DETECTOR);
    return detectorEntryPtr->getPartsAbsorbedCopy();
}

int World::getTotalParticles() {
    return particles.size();
}
