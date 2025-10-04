#ifndef WORLD_H
#define WORLD_H

#include "../Voxel/voxel.h"
#include "../Particle/particle.h"
#include <vector>

class World {
    private:
        short int cellsX;
        short int cellsY;
        short int cellsZ;

        std::vector<std::vector<std::vector<Voxel>>> scene;
        std::vector<Particle> particles = {};
    public:
        World(short int cellsX, short int cellsY, short int cellsZ) {
            this->cellsX = cellsX;
            this->cellsY = cellsY;
            this->cellsZ = cellsZ;
        }

        void setScene(std::vector<std::vector<std::vector<Voxel>>> scene);
        Voxel& takeVoxel(short int x, short int y, short int z);
};

#endif
