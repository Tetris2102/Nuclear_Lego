#include "voxel.h"
#include "../Data/data.cpp"
#include "../Vector3/vector3.h"
#include "../IsotopeSample/isotopeSample.h"
#include <iostream>

int main() {
    // Needed for data.cpp
    Material air = getAir();

    Voxel detectorAir(4.0, DETECTOR, Vector3{6.0, 2.0, 2.0}, air);
    IsotopeSample myCesium("myCesium", ALPHA, 2.1, 10000);  // 10000 Bq activity
    Voxel sourceCesium(4.0, SOURCE, Vector3{2.0, 2.0, 2.0}, air, myCesium);

    int a = 0;

    auto emittedPartsList = sourceCesium.getPartsEmittedList(10.0);
    std::cout << "Emitted " << emittedPartsList.size() << " particles." << std::endl;
    for(auto p : emittedPartsList) {
        detectorAir.processParticle(p);
        if(a < 12) {
           // std::cout << "Momentum: " << p.getMomentum().x << ", " <<
           // p.getMomentum().y << ", " << p.getMomentum().z << std::endl;
           // std::cout << "Intersects: " << detectorAir.intersects(p) << std::endl;
           // if(!p.isActive()) std::cout << "NOT ACTIVE" << std::endl;
           a++;
        }
    }
    double absorbed = detectorAir.getPartsAbsorbed();
    std::cout << "Absorbed " << absorbed << " particles." << std::endl;
    return 0;
}

// Voxel myVoxel(1.0, {1.207, 1.207, 1.207}, air); // in cm
// Particle myProton(PROTON, 1.0, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0});
// std::cout << myVoxel.intersects(myProton) << '\n';
// auto params = myVoxel.intersectParams(myProton);
// std::cout << "tmin: " << params[0] << ", tmax: " << params[1] << '\n';
