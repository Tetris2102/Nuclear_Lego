// world.cpp

#include "world.h"
#include "../config.h"
#include <cmath>
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
        auto vPos = nextVoxelPosVec(p);
        auto v = voxelAt(vPos.x, vPos.y, vPos.z);

        auto newParticles = v.processParticle(p, voxelHalfSide,
          threadDist, threadGen);

        if(!newParticles.empty()) {
            // std::lock_guard<std::mutex> partLock(newParticlesMutex);
            threadsNewParticles[thrIdx] = newParticles;
        }

        // // Create separate scope for std::lock_guard
        // {
        //     std::lock_guard<std::mutex> lock(
        //       (voxelMutexes[indexAt(vPos.x, vPos.y, vPos.z)]));
        //     auto newParticles = v.processParticle(p, voxelHalfSide,
        //       threadDist, threadGen);
        //
        //     if(!newParticles.empty()) {
        //         std::lock_guard<std::mutex> partLock(newParticlesMutex);
        //         threadsNewParticles[thrIdx] = newParticles;
        //     }
        // }
    }
}

// For float, not short int position
Vector3 World::getVoxelCoordVec3(int index) {
    float x = index % sizeX;
    float y = (index / sizeX) % sizeY;
    float z = index / (sizeX * sizeY);

    return Vector3{(x+1) * voxelHalfSide,
      (y+1) * voxelHalfSide,
      (z+1) * voxelHalfSide};
}

void World::updateLists() {
    sources = {};
    matters = {};
    detectors = {};

    for(auto& voxel : scene) {
        VoxelType type = voxel.getType();
        if(type == SOURCE) {
            sources.push_back(&voxel);
        } else if (type == MATTER) {
            matters.push_back(&voxel);
        } else {
            detectors.push_back(&voxel);
        }
    }
}

Voxel& World::nextVoxel(Particle& p) {
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
        return scene[0];
    }
    if(y < 0 || y > sizeY - 1) {
        p.deactivate();
        return scene[0];
    }
    if(z < 0 || z > sizeZ - 1) {
        p.deactivate();
        return scene[0];
    }

    return voxelAt(x, y, z);  // Convert short int indices into
                              // float absolute positions
}

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
        return Vector3(0, 0, 0);
    }
    if(y < 0 || y > sizeY - 1) {
        p.deactivate();
        return Vector3(0, 0, 0);
    }
    if(z < 0 || z > sizeZ - 1) {
        p.deactivate();
        return Vector3(0, 0, 0);
    }

    return Vector3(x, y, z);  // Convert short int indices into
                              // float absolute positions
}

void World::addParticlesEmitted(float time) {
    for(auto* voxel : sources) {
        auto particlesEmitted = voxel->getPartsEmittedList(time, dist, gen);
        for(auto& p : particlesEmitted) voxel->moveToExit(p, voxelHalfSide);
        particles.insert(particles.end(), particlesEmitted.begin(),
          particlesEmitted.end());
    }
}

void World::cleanParticles(std::vector<Particle>& partList) {
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
int World::indexAt(short int x, short int y, short int z) const {
    return x + sizeX * (y + z * sizeY);
}

Voxel& World::voxelAt(short int x, short int y, short int z) {
    assert(x < sizeX && x >= 0 &&
        y < sizeY && y >= 0 &&
        z < sizeZ && z >= 0);
    return scene[indexAt(x, y, z)];
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
        for(auto& p : particles) {
            nextVoxel(p).processParticle(p, voxelHalfSide, dist, gen);
        }

        cleanParticles(particles);
    } else {
        std::vector<Particle> currentParticles;

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

void World::setScene(std::vector<Voxel>& newScene, short int newX,
  short int newY, short int newZ) {
    if(newX != 0) sizeX = newX;
    if(newY != 0) sizeY = newY;
    if(newZ != 0) sizeZ = newZ;

    scene = newScene;
    // voxelMutexes.resize(scene.size());  // must have a mutex for each voxel
    for(int i = 0; i<newScene.size(); i++) {
        scene[i].setPosition(getVoxelCoordVec3(i));
    }
    updateLists();
}

std::vector<Voxel*> World::getDetectors() {
    return detectors;
}

int World::getParticleCount() {
    return particles.size();
}
