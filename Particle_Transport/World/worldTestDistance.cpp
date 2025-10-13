// Compares particles absorbed by detectors
// at different distances to source

#include "world.h"
#include "../Data/data.cpp"
#include <vector>
#include <random>
#include <iostream>
#include <chrono>

int main(int argc, char const *argv[]) {

    Material airMat = getAir();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0, 1);

    Voxel air(MATTER, airMat, gen, dist);

    IsotopeSample betaSample("betaSample", BETA, 1.0, 10000);
    Voxel sourceInAir(SOURCE, airMat, betaSample, gen, dist);

    Voxel detector(DETECTOR, airMat, gen, dist);

    const short int x = 3, y = 3, z = 3;

    World world(x, y, z, 4.0);
    std::vector<Voxel> scene = {
        sourceInAir, detector,    sourceInAir,    air, air, air, air, air, air,
           air,    sourceInAir, detector, air, air, air, air, air, air,
           air,        air,       air,    air, air, air, air, air, air
    };
    world.setScene(scene);

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i<10; i++) {
        world.simulate(1.0);
    }
    auto end = std::chrono::high_resolution_clock::now();

    int partsD1 = world.voxelAt(1, 0, 0).getPartsAbsorbed();
    int partsD2 = world.voxelAt(2, 0, 1).getPartsAbsorbed();

    std::cout << "Simulated for 10.0 s" << std::endl;
    std::cout << "Results:" << std::endl;
    std::cout << "Detector 1 (closer):" << std::endl;
    std::cout << "    " << partsD1 << " particles detected" << std::endl;
    std::cout << "Detector 2 (further):" << std::endl;
    std::cout << "    " << partsD2 << " particles detected" << std::endl;
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Simulating 10.0s took: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
