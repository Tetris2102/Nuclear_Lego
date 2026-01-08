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

// TODO: make Voxel::processParticleGroup() accept pointers to RNG objects
// instead of having RNG instances for every object

int main() {

    Material airMat = getM_Air();
    Material leadMat = getM_Pb207();

    Voxel air_obj(MATTER, airMat);
    Voxel* air = &air_obj;

    IsotopeSample betaSample("betaSample", ALPHA, 1.0, 2000000);
    Voxel sourceInAir_obj(SOURCE, airMat, betaSample);
    Voxel* sourceInAir = &sourceInAir_obj;

    Voxel detector1(DETECTOR, leadMat);
    Voxel detector2(DETECTOR, leadMat);

    const short int x = 3, y = 3, z = 3;

    unsigned int seed = 42;
    World world(x, y, z, 4.0, seed);
    std::vector<Voxel*> scene = {
    sourceInAir,   air,     &detector1,
        air,       air,       air,
        air,       air,       air,

        air,       air,       air,
        air,       air,       air,
        air,       air,       air,

     &detector2,   air,       air,
        air,       air,       air,
        air,       air,       air
    };
    world.setScene(scene);


  std::cout << "Detector pointers:" << std::endl;
  std::cout << world.voxelAt(2, 0, 0) << std::endl;
  std::cout << world.voxelAt(0, 0, 2) << std::endl;

  // reset global voxel instrumentation counters so we get fresh numbers for this run
  Voxel::resetGlobalStats();

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i<20; i++) {
        world.simulate(0.05);
    }
    auto end = std::chrono::high_resolution_clock::now();

  // Print voxel-level event statistics to help diagnose Monte-Carlo behavior
  {
    auto stats = Voxel::getGlobalStats();
    std::cout << "Voxel events: passes=" << stats.passes
          << " scatters=" << stats.scatters
          << " absorbs=" << stats.absorbs
          << " created=" << stats.created << std::endl;
  }

    int partsD1 = world.detectorCountAt(2, 0, 0);
    int partsD2 = world.detectorCountAt(0, 0, 2);

    std::cout << "Simulated for 1.0 s" << std::endl;
    std::cout << "Results:" << std::endl;
    std::cout << "Detector 1 (closer):" << std::endl;
    std::cout << "    " << partsD1 << " particles detected" << std::endl;
    std::cout << "Detector 2 (further):" << std::endl;
    std::cout << "    " << partsD2 << " particles detected" << std::endl;
    // auto detectors = world.getDetectors();
    // for(size_t i = 0; i<detectors.size(); i++) {
    //     std::cout << "Detector #" << i << " absorbed " <<
    //     detectors[i]->getPartsAbsorbed() << std::endl;
    // }
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Simulating 1.0s took: " << elapsed.count() <<
      " seconds" << std::endl;

    std::cout << "Total particle count on exit: " <<
      world.getTotalParticles() << std::endl;

    return 0;
}
