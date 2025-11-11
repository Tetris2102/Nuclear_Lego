// particle.cpp

#include "particleGroup.h"

void ParticleGroup::setEnergy(float newE) {
    E = newE;
}

float ParticleGroup::getEnergy() const {
    return E;
}

void ParticleGroup::setType(ParticleType newType) {
    type = newType;
}

ParticleType ParticleGroup::getType() const {
    return type;
}

void ParticleGroup::deactivate() {
    exists = false;
}

bool ParticleGroup::isActive() const {
    return exists;
}

void ParticleGroup::setMomentum(Vector3 newMom) {
    mom = newMom;
}

Vector3 ParticleGroup::getMomentum() const {
    return mom;
}

float ParticleGroup::getMomX() const {
    return mom.x;
}

float ParticleGroup::getMomY() const {
    return mom.y;
}

float ParticleGroup::getMomZ() const {
    return mom.z;
}

void ParticleGroup::setPosition(Vector3 newPos) {
    position = newPos;
}

Vector3 ParticleGroup::getPosition() const {
    return position;
}

float ParticleGroup::getX() const {
    return position.x;
}

float ParticleGroup::getY() const {
    return position.y;
}

float ParticleGroup::getZ() const {
    return position.z;
}

void ParticleGroup::setGroupSize(uint16_t newNumParticles) {
    numParticles = newNumParticles;
}

uint16_t ParticleGroup::getGroupSize() {
    return numParticles;
}

Vector3 ParticleGroup::pointAlongVec(float t) const {
    Vector3 pointAlongVec;
    pointAlongVec.x = position.x + t * mom.x;
    pointAlongVec.y = position.y + t * mom.y;
    pointAlongVec.z = position.z + t * mom.z;
    return pointAlongVec;
}

void ParticleGroup::moveToPointAlong(float t) {
    setPosition(pointAlongVec(t));
}
