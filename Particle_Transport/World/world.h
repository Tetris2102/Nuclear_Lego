// world.h

#ifndef WORLD_H
#define WORLD_H

#include "../Voxel/voxel.h"
#include "../Particle/particle.h"
#include <vector>
#include <array>

class World {
    private:
        // Default side is 4.0 cm
        short int sizeX = 4.0;
        short int sizeY = 4.0;
        short int sizeZ = 4.0;

        float voxelSide;

        std::vector<Voxel> scene;
        std::vector<Particle> particles = {};

        std::vector<Voxel*> sources = {};
        std::vector<Voxel*> matters = {};
        std::vector<Voxel*> detectors = {};

        void updateLists();
        // Get the next voxel a particle will pass
        Voxel& nextVoxel(const Particle& p) const;
        void addParticlesEmitted(float time);
        // Erase deactivated or escaped particles
        void cleanParticles();
    public:
        World(short int _sizeX, short int _sizeY, short int _sizeZ,
          float _voxelSide) {
            sizeX = _sizeX;
            sizeY = _sizeY;
            sizeZ = _sizeZ;
            voxelSide = _voxelSide;
        }

        // Voxel indexing begins at 0, i.e. voxel closest to origin is at {0, 0, 0}
        int indexAt(short int x, short int y, short int z) const;
        Voxel& voxelAt(short int x, short int y, short int z);
        void simulate(float time);
        // newScene must be structured according to
        // sizeX, sizeY and sizeZ given in constructor
        void setScene(const std::vector<Voxel>& newScene);
        // Vector of tuples of const Voxel&, x, y and z
        std::vector<Voxel*> World::getDetectors();
};

#endif
