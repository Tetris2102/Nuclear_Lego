// particle.cpp

#include "particle.h"

void Particle::setEnergy(double newE) {
    E = newE;
}

double Particle::getEnergy() const {
    return E;
}

void Particle::setType(ParticleType newType) {
    type = newType;
}

ParticleType Particle::getType() const {
    return type;
}

void Particle::deactivate() {
    exists = false;
}

bool Particle::isActive() const {
    return exists;
}

void Particle::setMomentum(Vector3 newMom) {
    mom = newMom;
}

Vector3 Particle::getMomentum() const {
    return mom;
}

double Particle::getMomX() const {
    return mom.x;
}

double Particle::getMomY() const {
    return mom.y;
}

double Particle::getMomZ() const {
    return mom.z;
}

void Particle::setPosition(Vector3 newPos) {
    position = newPos;
}

Vector3 Particle::getPosition() const {
    return position;
}

double Particle::getX() const {
    return position.x;
}

double Particle::getY() const {
    return position.y;
}

double Particle::getZ() const {
    return position.z;
}

Vector3 Particle::pointAlongVec(double t) const {
    Vector3 pointAlongVec;
    pointAlongVec.x = position.x + t * mom.x;
    pointAlongVec.y = position.y + t * mom.y;
    pointAlongVec.z = position.z + t * mom.z;
    return pointAlongVec;
}

void Particle::moveToPointAlong(double t) {
    setPosition(pointAlongVec(t));
}
