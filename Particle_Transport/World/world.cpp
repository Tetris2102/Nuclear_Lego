// world.cpp

#include "world.h"
#include <cmath>
#include <array>
#include <cassert>
#include <algorithm>  // std::clamp is used (C++17)
#include <limits>
#include <iostream>

// dir has to be normalized
Vector3 pointAlongVec3(const Vector3& origin, const Vector3& dir, float t) {
    float x = origin.x + t * dir.x;
    float y = origin.y + t * dir.y;
    float z = origin.z + t * dir.z;

    return Vector3{x, y, z};
}

// Vector3 getVoxelPos(const Voxel& v) {
//
// }

// For float, not short int position
Vector3 World::getVoxelPosVec3(int index) {
    float x = index % 3;
    float y = std::floor(index / 3);
    float z = std::floor(index / 9);

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

/*
  Formulas for indices of 6 voxels adjacent to given voxel,
  given its index j:
  i_1 = j + 1
  i_2 = j - 1
  i_3 = j + sizeX
  i_4 = j - sizeX
  i_5 = j + sizeX * sizeY
  i_6 = j - sizeX * sizeY
  i_7 = j + sizeX * (sizeY-1)
  i_8 = j + (sizeX-1) * (sizeY-1)
  i_9 = j + (sizeX-1) * sizeY
*/
// TransportOut World::nextVoxel(Particle& p) {
//     short int x = std::floor(p.getX() / voxelSide);
//     short int y = std::floor(p.getY() / voxelSide);
//     short int z = std::floor(p.getZ() / voxelSide);
//     int j = indexAt(x, y, z);
//     std::array<int, 6> adjIndices = {
//         j + 1,
//         j - 1,
//         j + sizeX,
//         j - sizeX,
//         j + sizeX * sizeY,
//         j - sizeX * sizeY
//     };
//
//     for(auto& i : adjIndices) {
//         if(scene[i].intersects(p)) return scene[i];
//     }
//     return scene[0];  // Else redirect particle to first voxel, which will
//                       // just discard it because no intersection occurs
// }

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
        nextBoundaryX = std::numeric_limits<float>::infinity();
    }
    
    if(dir.y != 0) {
        nextBoundaryY = (((dir.y > 0) ? (y+1) : y) * voxelSide - pos.y) / dir.y;
    } else {
        nextBoundaryY = std::numeric_limits<float>::infinity();
    }

    if(dir.z != 0) {
        nextBoundaryZ = (((dir.z > 0) ? (z+1) : z) * voxelSide - pos.z) / dir.z;
    } else {
        nextBoundaryZ = std::numeric_limits<float>::infinity();
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

void World::addParticlesEmitted(float time) {
    for(auto* voxel : sources) {
        auto particlesEmitted = voxel->getPartsEmittedList(time);
        for(auto& p : particlesEmitted) voxel->moveToExit(p, voxelHalfSide);
        particles.insert(particles.end(), particlesEmitted.begin(),
          particlesEmitted.end());
    }
}

void World::cleanParticles() {
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

    particles.erase(
      std::remove_if(particles.begin(), particles.end(), toRemove),
      particles.end()
    );
}

// Voxels in scene are individually numbered from 0, first along x-axis,
// then in rows along y-axis, then in layers along z-axis
int World::indexAt(short int x, short int y, short int z) const {
    return x + sizeX * (y + sizeY * z);
}

Voxel& World::voxelAt(short int x, short int y, short int z) {
    assert(x < sizeX && x >= 0 &&
        y < sizeY && y >= 0 &&
        z < sizeZ && z >= 0);
    return scene[indexAt(x, y, z)];
}

void World::simulate(float time) {
    addParticlesEmitted(time);

    for(auto& p : particles) {
        nextVoxel(p).processParticle(p, voxelHalfSide);
    }
    cleanParticles();
}

void World::setScene(std::vector<Voxel>& newScene, short int newX,
  short int newY, short int newZ) {
    if(newX != 0) sizeX = newX;
    if(newY != 0) sizeY = newY;
    if(newZ != 0) sizeZ = newZ;

    scene = newScene;
    for(int i = 0; i<newScene.size(); i++) {
        scene[i].setPosition(getVoxelPosVec3(i));
    }
    updateLists();
}

std::vector<Voxel*> World::getDetectors() {
    return detectors;
}

int World::getParticleCount() {
    return particles.size();
}
