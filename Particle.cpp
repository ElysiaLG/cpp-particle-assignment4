#include "Particle.h"

#include <iostream>
#include <utility>

namespace
{
    const char* kDefaultParticleName = "electron";

    bool isValidParticleName(const std::string& name)
    {
        return name == "electron" || name == "muon" || name == "tau" ||
               name == "antielectron" || name == "antimuon" || name == "antitau";
    }

    std::string normalizedParticleName(const std::string& name)
    {
        if (isValidParticleName(name))
        {
            return name;
        }
        std::cerr << "Warning: Invalid particle name '" << name
                  << "'. Using '" << kDefaultParticleName << "'.\n";
        return kDefaultParticleName;
    }
}

Particle::Particle()
    : name(""), fourMomentum(new FourMomentum)
{
    std::cout << "Calling Particle default constructor\n";
}

Particle::Particle(const std::string& name, double E, double px, double py, double pz)
    : name(normalizedParticleName(name)), fourMomentum(nullptr)
{
    std::cout << "Calling Particle parameterized constructor\n";
    fourMomentum = new FourMomentum(E, px, py, pz);
}

Particle::~Particle()
{
    std::cout << "Calling Particle destructor\n";
    delete fourMomentum;
}

Particle::Particle(const Particle& other)
    : name(other.name),
      fourMomentum(other.fourMomentum ? new FourMomentum(*other.fourMomentum) : new FourMomentum)
{
    std::cout << "Calling Particle copy constructor\n";
}

Particle& Particle::operator=(const Particle& other)
{
    std::cout << "Calling Particle copy assignment operator\n";
    if (this != &other)
    {
        name = other.name;
        if (other.fourMomentum)
        {
            if (fourMomentum)
            {
                *fourMomentum = *other.fourMomentum;
            }
            else
            {
                fourMomentum = new FourMomentum(*other.fourMomentum);
            }
        }
        else
        {
            if (!fourMomentum)
            {
                fourMomentum = new FourMomentum;
            }
            else
            {
                *fourMomentum = FourMomentum{};
            }
        }
    }
    return *this;
}

Particle::Particle(Particle&& other) noexcept
    : name(std::move(other.name)), fourMomentum(other.fourMomentum)
{
    std::cout << "Calling Particle move constructor\n";
    other.fourMomentum = nullptr;
}

Particle& Particle::operator=(Particle&& other) noexcept
{
    std::cout << "Calling Particle move assignment operator\n";
    if (this != &other)
    {
        delete fourMomentum;
        name = std::move(other.name);
        fourMomentum = other.fourMomentum;
        other.fourMomentum = nullptr;
    }
    return *this;
}

void Particle::setName(const std::string& name)
{
    this->name = normalizedParticleName(name);
}

void Particle::setE(double E)
{
    fourMomentum->setE(E);
}

void Particle::setPx(double px)
{
    fourMomentum->setPx(px);
}

void Particle::setPy(double py)
{
    fourMomentum->setPy(py);
}

void Particle::setPz(double pz)
{
    fourMomentum->setPz(pz);
}

const std::string& Particle::getName() const
{
    return name;
}

double Particle::getE() const
{
    return fourMomentum->getE();
}

double Particle::getPx() const
{
    return fourMomentum->getPx();
}

double Particle::getPy() const
{
    return fourMomentum->getPy();
}

double Particle::getPz() const
{
    return fourMomentum->getPz();
}

std::ostream& operator<<(std::ostream& os, const Particle& p)
{
    os << p.getName() << " | E=" << p.getE() << ", px=" << p.getPx() << ", py=" << p.getPy()
       << ", pz=" << p.getPz();
    return os;
}

// Returning FourMomentum matches physics: adding particles yields total momentum, not a new species.
FourMomentum Particle::operator+(const Particle& other) const
{
    return (*fourMomentum) + (*other.fourMomentum);
}

// Delegates to FourMomentum::dot so the metric convention lives in one place.
double Particle::dotProduct(const Particle& other) const
{
    return fourMomentum->dot(*other.fourMomentum);
}

double Particle::invariantMass() const
{
    return fourMomentum->invariantMass();
}
