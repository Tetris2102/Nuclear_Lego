// world.cpp

#include "world.h"
#include <cmath>
#include <array>
#include <cassert>
#include <algorithm>

void World::updateLists() {
    sources = {};
    matters = {};
    detectors = {};

    for(const auto& voxel : scene) {
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
  i_5 = j - sizeX * sizeY
*/
Voxel& World::nextVoxel(const Particle& p) const {
    short int x = std::floor(p.getX() / voxelSide);
    short int y = std::floor(p.getY() / voxelSide);
    short int z = std::floor(p.getZ() / voxelSide);
    int j = indexAt(x, y, z);
    std::array<int, 6> adjIndices = {
        j + 1,
        j - 1,
        j + sizeX,
        j - sizeX,
        j + sizeX * sizeY,
        j - sizeX * sizeY
    };

    for(const auto& i : adjIndices) {
        if(scene[i].intersects(p)) return scene[i];
    }
}

void World::addParticlesEmitted(float time) {
    for(const auto* voxel : sources) {
        auto particlesEmitted = voxel->getPartsEmittedList(time);
        for(auto& p : particlesEmitted) voxel->moveToExit(p);
        particles.insert(particles.end(), particlesEmitted.begin(),
          particlesEmitted.end());
    }
}

void World::cleanParticles() {
    // Maximum scene dimensions
    float maxX = voxelSide * (sizeX + 1);
    float maxY = voxelSide * (sizeY + 1);
    float maxZ = voxelSide * (sizeZ + 1);

    auto toRemove = [&] (const Particle& p) {
        bool xEscaped = (p.getX() < 0 || p.getX() > maxX);
        bool yEscaped = (p.getY() < 0 || p.getY() > maxY);
        bool zEscaped = (p.getZ() < 0 || p.getZ() > maxZ);

        return (!p.isActive() || xEscaped || yEscaped || zEscaped);
    };

    particles::erase(
      particles::remove_if(particles.begin(), particles.end(), toRemove,
      particles.end()
    );
}

// Voxels in scene are individually numbered from 0, first along x-axis,
// then in rows along y-axis, then in layers along z-axis
int World::indexAt(short int x, short int y, short int z) const {
    return x + sizeX * (y + sizeY * z);
}

Voxel& World::voxelAt(short int x, short int y, short int z) {
    assert(x < sizeX && y < sizeY && z < sizeZ);
    return scene[indexAt(x, y, z)];
}

void World::simulate(float time) {
    for(auto& p : particles) {
        addParticlesEmitted(time);
        nextVoxel(p).processParticle(p);
    }
    cleanParticles();
}

void World::setScene(const std::vector<Voxel*>& newScene) {
    assert(scene.size() == newScene.size());
    for(int i = 0; i<scene.size(); i++) {
        scene[i] = *newScene[i];
    }
    updateLists();
}

std::vector<Voxel*> World::getDetectors() {
    return detectors;
}
