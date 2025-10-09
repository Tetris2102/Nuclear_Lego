// particle.cpp

#include "particle.h"

void Particle::setEnergy(float newE) {
    E = newE;
}

float Particle::getEnergy() const {
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

float Particle::getMomX() const {
    return mom.x;
}

float Particle::getMomY() const {
    return mom.y;
}

float Particle::getMomZ() const {
    return mom.z;
}

void Particle::setPosition(Vector3 newPos) {
    position = newPos;
}

Vector3 Particle::getPosition() const {
    return position;
}

float Particle::getX() const {
    return position.x;
}

float Particle::getY() const {
    return position.y;
}

float Particle::getZ() const {
    return position.z;
}

Vector3 Particle::pointAlongVec(float t) const {
    Vector3 pointAlongVec;
    pointAlongVec.x = position.x + t * mom.x;
    pointAlongVec.y = position.y + t * mom.y;
    pointAlongVec.z = position.z + t * mom.z;
    return pointAlongVec;
}

void Particle::moveToPointAlong(float t) {
    setPosition(pointAlongVec(t));
}
