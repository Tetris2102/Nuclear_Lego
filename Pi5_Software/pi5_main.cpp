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
#include <unistd.h>
#include <iostream>
#include <iomanip>

#define MUX1_ADDR 0x70
#define MUX2_ADDR 0x71

using namespace std;

// Global variables
int fd = -1;
vector<Material> materials;
vector<IsotopeSample> isotopeSamples;
World* worldPtr = nullptr;

vector<Voxel*> createAirFilledScene(int size) {
    vector<Voxel*> airScene;
    airScene.reserve(size);
    Material air = getM_Air();

    for(uint8_t i=0; i<size; i++) {
        airScene.push_back(new Voxel{MATTER, air});
    }
    return airScene;
}

Material getMaterialFromInt(uint8_t idx) {
    return materials[idx];
}

IsotopeSample getSampleFromInt(uint8_t idx) {
    return isotopeSamples[idx];
}

vector<ParticleType> partsListFromInt(uint8_t val) {
    vector<ParticleType> partsList;
    for(uint8_t i=0; i<8; i++) {
        if(((1 << i) & val) != 0) {
            partsList.push_back(static_cast<ParticleType>(i));
        }
    }
    return partsList;
}

void recordCubeDataTo(vector<Voxel*>& sceneToWrite,
  array<uint8_t, 5>& cubeData, int x, int y) {
    uint8_t z = cubeData[4];
    auto voxelEntryPtr = worldPtr->voxelEntryAt(x, y, z);
    auto* voxelPtr = voxelEntryPtr->vPtr;
    VoxelType voxelType = static_cast<VoxelType>(cubeData[0]);
    voxelPtr->setType(voxelType);
    voxelPtr->setMaterial(getMaterialFromInt(cubeData[1]));
    if(voxelType == DETECTOR) {
        voxelPtr->setIsotopeSample(getSampleFromInt(cubeData[2]));
        voxelEntryPtr->setPartsDetectable(partsListFromInt(cubeData[3]));
    }
}

void signalInterrupt(int sig) {
    (void)sig;  // Suppress unused parameter warning
    cout << "Exiting program..." << endl;
    if(fd >= 0) {
        close(fd);
    }
    cout << "Done." << endl;
    exit(0);
}

// 4x4x3 cube world, each cube has a side of 4.0 cm
World world(4, 4, 3, 4.0);

int main() {

    // Handle system shutdown or program termination
    signal(SIGTERM, signalInterrupt);
    signal(SIGINT, signalInterrupt);

    materials = getMaterials();
    isotopeSamples = getIsotopeSamples();
    worldPtr = &world;

    Material air_mat = getM_Air();

    vector<Voxel*> scene = createAirFilledScene(48);
    world.setScene(scene);

    const char* dev = "/dev/i2c-1";

    fd = open(dev, O_RDWR);
    if(fd < 0) {
        cerr << "Error: Failed to initialize i2c on master" << endl;
        return 1;
    }

    // Deselect all channels at MUX 1
    unsigned char muxData = 0b00000000;
    if(ioctl(fd, I2C_SLAVE, MUX1_ADDR) < 0) {
        cerr << "Error: Failed to set MUX1 slave address" << endl;
        return 1;
    }
    if(write(fd, &muxData, 1) < 1) {
        cerr << "Error: Failed to write to multiplexer 1" << endl;
    }

    // Deselect all channels at MUX 2
    if(ioctl(fd, I2C_SLAVE, MUX2_ADDR) < 0) {
        cerr << "Error: Failed to set MUX2 slave address" << endl;
        return 1;
    }
    if(write(fd, &muxData, 1) < 1) {
        cerr << "Error: Failed to write to multiplexer 2" << endl;
    }

    // currentCubeData = [voxelType, materialType,
    // sampleType, particlesDetectable, level]
    array<uint8_t, 5> currentCubeData;
    int muxChannelNum = 0;
    for(uint8_t x=0; x<4; x++) {
        for(uint8_t y=0; y<4; y++) {
            muxChannelNum = y * 4 + (x + 1) - 1;  // Channels numbered 0 to 15
            if(muxChannelNum < 8) {

                // Select MUX1 channel (muxChannelNum 1-8 maps to mux channels 0-7)
                if(ioctl(fd, I2C_SLAVE, MUX1_ADDR) < 0) {
                    cerr << "Error: Failed to set MUX1 slave address" << endl;
                    continue;
                }
                muxData = 1 << muxChannelNum;
                if(write(fd, &muxData, 1) < 1) {
                    cerr << "Error: Failed to write to MUX1" << endl;
                    continue;
                }
                usleep(1000);  // Small delay for mux to settle

                // Read from each level (addresses 8, 9, 10)
                for(uint8_t i2c_addr=8; i2c_addr<11; i2c_addr++) {
                    if(ioctl(fd, I2C_SLAVE, i2c_addr) < 0) continue;
                    int bytesRead = read(fd, currentCubeData.data(), 5);
                    if(bytesRead == 5) {
                        recordCubeDataTo(scene, currentCubeData, x, y);
                    } else if(bytesRead > 0) {
                        cerr << "Warning: Only read " << bytesRead 
                             << " bytes from 0x" << hex << (int)i2c_addr << endl;
                    }
                }

                // Deselect MUX1
                if(ioctl(fd, I2C_SLAVE, MUX1_ADDR) < 0) {
                    cerr << "Error: Failed to set MUX1 slave address" << endl;
                    continue;
                }
                muxData = 0b00000000;
                if(write(fd, &muxData, 1) < 1) {
                    cerr << "Error: Failed to write to MUX1" << endl;
                }

            } else if (muxChannelNum < 16) {

                // Select MUX2 channel
                if(ioctl(fd, I2C_SLAVE, MUX2_ADDR) < 0) {
                    cerr << "Error: Failed to set MUX2 slave address" << endl;
                    continue;
                }
                muxData = 1 << (muxChannelNum - 8);
                if(write(fd, &muxData, 1) < 1) {
                    cerr << "Error: Failed to write to MUX2" << endl;
                    continue;
                }
                usleep(1000);  // Small delay for mux to settle

                // Read from each level (addresses 8, 9, 10)
                for(uint8_t i2c_addr=8; i2c_addr<11; i2c_addr++) {
                    if(ioctl(fd, I2C_SLAVE, i2c_addr) < 0) continue;
                    int bytesRead = read(fd, currentCubeData.data(), 5);
                    if(bytesRead == 5) {
                        recordCubeDataTo(scene, currentCubeData, x, y);
                    } else if(bytesRead > 0) {
                        cerr << "Warning: Only read " << bytesRead 
                             << " bytes from 0x" << hex << (int)i2c_addr << endl;
                    }
                }

                // Deselect MUX2
                if(ioctl(fd, I2C_SLAVE, MUX2_ADDR) < 0) {
                    cerr << "Error: Failed to set MUX2 slave address" << endl;
                    continue;
                }
                muxData = 0b00000000;
                if(write(fd, &muxData, 1) < 1) {
                    cerr << "Error: Failed to write to MUX2" << endl;
                }

            } else {

                cerr << "Error: Invalid multiplexer channel number: " 
                    << muxChannelNum << endl;

            }
        }
    }

    int steps = 5;
    float dt = 1.0 / steps;  // Simulating one second total
    for(int i=0; i<steps; i++) {
        world.simulate(dt);
    }

    array<uint8_t, 2> cubeDataToWrite;
    auto detectorEntries = world.getDetectorEntries();
    for(const auto& dEntry : detectorEntries) {

        uint8_t x = dEntry->x;
        uint8_t y = dEntry->y;
        uint8_t z = dEntry->z;

        uint16_t activity = dEntry->getNPartsAbsorbed();
        cubeDataToWrite[0] = activity & 0xFF;  // Least significant byte
        cubeDataToWrite[1] = activity >> 8;    // Most significant byte

        muxChannelNum = y * 4 + (x+1);
        uint8_t i2c_addr = 8 + z;   // Address based on level (z): 8, 9, or 10

        if(muxChannelNum < 9) {

            // Select MUX1 channel (muxChannelNum 1-8 maps to mux channels 0-7)
            if(ioctl(fd, I2C_SLAVE, MUX1_ADDR) < 0) {
                cerr << "Error: Failed to set MUX1 slave address" << endl;
                continue;
            }
            muxData = 1 << muxChannelNum;
            if(write(fd, &muxData, 1) < 1) {
                cerr << "Error: Failed to write to MUX1" << endl;
                continue;
            }
            usleep(1000);  // Small delay for mux to settle

            // Write activity to the specific level
            if(ioctl(fd, I2C_SLAVE, i2c_addr) < 0) {
                cerr << "Error: Failed to set slave address 0x" 
                     << hex << (int)i2c_addr << endl;
                continue;
            }
            if(write(fd, cubeDataToWrite.data(), 2) < 2) {
                cerr << "Error: Failed to write activity data" << endl;
            }

            // Deselect MUX1
            if(ioctl(fd, I2C_SLAVE, MUX1_ADDR) < 0) {
                cerr << "Error: Failed to set MUX1 slave address" << endl;
                continue;
            }
            muxData = 0b00000000;
            if(write(fd, &muxData, 1) < 1) {
                cerr << "Error: Failed to write to MUX1" << endl;
            }

        } else if(muxChannelNum < 17) {

            // Select MUX2 channel
            if(ioctl(fd, I2C_SLAVE, MUX2_ADDR) < 0) {
                cerr << "Error: Failed to set MUX2 slave address" << endl;
                continue;
            }
            muxData = 1 << (muxChannelNum - 8);
            if(write(fd, &muxData, 1) < 1) {
                cerr << "Error: Failed to write to MUX2" << endl;
                continue;
            }
            usleep(1000);  // Small delay for mux to settle

            // Write activity to the specific level
            if(ioctl(fd, I2C_SLAVE, i2c_addr) < 0) {
                cerr << "Error: Failed to set slave address 0x" 
                     << hex << (int)i2c_addr << endl;
                continue;
            }
            if(write(fd, cubeDataToWrite.data(), 2) < 2) {
                cerr << "Error: Failed to write activity data" << endl;
            }

            // Deselect MUX2
            if(ioctl(fd, I2C_SLAVE, MUX2_ADDR) < 0) {
                cerr << "Error: Failed to set MUX2 slave address" << endl;
                continue;
            }
            muxData = 0b00000000;
            if(write(fd, &muxData, 1) < 1) {
                cerr << "Error: Failed to write to MUX2" << endl;
            }

        } else {

            cerr << "Error: Invalid multiplexer channel number: " 
                << muxChannelNum << endl;

        }
    }

    close(fd);
    return 0;

    // scan for cubes on each multiplexer channel and record their data in scene - done
    // compute activities - done
    // write activities to cubes - done
}
