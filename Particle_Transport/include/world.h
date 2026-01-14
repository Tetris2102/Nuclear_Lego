// world.h

#ifndef WORLD_H
#define WORLD_H

#include "voxel.h"
#include "particleGroup.h"
#include "config.h"
#include <vector>
#include <array>
#include <omp.h>
// #include <mutex>
// #include <memory>
#include <random>
#include <utility>  // for std::pair
#include <atomic>
// #include <unordered_map>
#include <chrono>
#include <cstdint>

class alignas(64) VoxelEntry {
    private:
        bool presentInVector(const std::vector<ParticleType>& vec, ParticleType pt) {           for(const auto& val : vec) {
                if(val == pt) return true;
           }
           return false;
        }
    public:
        Voxel* vPtr;
        // Should be modified only when joining particles from threads
        std::vector<ParticleGroup> partsAbsorbed;
        std::atomic<int> nPartsAbsorbed;
        std::vector<ParticleType> partsDetectable;
        short int x, y, z;

        VoxelEntry(Voxel* _vPtr, short int _x,
          short int _y, short int _z, std::vector<ParticleGroup> _pAbsorbed={}) {
            vPtr = _vPtr;
            x = _x;
            y = _y;
            z = _z;
            partsAbsorbed = _pAbsorbed;
            nPartsAbsorbed.store(0, std::memory_order_relaxed);
        }

        void addPartsAbsorbed(const std::vector<ParticleGroup>& _partsAbsorbed) {
            // std::lock_guard<std::mutex> lock(vPtr->getMtxRef());
            int probeCount = 100;
            int notAbsorbable;
            for(int i=0; i<probeCount; i++) {
                if(!presentInVector(partsDetectable, _partsAbsorbed[i].getType())) notAbsorbable++;
            }
            size_t vecSize = _partsAbsorbed.size();
            float percentageAbsorbable = 1.0f - notAbsorbable/vecSize;
            auto end = _partsAbsorbed.end() - percentageAbsorbable * vecSize;
            partsAbsorbed.insert(partsAbsorbed.begin(),
              _partsAbsorbed.begin(), end);
        }

        void incrementPartsAbsorbed(int count) {
            nPartsAbsorbed.fetch_add(count, std::memory_order_relaxed);
        }

        void setPartsDetectable(const std::vector<ParticleType>& _partsDetectable) {
            partsDetectable = _partsDetectable;
        }

        void resetNPartsAbsorbed() {
            nPartsAbsorbed.store(0, std::memory_order_relaxed);
        }

        size_t getNPartsAbsorbed() {
            if(COLLECT_ABSORBED_PARTICLES) {
              return partsAbsorbed.size();
            } else {
              return nPartsAbsorbed.load(std::memory_order_relaxed);
            }
        }

        std::vector<ParticleGroup> getPartsAbsorbedCopy() {
            return partsAbsorbed;
        }

        VoxelEntry(Voxel* _vPtr, std::array<short int, 3> _xyz,
          std::vector<ParticleGroup> _pAbsorbed={}) {
            vPtr = _vPtr;
            x = _xyz[0];
            y = _xyz[1];
            z = _xyz[2];
            partsAbsorbed = _pAbsorbed;
            nPartsAbsorbed.store(0, std::memory_order_relaxed);
        }

        // Explicitly define move constructor since std::atomic is not movable
        VoxelEntry(VoxelEntry&& other) noexcept :
            vPtr(other.vPtr),
            partsAbsorbed(std::move(other.partsAbsorbed)),
            nPartsAbsorbed(other.nPartsAbsorbed.load(std::memory_order_relaxed)),
            x(other.x), y(other.y), z(other.z) {
        }

        // Delete copy constructor and assignment (atomic can't be copied)
        VoxelEntry(const VoxelEntry& other) = delete;
        VoxelEntry& operator=(const VoxelEntry& other) = delete;
        VoxelEntry& operator=(VoxelEntry&&) = delete;
};

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
        std::vector<VoxelEntry> scene;
        std::vector<ParticleGroup> particles = {};

        std::vector<VoxelEntry*> sources = {};
        std::vector<VoxelEntry*> matters = {};
        std::vector<VoxelEntry*> detectors = {};

        // std::mutex newParticleGroupsMutex;  // Used for accessing threadsNewParticleGroups
        // std::vector<std::mutex> voxelMutexes;

        // For multithreading
        std::array<std::mt19937, N_THREADS> threadGens;
        std::array<std::uniform_real_distribution<float>, N_THREADS> threadDists;
        // // std::array<std::vector<ParticleGroup>, N_THREADS> threadPartAccumulator;

        // For single thread
        std::mt19937 gen;
        // Distribution should be in (0, 1) range
        std::uniform_real_distribution<float> dist;

        std::chrono::duration<float> lastIterationTook;
        // Controls when to decrease particleGroupSize
        float stepChangeRatio = 0.5;
        uint16_t particleGroupSize = 1;

        // void processParticleGroupsRange(int thrIdx, float time, size_t startIdx, size_t endIdx,
        //   std::vector<ParticleGroup>& currentParticleGroups);
        Vector3 getVoxelCoordVec3(int index);
        Vector3 getVoxelCoordVec3(short int x, short int y, short int z);
        std::array<short int, 3> getVoxelPos(int index);
        void updateLists();
        // Get the next voxel a particle will pass
        std::array<short int, 3> nextVoxelPos(ParticleGroup& p);
        Vector3 nextVoxelPosVec(ParticleGroup& p);
        void addParticlesEmitted(float time, std::mt19937& gen,
          std::uniform_real_distribution<float>& dist);
        void addParticlesEmittedMultithread(float time);
        // Erase deactivated or escaped particles
        void cleanParticleGroups(std::vector<ParticleGroup>& partList);
        Vector3 getVoxelPos(const Voxel& v);
    public:
        World(short int _sizeX, short int _sizeY, short int _sizeZ,
          float _voxelSide, unsigned int seed = std::random_device{}()) :
          gen(seed), dist(0, 1), lastIterationTook(0.0f) {
            sizeX = _sizeX;
            sizeY = _sizeY;
            sizeZ = _sizeZ;
            voxelSide = _voxelSide;
            voxelHalfSide = _voxelSide / 2;

            omp_set_num_threads(N_THREADS);

            // Set RNG seeds for each thread
            for(size_t i = 0; i<N_THREADS; i++) {
                threadGens[i] = std::mt19937(gen());
                threadDists[i] = std::uniform_real_distribution<float>(0, 1);
            }
        }

        void initVoxels(std::vector<Voxel> voxels);
        // Voxel indexing begins at 0, i.e. voxel closest to origin is at {0, 0, 0}
        size_t indexAt(short int x, short int y, short int z) const;
        VoxelEntry* voxelEntryAt(short int x, short int y, short int z);
        Voxel* voxelAt(short int x, short int y, short int z);
        VoxelEntry* voxelEntryAtPos(const Vector3& pos);
        Voxel& voxelAtPos(const Vector3& pos);
        void simulate(float time);
        std::chrono::duration<float> getLastIterationTime();
        // newScene must be structured according to
        // sizeX, sizeY and sizeZ given in constructor
        void setScene(std::vector<Voxel*>& newScene,
          short int newX=0, short int newY=0, short int newZ=0);
        // void setScene(std::vector<VoxelEntry>& newScene,
        //   short int newX=0, short int newY=0, short int newZ=0);
        int detectorCountAt(short int x, short int y, short int z);
        std::vector<ParticleGroup> detectorPartListAt(short int x,
          short int y, short int z);
        std::vector<VoxelEntry*> getDetectorEntries();
        size_t getTotalParticles();
};

#endif
