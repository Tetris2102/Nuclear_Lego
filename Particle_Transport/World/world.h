// world.h

#ifndef WORLD_H
#define WORLD_H

#include "../Voxel/voxel.h"
#include "../Particle/particle.h"
#include "../config.h"
#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <memory>
#include <random>

class World {
    private:
        // Default scene size is 4x4x4 voxels
        short int sizeX = 4;
        short int sizeY = 4;
        short int sizeZ = 4;

        float voxelSide;
        float voxelHalfSide;

        // Use references to avoid copying Voxel
        // Impossible to copy Voxel because it uses RNG and std::mutex
        // Should scene be const?
        std::vector<Voxel> scene;
        std::vector<Particle> particles = {};

        std::vector<Voxel*> sources = {};
        std::vector<Voxel*> matters = {};
        std::vector<Voxel*> detectors = {};

        // std::mutex newParticlesMutex;  // Used for accessing threadsNewParticles
        // std::vector<std::mutex> voxelMutexes;

        std::array<uint32_t, N_THREADS> threadSeeds;
        std::mt19937 gen;
        // Distribution should be in (0, 1) range
        std::uniform_real_distribution<float> dist;

        // Stores particles created in each
        // thread after running processParticlesRange()
        std::array<std::vector<Particle>, N_THREADS> threadsNewParticles;

        void processParticlesRange(int thrIdx, size_t startIdx, size_t endIdx,
          std::vector<Particle>& currentParticles, uint32_t seed);
        Vector3 getVoxelCoordVec3(int index);
        void updateLists();
        // Get the next voxel a particle will pass
        Voxel& nextVoxel(Particle& p);
        Vector3 nextVoxelPosVec(Particle& p);
        void addParticlesEmitted(float time);
        // Erase deactivated or escaped particles
        void cleanParticles(std::vector<Particle>& partList);
        Vector3 getVoxelPos(const Voxel& v);
    public:
        World(short int _sizeX, short int _sizeY, short int _sizeZ,
          float _voxelSide, unsigned int seed = std::random_device{}()) :
          gen(seed), dist(0, 1) {
            sizeX = _sizeX;
            sizeY = _sizeY;
            sizeZ = _sizeZ;
            voxelSide = _voxelSide;
            voxelHalfSide = _voxelSide / 2;

            // Set RNG seeds for each thread
            for(size_t i = 0; i<N_THREADS; i++) threadSeeds[i] = gen();
        }

        // Voxel indexing begins at 0, i.e. voxel closest to origin is at {0, 0, 0}
        int indexAt(short int x, short int y, short int z) const;
        Voxel& voxelAt(short int x, short int y, short int z);
        void simulate(float time);
        // newScene must be structured according to
        // sizeX, sizeY and sizeZ given in constructor
        void setScene(std::vector<Voxel>& newScene,
          short int newX=0, short int newY=0, short int newZ=0);
        // Vector of tuples of const Voxel&, x, y and z
        std::vector<Voxel*> getDetectors();
        int getParticleCount();
};

#endif
