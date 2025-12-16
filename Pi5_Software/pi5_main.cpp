// pi5_main.cpp

#include "../Particle_Transport/World/world.cpp"
#include "../Particle_Transport/Data/data.cpp"
#include "../enums.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <csignal>
#include <array>
#include <vector>

#define MUX1_ADDR 0x70
#define MUX2_ADDR 0x71

using namespace std;

Material getMaterialFromInt(uint8_t int) {
    return materials[i];
}

Sample getSampleFromInt(uint8_t int) {
    return isotopeSamples[i];
}

vector<ParticleType> partsListFromInt(uint8_t int) {
    vector<ParticleType> partsList;
    for(uint8_t i=0; i<8; i++) {
        if((1 << i) & int != 0) {
            partsList.push_back(static_cast<ParticleType>(i));
        }
    }
    return partsList;
}

void recordCubeDataTo(vector<Voxel*>& sceneToWrite,
  array<uint8_t, 2>& cubeData, int x, int y) {
    uint8_t& z = cubeData[4];
    auto voxelPtr = world.voxelEntryAt(x, y, z).vPtr;
    VoxelType voxelType = static_cast<VoxelType>(cubeData[0]);
    voxelPtr->setType(voxelType);
    voxelPtr->setMatrial(getMaterialFromInt(cubeData[1]));
    if(voxelType == DETECTOR) {
        voxelPtr->setIsotopeSample(getSampleFromInt(cubeData[2]));
        voxelPtr->setPartsDetectable(partsListFromInt(cubeData[3]));
    }
}

void signalInterrupt() {
    cout << "Exiting program..." << endl;
    close(fd);
    cout << "Done." << endl;
}

vector<Material> materials;
vector<IsotopeSample> isotopeSamples;

// 4x4x3 cube world, each cube has a side of 4.0 cm
World world(4, 4, 3, 4.0);

int main() {


    // ! WILL NOT WORK UNTIL getMaterials() and getIsotopeSamples()
    // ARE IMPLEMENTED IN data.cpp !


    // Maybe unite enums.h and data.cpp (dependent on each other)?

    // Handle system shutdown or program termination
    signal(SIGTERM, signalInterrupt);
    signal(SIGINT, signalInterrupt);

    vector<Material> materials = getMaterials();
    vector<IsotopeSample> isotopeSamples = getIsotopeSamples();

    Material air_mat = getAir();
    Voxel air_obj(MATTER, air_mat);
    Voxel* air = &air_obj;

    vector<Voxel*> initialScene(48, air);
    world.setScene(scene);

    unsigned char* dev = "/dev/i2c-1";

    int fd = open(dev, O_RDWR);
    if(fd < 0) {
        cerr << "Error: Failed to initialize i2c on master" << endl;
        return 1;
    }

    // Deselect all channels at MUX 1
    ioctl(fd, I2C_SLAVE, MUX1_ADDR);
    if(write(fd, {0b00000000}, 1) < 1) {
        cerr << "Error: Failed to write to multiplexer 1" << endl;
    }

    // Deselect all channels at MUX 2
    ioctl(fd, I2C_SLAVE, MUX2_ADDR);
    if(write(fd, {0b00000000}, 1) < 1) {
        cerr << "Error: Failed to write to multiplexer 2" << endl;
    }

    // currentCubeData = [voxelType, materialType,
    // sampleType, particlesDetectable, level]
    array<uint8_t, 5> currentCubeData;
    int muxChannelNum = 0;
    for(uint8_t x=0; x<4; x++) {
        for(uint8_t y=0; y<4; y++) {
            muxChannelNum = y * 4 + (x+1);
            if(muxChannelNum < 9) {

                ioctl(fd, I2C_SLAVE, MUX1_ADDR);
                if(write(fd, {0b00000001 << muxChannelNum}, 1) < 1) {
                    cerr << "Error: Failed to write to multiplexer 1" << endl;
                }

                for(uint8_t i2c_addr=8; i2c_addr<11; i2c_addr++) {
                    ioctl(fd, I2C_SLAVE, i2c_addr);
                    read(fd, currentCubeData, 2);
                    recordCubeDataTo(scene, currentCubeData, x, y);
                }

                ioctl(fd, I2C_SLAVE, MUX1_ADDR);
                if(write(fd, {0b00000000}, 1) < 1) {
                    cerr << "Error: Failed to write to multiplexer 1" << endl;
                }

            } else if (muxChannelNum < 17) {

                ioctl(fd, I2C_SLAVE, MUX2_ADDR);
                if(write(fd, {0b00000001 << (muxChannelNum % 8)}, 1) < 1) {
                    cerr << "Error: Failed to write to multiplexer 1" << endl;
                }

                for(uint8_t i2c_addr=8; i2c_addr<11; i2c_addr++) {
                    ioctl(fd, I2C_SLAVE, i2c_addr);
                    read(fd, currentCubeData, 2);
                    recordCubeDataTo(scene, currentCubeData, x, y);
                }

                ioctl(fd, I2C_SLAVE, MUX2_ADDR);
                if(write(fd, {0b00000000}, 1) < 1) {
                    cerr << "Error: Failed to write to multiplexer 1" << endl;
                }

            } else {

                cerr << "Error: Invalid multiplexer channel number" << endl;

            }
        }
    }

    int steps = 5;
    float dt = 1.0 / steps;  // Simulating one second total
    for(int i=0; i<steps; i++) {
        world.simulate(dt);
    }

    array<uint8_t, 2> cubeDataToWrite;
    auto detectorsMap = world.getDetectorsAbsorbedMap();
    for(const auto& dMap : detectorsMap) {

        auto detectorEntry = dMap.first;
        auto detectorPtr = detectorEntry.vPtr;

        uint8_t x = detectorEntry->x;
        uint8_t y = detectorEntry->y;
        uint8_t z = detectorEntry->z;

        uint16_t activity = detectorPtr->getNPartsAbsorbed();
        cubeDataToWrite[0] = activity & 0xFF;  // Least significant byte
        cubeDataToWrite[1] = activity >> 8;    // Most significant byte

        muxChannelNum = y * 4 + (x+1);

        if(muxChannelNum < 9) {

            ioctl(fd, I2C_SLAVE, MUX1_ADDR);
            if(write(fd, {0b00000001 << muxChannelNum}, 1) < 1) {
                cerr << "Error: Failed to write to multiplexer 1" << endl;
            }

            ioctl(fd, I2C_SLAVE, i2c_addr+8);
            write(fd, cubeDataToWrite, 2);

            ioctl(fd, I2C_SLAVE, MUX1_ADDR);
            if(write(fd, {0b00000000}, 1) < 1) {
                cerr << "Error: Failed to write to multiplexer 1" << endl;
            }

        } else if(muxChannelNum < 17) {

            ioctl(fd, I2C_SLAVE, MUX2_ADDR);
            if(write(fd, {0b00000001 << muxChannelNum}, 1) < 1) {
                cerr << "Error: Failed to write to multiplexer 1" << endl;
            }

            uint16_t activity = detectorPtr->getNPartsAbsorbed();
            cubeDataToWrite[0] = activity & 0xFF;  // Least significant byte
            cubeDataToWrite[1] = activity >> 8;    // Most significant byte

            ioctl(fd, I2C_SLAVE, i2c_addr+8);
            write(fd, cubeDataToWrite, 2);

            ioctl(fd, I2C_SLAVE, MUX2_ADDR);
            if(write(fd, {0b00000000}, 1) < 1) {
                cerr << "Error: Failed to write to multiplexer 1" << endl;
            }

        } else {

            cerr << "Error: Invalid multiplexer channel number" << endl;

        }
    }

    // scan for cubes on each multiplexer channel and record their data in scene - done
    // compute activities - done
    // write activities to cubes - done
}
