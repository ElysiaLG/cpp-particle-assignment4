#ifndef FOURMOMENTUM_H
#define FOURMOMENTUM_H

#include <iosfwd>
#include <vector>

class FourMomentum
{
private:
    std::vector<double>* p_;

public:
    FourMomentum();
    FourMomentum(double E, double px, double py, double pz);
    ~FourMomentum();

    FourMomentum(const FourMomentum& other);
    FourMomentum& operator=(const FourMomentum& other);
    FourMomentum(FourMomentum&& other) noexcept;
    FourMomentum& operator=(FourMomentum&& other) noexcept;

    double getE() const;
    double getPx() const;
    double getPy() const;
    double getPz() const;

    void setE(double E);
    void setPx(double px);
    void setPy(double py);
    void setPz(double pz);

    // Four-vector addition is defined on momenta; keep it here so Particle stays a wrapper.
    FourMomentum operator+(const FourMomentum& other) const;

    // Minkowski inner product with metric signature (+, -, -, -).
    double dot(const FourMomentum& other) const;

    // Invariant mass: m^2 = E^2 - px^2 - py^2 - pz^2 (units with c = 1).
    // Clamp negative m^2 to zero before sqrt when rounding makes m^2 slightly negative.
    double invariantMass() const;
};

std::ostream& operator<<(std::ostream& os, const FourMomentum& p);

#endif
