// particle.cpp

#include "particle.h"

void Particle::setEnergy(double newE) {
    E = newE;
}

double Particle::getEnergy() {
    return E;
}

void Particle::setType(ParticleType newType) {
    type = newType;
}

ParticleType Particle::getType() {
    return type;
}

void Particle::deactivate() {
    exists = false;
}

bool Particle::isActive() {
    return exists;
}

void Particle::setMomentum(std::array<double, 3> newMom) {
    mom = newMom;
}

std::array<double, 3> Particle::getMomentum() {
    return mom;
}

double Particle::getMomX() {
    return mom[0];
}

double Particle::getMomY() {
    return mom[1];
}

double Particle::getMomZ() {
    return mom[2];
}

void Particle::setPosition(std::array<double, 3> newPos) {
    position = newPos;
}

std::array<double, 3> Particle::getPosition() {
    return position;
}

double Particle::getX() {
    return position[0];
}

double Particle::getY() {
    return position[1];
}

double Particle::getZ() {
    return position[2];
}
