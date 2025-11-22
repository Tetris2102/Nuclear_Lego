#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>  // for std::hex and std::showbase
#include <array>

#define MUX1_ADDR 0x70
#define MUX2_ADDR 0x71

using namespace std;

void selectMux(uint8_t addr, uint8_t channel);
void deactivateMux(uint8_t addr);

int file;

int main() {

    char* filename = "dev/i2c-1";
    array<uint8_t, 3> slaveAddrs = {0x08, 0x09, 0x0A};

    if((file = open(filename, I2C_RDWR)) < 0) {
        cerr << "Failed to initialize i2c" << endl;
        return 1;
    }

    cout << "Scanning for i2c devices..." << endl;

    cout << "\nMultiplexer 1:" << endl;
    for(uint8_t i = 0; i<8; i++) {
        selectMux(MUX1_ADDR, i);  // Select channel i
        for(const auto& addr : slaveAddrs) {
            if(ioctl(file, I2C_SLAVE, addr) >= 0) {
                cout << "Found device at " << showbase << hex << addr << endl;
            }
        }
        deactivateMux(i);
    }

    cout << "\nMultiplexer 2:" << endl;
    for(uint8_t i = 0; i<8; i++) {
        selectMux(MUX2_ADDR, i);  // Select channel i
        for(const auto& addr : slaveAddrs) {
            if(ioctl(file, I2C_SLAVE, addr) >= 0) {
                cout << "Found device at " << showbase << hex << addr << endl;
            }
        }
        deactivateMux(i);
    }

    cout << "Finished scanning." << endl;

    close(file);

    return 0;
}

void selectMux(uint8_t addr, uint8_t channel) {
    unsigned char buffer[1] = {0b00000000};
    buffer[0] << channel;
    if(write(file, buffer, 1) != 1) {
        cerr << "Failed to select channel for multiplexer at "
          << showbase << hex << addr << endl;
    }
}

void deactivateMux(uint8_t addr) {
    unsigned char buffer[1] = {0b00000001};
    if(write(file, buffer, 1) != 1) {
        cerr << "Failed to select channel for multiplexer at "
          << showbase << hex << addr << endl;
    }
}
