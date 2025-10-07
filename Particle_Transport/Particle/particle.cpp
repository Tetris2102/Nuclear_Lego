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

void Particle::setMomentum(Vector3 newMom) {
    mom = newMom;
}

Vector3 Particle::getMomentum() {
    return mom;
}

double Particle::getMomX() {
    return mom.x;
}

double Particle::getMomY() {
    return mom.y;
}

double Particle::getMomZ() {
    return mom.z;
}

void Particle::setPosition(Vector3 newPos) {
    position = newPos;
}

Vector3 Particle::getPosition() {
    return position;
}

double Particle::getX() {
    return position.x;
}

double Particle::getY() {
    return position.y;
}

double Particle::getZ() {
    return position.z;
}

Vector3 Particle::pointAlongVec(Vector3 origin,
  Vector3 dirXYZ, double tmax) {
    Vector3 pointAlongVec;
    pointAlongVec.x = origin.x + tmax * dirXYZ.x;
    pointAlongVec.y = origin.y + tmax * dirXYZ.y;
    pointAlongVec.z = origin.z + tmax * dirXYZ.z;
    return pointAlongVec;
}

void Particle::moveToPointAlong(double t) {
    setPosition(pointAlongVec(getPosition(), getMomentum(), t));
}
