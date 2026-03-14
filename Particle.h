#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include <vector>

class Particle
{
private:
    std::string name;
    std::vector<double>* fourMomentum;

public:
    // Constructors and destructor
    Particle();
    Particle(const std::string& name, double E, double px, double py, double pz);
    ~Particle();

    // Rule of five
    Particle(const Particle& other);
    Particle& operator=(const Particle& other);
    Particle(Particle&& other) noexcept;
    Particle& operator=(Particle&& other) noexcept;

    // Getters
    const std::string& getName() const;
    double getE() const;
    double getPx() const;
    double getPy() const;
    double getPz() const;

    // Setters
    void setName(const std::string& name);
    void setE(double E);
    void setPx(double px);
    void setPy(double py);
    void setPz(double pz);

    // Operations
    Particle operator+(const Particle& other) const;
    double dotProduct(const Particle& other) const;
    void printData() const;
};

#endif // PARTICLE_H
