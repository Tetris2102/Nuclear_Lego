#include <iostream>
#include "particle.h"

int main() {
    Particle p(ALPHA, 2.0, {0.62, 3.5, 0.12}, {1.0, 0.0, 0.0});
    std::cout << "Particle type: " << p.getType() << std::endl;
    std::cout << "Energy: " << p.getEnergy() << std::endl;
    auto momentum = p.getMomentum();
    std::cout << "Momentum: (" << momentum[0] << ", " << momentum[1] << ", " << momentum[2] << ")" << std::endl;
    return 0;
}
