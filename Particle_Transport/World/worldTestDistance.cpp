// Compares particles absorbed by detectors
// at different distances to source

#include "world.h"
#include "../Data/data.cpp"
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <mutex>
#include <memory>
#include <utility>

// TODO: make Voxel::processParticle() accept pointers to RNG objects
// instead of having RNG instances for every object

int main(int argc, char const *argv[]) {

    Material airMat = getAir();

    Voxel air(MATTER, airMat);

    IsotopeSample betaSample("betaSample", BETA, 1.0, 10000);
    Voxel sourceInAir(SOURCE, airMat, betaSample);

    Voxel detector(DETECTOR, airMat);

    const short int x = 3, y = 3, z = 3;

    World world(x, y, z, 4.0);
    std::vector<Voxel> scene = {
        sourceInAir, detector, air, detector,  air,    air, air, air, air,
         detector,  air, detector,    air,   detector, air, air, air, air,
           air,        air,       air,        air,     air,    air, air, air, air
    };
    world.setScene(scene);

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i<1; i++) {
        world.simulate(10.0);
    }
    auto end = std::chrono::high_resolution_clock::now();

    int partsD1 = world.voxelAt(0, 1, 0).getPartsAbsorbed();
    int partsD2 = world.voxelAt(0, 0, 1).getPartsAbsorbed();

    std::cout << "Simulated for 10.0 s" << std::endl;
    std::cout << "Results:" << std::endl;
    std::cout << "Detector 1 (closer):" << std::endl;
    std::cout << "    " << partsD1 << " particles detected" << std::endl;
    std::cout << "Detector 2 (further):" << std::endl;
    std::cout << "    " << partsD2 << " particles detected" << std::endl;
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Simulating 10.0s took: " << elapsed.count() << " seconds" << std::endl;

    std::cout << "Total particle count on exit: " << world.getParticleCount() << std::endl;

    return 0;
}
