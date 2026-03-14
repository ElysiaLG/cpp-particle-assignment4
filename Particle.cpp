#include "Particle.h"
#include <iostream>
#include <cstdlib>
#include <utility>

namespace
{
    bool isValidParticleName(const std::string& name)
    {
        return name == "electron" || name == "muon" || name == "tau" ||
               name == "photon" || name == "proton" || name == "neutron";
    }

    void validateParticleName(const std::string& name)
    {
        if (!isValidParticleName(name))
        {
            std::cerr << "Error: Invalid particle name '" << name << "'. "
                      << "Valid names include: electron, muon, tau.\n";
            std::exit(1);
        }
    }

    void validateEnergy(double E)
    {
        if (E < 0)
        {
            std::cerr << "Error: Energy E cannot be negative (got " << E << ").\n";
            std::exit(1);
        }
    }
}

Particle::Particle()
    : name(""), fourMomentum(new std::vector<double>{0.0, 0.0, 0.0, 0.0})
{
    std::cout << "Calling default constructor\n";
}

Particle::Particle(const std::string& name, double E, double px, double py, double pz)
    : name(name), fourMomentum(nullptr)
{
    std::cout << "Calling parameterized constructor\n";
    validateParticleName(name);
    validateEnergy(E);
    fourMomentum = new std::vector<double>{E, px, py, pz};
}

Particle::~Particle()
{
    std::cout << "Calling destructor\n";
    delete fourMomentum;
}

Particle::Particle(const Particle& other)
    : name(other.name), fourMomentum(new std::vector<double>(*other.fourMomentum))
{
    std::cout << "Calling copy constructor\n";
}

Particle& Particle::operator=(const Particle& other)
{
    std::cout << "Calling copy assignment operator\n";
    if (this != &other)
    {
        name = other.name;
        if (fourMomentum)
        {
            *fourMomentum = *other.fourMomentum;
        }
        else
        {
            fourMomentum = new std::vector<double>(*other.fourMomentum);
        }
    }
    return *this;
}

Particle::Particle(Particle&& other) noexcept
    : name(std::move(other.name)), fourMomentum(other.fourMomentum)
{
    std::cout << "Calling move constructor\n";
    other.fourMomentum = nullptr;
}

Particle& Particle::operator=(Particle&& other) noexcept
{
    std::cout << "Calling move assignment operator\n";
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
    validateParticleName(name);
    this->name = name;
}

void Particle::setE(double E)
{
    validateEnergy(E);
    (*fourMomentum)[0] = E;
}

void Particle::setPx(double px)
{
    (*fourMomentum)[1] = px;
}

void Particle::setPy(double py)
{
    (*fourMomentum)[2] = py;
}

void Particle::setPz(double pz)
{
    (*fourMomentum)[3] = pz;
}

const std::string& Particle::getName() const
{
    return name;
}

double Particle::getE() const
{
    return (*fourMomentum)[0];
}

double Particle::getPx() const
{
    return (*fourMomentum)[1];
}

double Particle::getPy() const
{
    return (*fourMomentum)[2];
}

double Particle::getPz() const
{
    return (*fourMomentum)[3];
}

void Particle::printData() const
{
    std::cout << "Particle: " << name
              << " | E=" << getE()
              << ", px=" << getPx()
              << ", py=" << getPy()
              << ", pz=" << getPz() << "\n";
}

Particle Particle::operator+(const Particle& other) const
{
    return Particle(name, getE() + other.getE(), getPx() + other.getPx(),
                    getPy() + other.getPy(), getPz() + other.getPz());
}

double Particle::dotProduct(const Particle& other) const
{
    return getE() * other.getE() - getPx() * other.getPx()
         - getPy() * other.getPy() - getPz() * other.getPz();
}
