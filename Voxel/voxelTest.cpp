#import "voxel.h"
#import <iostream>

int main() {
    Material air("air", 16, 0.01);
    Voxel myVoxel(1.0, {1.207, 1.207, 1.207}, air); // in cm
    Particle myProton(PROTON, 1.0, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0});
    std::cout << myVoxel.intersects(myProton) << '\n';
    auto params = myVoxel.intersectParams(myProton);
    std::cout << "tmin: " << params[0] << ", tmax: " << params[1] << '\n';
    return 0;
}
