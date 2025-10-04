#include "voxel.h"
#include "../Helpers/helpers.h"
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

void Voxel::moveToExit(Particle& p, double tmax) {
    p.setPosition(pointAlongVec(p.getPosition(), p.getMomentum(), tmax));
}

XSRecord Voxel::chooseEventAndXS(const Particle& p) {
    std::array<double, 3> xss = material.getEventXSs(p);
    std::discrete_distribution<int> discrete_dist{xss.begin(), xss.end()};
    XSRecord chosen = xss[discrete_dist(generator)];
    return xsChosen;
}

std::vector<Particle> Voxel::processParticle(Particle& p) {
    // Check if particle passes through Voxel
    if(!intersects(p)) return {};

    // Check if particle will undergo any event
    double tmax = intersectParams(p)[1];
    double prob = getTotalIntProb(p, tmax);
    if(uniform_real_dist(generator) > prob) return {};


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

std::array<double, 3> Voxel::getPosition() {
    return position;
}

void Voxel::setMaterial(const Material& m) {
    material = m;
}

Material Voxel::getMaterial() {
    return material;
}
