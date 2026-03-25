#ifndef PARTICLE_H
#define PARTICLE_H

#include <iosfwd>
#include <string>

#include "FourMomentum.h"

class Particle
{
private:
    std::string name;
    FourMomentum* fourMomentum;

public:
    Particle();
    Particle(const std::string& name, double E, double px, double py, double pz);
    ~Particle();

    Particle(const Particle& other);
    Particle& operator=(const Particle& other);
    Particle(Particle&& other) noexcept;
    Particle& operator=(Particle&& other) noexcept;

    const std::string& getName() const;
    double getE() const;
    double getPx() const;
    double getPy() const;
    double getPz() const;

    void setName(const std::string& name);
    void setE(double E);
    void setPx(double px);
    void setPy(double py);
    void setPz(double pz);

    // Total four-momentum of a system is a four-vector, not another named Particle.
    FourMomentum operator+(const Particle& other) const;
    double dotProduct(const Particle& other) const;
    double invariantMass() const;
};

std::ostream& operator<<(std::ostream& os, const Particle& p);

#endif
