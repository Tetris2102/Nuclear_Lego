// xstable.cpp

#include "xstable.h"

EventType XSTable::getEventType() {
    return eventType;
}

ParticleType XSTable::getIncParticleType() {
    return incParticle;
}

ParticleType XSTable::getFinalParticleType() {
    return finalParticle;
}

int XSTable::getFinalParticles() {
    return finalParticleCount;
}

void XSTable::addXS(double energy, double xs) {
    eToXS[energy] = xs;
}

double XSTable::getXS(double energy) {
    auto it = eToXS.lower_bound(energy);

    if (it == eToXS.begin()) {
      return it->second;
    } else if (it == eToXS.end()) {
      return (it--)->second;
    } else {
      auto prevIt = std::prev(it);
      if (std::abs(energy - it->first) < std::abs(energy - prevIt->first)) {
        return it->second;
      } else {
        return prevIt->second;
      }
    }
}
