#include "FourMomentum.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace
{
    void validateEnergy(double E)
    {
        if (E < 0)
        {
            std::cerr << "Error: Energy E cannot be negative (got " << E << ").\n";
            std::exit(1);
        }
    }
}

FourMomentum::FourMomentum()
    : p_(new std::vector<double>{0.0, 0.0, 0.0, 0.0})
{
}

FourMomentum::FourMomentum(double E, double px, double py, double pz)
    : p_(nullptr)
{
    validateEnergy(E);
    p_ = new std::vector<double>{E, px, py, pz};
}

FourMomentum::~FourMomentum()
{
    std::cout << "Calling FourMomentum destructor\n";
    delete p_;
}

FourMomentum::FourMomentum(const FourMomentum& other)
    : p_(new std::vector<double>(other.p_ ? *other.p_
                                            : std::vector<double>{0.0, 0.0, 0.0, 0.0}))
{
    std::cout << "Calling FourMomentum copy constructor\n";
}

FourMomentum& FourMomentum::operator=(const FourMomentum& other)
{
    std::cout << "Calling FourMomentum copy assignment operator\n";
    if (this != &other)
    {
        if (other.p_)
        {
            if (p_)
            {
                *p_ = *other.p_;
            }
            else
            {
                p_ = new std::vector<double>(*other.p_);
            }
        }
        else
        {
            if (!p_)
            {
                p_ = new std::vector<double>{0.0, 0.0, 0.0, 0.0};
            }
            else
            {
                *p_ = std::vector<double>{0.0, 0.0, 0.0, 0.0};
            }
        }
    }
    return *this;
}

FourMomentum::FourMomentum(FourMomentum&& other) noexcept
    : p_(other.p_)
{
    std::cout << "Calling FourMomentum move constructor\n";
    other.p_ = nullptr;
}

FourMomentum& FourMomentum::operator=(FourMomentum&& other) noexcept
{
    std::cout << "Calling FourMomentum move assignment operator\n";
    if (this != &other)
    {
        delete p_;
        p_ = other.p_;
        other.p_ = nullptr;
    }
    return *this;
}

double FourMomentum::getE() const
{
    return (*p_)[0];
}

double FourMomentum::getPx() const
{
    return (*p_)[1];
}

double FourMomentum::getPy() const
{
    return (*p_)[2];
}

double FourMomentum::getPz() const
{
    return (*p_)[3];
}

void FourMomentum::setE(double E)
{
    validateEnergy(E);
    (*p_)[0] = E;
}

void FourMomentum::setPx(double px)
{
    (*p_)[1] = px;
}

void FourMomentum::setPy(double py)
{
    (*p_)[2] = py;
}

void FourMomentum::setPz(double pz)
{
    (*p_)[3] = pz;
}

FourMomentum FourMomentum::operator+(const FourMomentum& other) const
{
    return FourMomentum(
        getE() + other.getE(),
        getPx() + other.getPx(),
        getPy() + other.getPy(),
        getPz() + other.getPz());
}

double FourMomentum::dot(const FourMomentum& other) const
{
    return getE() * other.getE() - getPx() * other.getPx()
         - getPy() * other.getPy() - getPz() * other.getPz();
}

double FourMomentum::invariantMass() const
{
    const double mSq = getE() * getE() - getPx() * getPx() - getPy() * getPy() - getPz() * getPz();
    return std::sqrt(std::max(0.0, mSq));
}

std::ostream& operator<<(std::ostream& os, const FourMomentum& p)
{
    os << "E=" << p.getE() << ", px=" << p.getPx() << ", py=" << p.getPy() << ", pz=" << p.getPz();
    return os;
}
