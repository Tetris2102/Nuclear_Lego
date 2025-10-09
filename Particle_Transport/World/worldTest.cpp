#include "world.h"
#include "../Data/data.cpp"
#include <vector>

int main(int argc, char const *argv[]) {
    Material airMat = getAir();
    float voxelSide = 2.0;
    Voxel air(voxelSide, MATTER, Vector3{0.0, 0.0, 0.0}, airMat);
    const short int x = 3, y = 3, z = 3;

    World world(x, y, z, voxelSide);
    std::vector<Voxel> scene = {
        air, air, air, air, air, air, air, air, air,
        air, air, air, air, air, air, air, air, air,
        air, air, air, air, air, air, air, air, air
    };
    world.setScene(scene);
    return 0;
}
