#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>  // for std::hex and std::showbase
#include <array>
#include <cstring>

#define MUX1_ADDR 0x70
#define MUX2_ADDR 0x71

using namespace std;

void selectMux(uint8_t addr, uint8_t channel);
void deactivateMux(uint8_t addr);

int file;

int main() {

    const char* filename = "/dev/i2c-1";
    array<uint8_t, 3> slaveAddrs = {0x08, 0x09, 0x0A};

    if((file = open(filename, O_RDWR)) < 0) {
        cerr << "Failed to initialize i2c" << endl;
        return 1;
    }

    cout << "Scanning for i2c devices..." << endl;

    cout << "\nMultiplexer 1:" << endl;
    for(uint8_t i = 0; i<8; i++) {
        selectMux(MUX1_ADDR, i);  // Select channel i
        usleep(1000);  // Small delay for mux to settle
        for(const auto& addr : slaveAddrs) {
            if(ioctl(file, I2C_SLAVE, addr) >= 0) {
                // Try to read from device to verify it's present
                unsigned char buffer[5];
                if(read(file, buffer, 5) >= 0) {
                    cout << "Found device at 0x" << hex << (int)addr 
                         << " on channel " << dec << (int)i << endl;
                }
            }
        }
        deactivateMux(MUX1_ADDR);
    }

    cout << "\nMultiplexer 2:" << endl;
    for(uint8_t i = 0; i<8; i++) {
        selectMux(MUX2_ADDR, i);  // Select channel i
        usleep(1000);  // Small delay for mux to settle
        for(const auto& addr : slaveAddrs) {
            if(ioctl(file, I2C_SLAVE, addr) >= 0) {
                // Try to read from device to verify it's present
                unsigned char buffer[5];
                if(read(file, buffer, 5) >= 0) {
                    cout << "Found device at 0x" << hex << (int)addr 
                         << " on channel " << dec << (int)i << endl;
                }
            }
        }
        deactivateMux(MUX2_ADDR);
    }

    cout << "Finished scanning." << endl;

    close(file);

    return 0;
}

void selectMux(uint8_t addr, uint8_t channel) {
    if(ioctl(file, I2C_SLAVE, addr) < 0) {
        cerr << "Failed to set multiplexer slave address 0x" 
             << showbase << hex << (int)addr << endl;
        return;
    }
    unsigned char buffer[1] = {static_cast<unsigned char>(1 << channel)};
    if(write(file, buffer, 1) != 1) {
        cerr << "Failed to select channel " << dec << (int)channel 
             << " for multiplexer at 0x" << hex << (int)addr << endl;
    }
}

void deactivateMux(uint8_t addr) {
    if(ioctl(file, I2C_SLAVE, addr) < 0) {
        cerr << "Failed to set multiplexer slave address 0x" 
             << showbase << hex << (int)addr << endl;
        return;
    }
    unsigned char buffer[1] = {0b00000000};
    if(write(file, buffer, 1) != 1) {
        cerr << "Failed to deactivate multiplexer at 0x" 
             << hex << (int)addr << endl;
    }
}
