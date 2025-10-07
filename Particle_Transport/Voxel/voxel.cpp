#include "voxel.h"
#include "../Helpers/helpers.h"
#include "../Particle/particle.h"
#include <algorithm>
#include <cmath>
#include <utility>

double Voxel::getTotalIntProb(const Particle& p, double travelDist) {
    std::array<double, 3> xss = material.getEventXSs(p);
    double macroXSA = 1.0 / (material.getADensity() * xss[0]);
    double macroXSS = 1.0 / (material.getADensity() * xss[1]);
    double macroXSR = 1.0 / (material.getADensity() * xss[2]);

    double totMacroXS = macroXSA + macroXSS + macroXSR;
    return std::exp(-totMacroXS * travelDist);
}

XSRecord Voxel::chooseEventAndXS(const Particle& p) {
    std::array<XSRecord, 3> records = material.getEventRecords(p);
    std::array<double, 3> xss = {records[0].xs, records[1].xs, records[2].xs};
    std::discrete_distribution<int> discrete_dist{xss.begin(), xss.end()};
    XSRecord recordChosen = records[discrete_dist(gen)];
    return recordChosen;
}

double Voxel::chooseIntDistAlong(double xs, double tmin, double tmax) {
    double u = uniform_real_dist(gen);
    double sigma = material.getADensity() * xs;  // Macroscopic cross-section
    // Use cumulative distribution function, i.e. probability of interaction
    // increases along particle path
    return tmin - (1/sigma) * std::log(1 - u*(1-std::exp(-sigma*(tmax-tmin))));
}

Vector3 Voxel::getScatterMomentum(const Vector3& oldMom, double energy) {
    double theta_max = REF_THETA / std::sqrt(energy);
    double theta = uniform_real_dist(gen) * theta_max;
    double phi = uniform_real_dist(gen) * TWO_PI;
    Vector3 randOrthVec = oldMom.randomOrthogonalV3(uniform_real_dist(gen),
      uniform_real_dist(gen), uniform_real_dist(gen));
    Vector3 scatterMom = (oldMom * std::cos(theta) +
      randOrthVec.cross(oldMom) * std::sin(theta));
    return scatterMom.normalize();
}

std::vector<Particle> Voxel::processParticle(Particle& p) {
    // Check if particle passes through Voxel
    if(!intersects(p)) return {};

    // Check if particle will undergo any event
    double tmin = intersectParams(p)[0];
    double tmax = intersectParams(p)[1];
    double prob = getTotalIntProb(p, tmax);
    if(uniform_real_dist(generator) > prob) {
        p.moveToPointAlong(tmax);  // Particle just passes through
        return {};
    }

    // Choose record to use (including EventType, cross-section, etc.)
    XSRecord record = chooseEventAndXS(p);

    std::vector<Particle> particlesCreated;

    if(record.event == SCATTER) {
        double intDistAlong = chooseIntDistAlong(record.xs, tmin, tmax);
        p.moveToPointAlong(intDistAlong);
        p.setMomentum(getScatterMomentum(p.getMomentum(), p.getEnergy()));
        tmax = intersectParams[1];
        p.moveToPointAlong(tmax); // Advance particle to exit of Voxel
    } else if(record.event == ABSORB) {
        // No need in two lines below since particle is deactivated anyway
        // double t = getIntDistAlong(record.xs, tmin, tmax);
        // p.moveToPointAlong(t);

        // Detect particle if applicable
        if(type == DETECTOR) {
            particlesAbsorbed.push_back(p);
        }
        p.deactivate();
    } else {
        double particleEnergy = p.getEnergy() / finalParticleCount;
        Particle newP(NONE, particleEnergy,
          Vector3{0.0, 0.0, 0.0}, Vector3{0.0, 0.0, 0.0});
        double intDistAlong = chooseIntDistAlong(record.xs, tmin, tmax);

        // Iterate to create particles
        for(int i = 0; i<record.finalParticleCount; i++) {
            newP.setType(record.finalParticle);
            newP.setOrigin(p.pointAlongVec(intDistAlong));
            newP.setMomentum(getScatterMomentum(p.getMomentum(), p.getEnergy()));
            particlesCreated.push_back(newP);
        }

        p.deactivate();  // Incident particle absorbed
    }

    return particlesCreated;
}

bool Voxel::intersects(const Particle& p) {
    double xmin, ymin, zmin;
    xmin = position[0] - halfSide;
    ymin = position[1] - halfSide;
    zmin = position[2] - halfSide;

    double xmax, ymax, zmax;
    xmax = position[0] + halfSide;
    ymax = position[1] + halfSide;
    zmax = position[2] + halfSide;

    double x = p.getX();
    double y = p.getY();
    double z = p.getZ();

    double dx = p.getMomX();
    double dy = p.getMomY();
    double dz = p.getMomZ();

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

    double t0x, t1x, t0y, t1y, t0z, t1z;

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

    double tmin = std::max({t0x, t0y, t0z});
    double tmax = std::min({t1x, t1y, t1z});

    return (tmax > tmin) ? true : false;
}

std::array<double, 2> Voxel::intersectParams(const Particle& p) {
    double xmin, ymin, zmin;
    xmin = position[0] - halfSide;
    ymin = position[1] - halfSide;
    zmin = position[2] - halfSide;

    double xmax, ymax, zmax;
    xmax = position[0] + halfSide;
    ymax = position[1] + halfSide;
    zmax = position[2] + halfSide;

    double x = p.getX();
    double y = p.getY();
    double z = p.getZ();

    double dx = p.getMomX();
    double dy = p.getMomY();
    double dz = p.getMomZ();

    // Normalize magnitude
    normalizeVecXYZ(dx, dy, dz);

    bool ignoreX, ignoreY, ignoreZ;
    ignoreX = (dx == 0);
    ignoreY = (dy == 0);
    ignoreZ = (dz == 0);

    double t0x, t1x, t0y, t1y, t0z, t1z;

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

    std::vector<double> tMinArgs;
    std::vector<double> tMaxArgs;
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

    double tmin = *tminIt;
    double tmax = *tmaxIt;

    return {tmin, tmax};
}

Vector3 Voxel::getPosition() {
    return position;
}

void Voxel::setMaterial(const Material& m) {
    material = m;
}

Material Voxel::getMaterial() {
    return material;
}

std::vector<Particle> Voxel::getParticlesAbsorbed() {
    return particlesAbsorbed;
}

int Voxel::getParticlesAbsorbed() {
    return particlesAbsorbed.size();
}

std::vector<Particle> Voxel::getAndEraseParticlesAbsorbed() {
    std::vector<Particle> copy = particlesAbsorbed;
    particlesAbsorbed = {};
    return copy;
}

int Voxel::getAndEraseParticlesAbsorbed() {
    int size = particlesAbsorbed.size();
    particlesAbsorbed = {};
    return size;
}
