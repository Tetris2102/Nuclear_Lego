// voxel.cpp

#include "voxel.h"
#include "../Helpers/helpers.cpp"
#include "../Particle/particle.h"
#include <algorithm>
#include <cmath>
#include <utility>
#include <iostream>

float Voxel::getTotalIntProb(const Particle& p, float travelDist) {
    std::array<float, 3> xss = material.getEventXSs(p);
    float macroXSA = material.getADensity() * xss[0];
    float macroXSS = material.getADensity() * xss[1];
    float macroXSR = material.getADensity() * xss[2];

    float totMacroXS = macroXSA + macroXSS + macroXSR;
    return 1.0 - std::exp(-totMacroXS * travelDist);
}

XSRecord Voxel::chooseEventAndXS(const Particle& p) {
    std::array<XSRecord, 3> records = material.getEventRecords(p);
    std::array<float, 3> xss = {records[0].xs, records[1].xs, records[2].xs};
    std::discrete_distribution<int> discrete_dist{xss.begin(), xss.end()};
    XSRecord recordChosen = records[discrete_dist(*gen_ptr)];
    return recordChosen;
}

float Voxel::chooseIntDistAlong(float xs, float tmin, float tmax) {
    float u = (*uniform_real_dist_ptr)(*gen_ptr);
    float sigma = material.getADensity() * xs;  // Macroscopic cross-section
    // Use cumulative distribution function, i.e. probability of interaction
    // increases along particle path
    return tmin - (1/sigma) * std::log(1 - u*(1-std::exp(-sigma*(tmax-tmin))));
}

Vector3 Voxel::getScatterMomentum(const Vector3& oldMom, float energy) {
    float theta_max = REF_THETA / std::sqrt(energy);
    float theta = (*uniform_real_dist_ptr)(*gen_ptr) * theta_max;
    float phi = (*uniform_real_dist_ptr)(*gen_ptr) * TWO_PI;
    Vector3 randOrthVec = oldMom.randomOrthogonalV3((*uniform_real_dist_ptr)(*gen_ptr),
      (*uniform_real_dist_ptr)(*gen_ptr), (*uniform_real_dist_ptr)(*gen_ptr));
    Vector3 scatterMom = (oldMom * std::cos(theta) +
      randOrthVec.cross(oldMom) * std::sin(theta));
    scatterMom.normalize();
    return scatterMom;
}

void Voxel::setRNG(std::uniform_real_distribution<float>& dist,
  std::mt19937& gen) {
      uniform_real_dist_ptr = &dist;
      gen_ptr = &gen;
}

std::vector<Particle> Voxel::processParticle(Particle& p) {
    // Check if particle passes through Voxel
    if(!intersects(p)) return {};

    // Check if particle will undergo any event
    float tmin = intersectParams(p)[0];
    float tmax = intersectParams(p)[1];
    float prob = getTotalIntProb(p, tmax-tmin);
    if((*uniform_real_dist_ptr)(*gen_ptr) > prob) {
        // Same as moveToExit(p);
        p.moveToPointAlong(tmax);  // Particle just passes through
        return {};
    }

    // Choose record to use (including EventType, cross-section, etc.)
    XSRecord record = chooseEventAndXS(p);

    std::vector<Particle> particlesCreated;

    if(record.event == SCATTER) {
        float intDistAlong = chooseIntDistAlong(record.xs, tmin, tmax);
        p.moveToPointAlong(intDistAlong);
        p.setMomentum(getScatterMomentum(p.getMomentum(), p.getEnergy()));
        float tmaxScatter = intersectParams(p)[1];
        p.moveToPointAlong(tmaxScatter); // Advance particle to exit of Voxel
        std::cout << "Scatter" << std::endl;
    } else if(record.event == ABSORB) {
        // No need in two lines below since particle is deactivated anyway
        // float t = getIntDistAlong(record.xs, tmin, tmax);
        // p.moveToPointAlong(t);

        // Detect particle if applicable
        if(type == DETECTOR) {
            particlesAbsorbed.push_back(p);
        }
        p.deactivate();
        std::cout << "Absorb" << std::endl;
    } else {
        float particleEnergy = p.getEnergy() / record.finalParticleCount;
        Particle newP(NONE, particleEnergy,
          Vector3{0.0, 0.0, 0.0}, Vector3{0.0, 0.0, 0.0});
        float intDistAlong = chooseIntDistAlong(record.xs, tmin, tmax);

        // Iterate to create particles
        for(int i = 0; i<record.finalParticleCount; i++) {
            newP.setType(record.finalParticle);
            newP.moveToPointAlong(intDistAlong);
            newP.setMomentum(getScatterMomentum(p.getMomentum(), p.getEnergy()));
            particlesCreated.push_back(newP);
        }

        p.deactivate();  // Incident particle absorbed
    }

    return particlesCreated;
}

void Voxel::moveToExit(Particle& p) const {
    // How far the point of exit is from origin along momentum Vector3
    float exitAlongMom = intersectParams(p)[1];
    p.moveToPointAlong(exitAlongMom);
}

bool Voxel::intersects(const Particle& p) {
    float xmin, ymin, zmin;
    xmin = position.x - halfSide;
    ymin = position.y - halfSide;
    zmin = position.z - halfSide;

    float xmax, ymax, zmax;
    xmax = position.x + halfSide;
    ymax = position.y + halfSide;
    zmax = position.z + halfSide;

    float x = p.getX();
    float y = p.getY();
    float z = p.getZ();

    float dx = p.getMomX();
    float dy = p.getMomY();
    float dz = p.getMomZ();

    // Normalize magnitude
    normalizeVecXYZ(dx, dy, dz);

    if(dx == 0) {
        if(x <= xmin || x >= xmax) {
            return false;
        } else {
            return true;
        }
    }
    if(dy == 0) {
        if(y <= ymin || y >= ymax) {
            return false;
        } else {
            return true;
        }
    }
    if(dz == 0) {
        if(z <= zmin || z >= zmax) {
            return false;
        } else {
            return true;
        }
    }

    float t0x, t1x, t0y, t1y, t0z, t1z;

    if(dx > 0) {
        t0x = (xmin - x) / dx;
        t1x = (xmax - x) / dx;
    } else {
        // Swap t0x and t1x
        t1x = (xmin - x) / dx;
        t0x = (xmax - x) / dx;
    }

    if(dy > 0) {
        t0y = (ymin - y) / dy;
        t1y = (ymax - y) / dy;
    } else {
        // Swap t0y and t1y
        t1y = (ymin - y) / dy;
        t0y = (ymax - y) / dy;
    }

    if(dz > 0) {
        t0z = (zmin - z) / dz;
        t1z = (zmax - z) / dz;
    } else {
        // Swap t0z and t1z
        t1z = (zmin - z) / dz;
        t0z = (zmax - z) / dz;
    }

    float tmin = std::max({t0x, t0y, t0z});
    float tmax = std::min({t1x, t1y, t1z});

    return (tmax > tmin) ? true : false;
}

std::array<float, 2> Voxel::intersectParams(const Particle& p) const {
    float xmin, ymin, zmin;
    xmin = position.x - halfSide;
    ymin = position.y - halfSide;
    zmin = position.z - halfSide;

    float xmax, ymax, zmax;
    xmax = position.x + halfSide;
    ymax = position.y + halfSide;
    zmax = position.z + halfSide;

    float x = p.getX();
    float y = p.getY();
    float z = p.getZ();

    float dx = p.getMomX();
    float dy = p.getMomY();
    float dz = p.getMomZ();

    // Normalize magnitude
    normalizeVecXYZ(dx, dy, dz);

    bool ignoreX, ignoreY, ignoreZ;
    ignoreX = (dx == 0);
    ignoreY = (dy == 0);
    ignoreZ = (dz == 0);

    float t0x, t1x, t0y, t1y, t0z, t1z;

    if(dx > 0) {
        t0x = (xmin - x) / dx;
        t1x = (xmax - x) / dx;
    } else {
        // Swap t0x and t1x
        t1x = (xmin - x) / dx;
        t0x = (xmax - x) / dx;
    }

    if(dy > 0) {
        t0y = (ymin - y) / dy;
        t1y = (ymax - y) / dy;
    } else {
        // Swap t0y and t1y
        t1y = (ymin - y) / dy;
        t0y = (ymax - y) / dy;
    }

    if(dz > 0) {
        t0z = (zmin - z) / dz;
        t1z = (zmax - z) / dz;
    } else {
        // Swap t0z and t1z
        t1z = (zmin - z) / dz;
        t0z = (zmax - z) / dz;
    }

    std::vector<float> tMinArgs;
    std::vector<float> tMaxArgs;
    if(!ignoreX) {
        tMinArgs.push_back(t0x);
        tMaxArgs.push_back(t1x);
    }
    if(!ignoreY) {
        tMinArgs.push_back(t0y);
        tMaxArgs.push_back(t1y);
    }
    if(!ignoreZ) {
        tMinArgs.push_back(t0z);
        tMaxArgs.push_back(t1z);
    }

    auto tminIt = std::max_element(tMinArgs.begin(), tMinArgs.end());
    auto tmaxIt = std::min_element(tMaxArgs.begin(), tMaxArgs.end());

    float tmin = *tminIt;
    float tmax = *tmaxIt;

    return {tmin, tmax};
}

VoxelType Voxel::getType() const {
    return type;
}

void Voxel::setPosition(const Vector3& newPos) {
    position = newPos;
}

Vector3 Voxel::getPosition() {
    return position;
}

void Voxel::setMaterial(const Material& newMat) {
    material = newMat;
}

Material Voxel::getMaterial() {
    return material;
}

std::vector<Particle> Voxel::getPartsEmittedList(float timeElapsed) {
    assert(type == SOURCE);
    return sample.generateParticles(timeElapsed, position,
      *uniform_real_dist_ptr, *gen_ptr);
}

int Voxel::getPartsEmitted(float time) {
    assert(type == SOURCE);
    return getPartsEmittedList(time).size();
}

std::vector<Particle> Voxel::getPartsAbsorbedList() {
    assert(type == DETECTOR);
    return particlesAbsorbed;
}

int Voxel::getPartsAbsorbed() {
    assert(type == DETECTOR);
    return particlesAbsorbed.size();
}

void Voxel::clearPartsAbsorbed() {
    assert(type == DETECTOR);
    particlesAbsorbed = {};
}
