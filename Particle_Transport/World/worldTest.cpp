#include "world.h"
#include "../Data/data.cpp"
#include <vector>
#include <random>
#include <iostream>

int main(int argc, char const *argv[]) {
    Material airMat = getAir();
    float voxelSide = 2.0;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist(0, 1);
    Voxel air(voxelSide, MATTER, Vector3{0.0, 0.0, 0.0}, airMat);
    air.setRNG(dist, gen);

    IsotopeSample alphaSample("alphaSample", ALPHA, 2.1, 2000);
    Voxel sourceInAir(voxelSide, SOURCE, Vector3{0.0, 0.0, 0.0},
      airMat, alphaSample);

    const short int x = 3, y = 3, z = 3;

    World world(x, y, z, voxelSide);
    std::vector<Voxel> scene = {
        air, air, air, air,     air,     air, air, air, air,
        air, air, air, air, sourceInAir, air, air, air, air,
        air, air, air, air,     air,     air, air, air, air
    };
    world.setScene(scene);
    world.simulate(10.0);
    std::cout << "Active particles: " << world.getParticleCount() << "." << std::endl;
    return 0;
}
