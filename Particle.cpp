/*
 * ===========================================================================
 *  STUDENT DETAILS
 * ===========================================================================
 *  Name:     Jindong Wang
 *  ID:       11072535
 *  Course:   PHYS30762 & PHYS63762 Object-Oriented Programming in C++
 *            2025/26 2nd Semester
 *  Topic:    Particle and Four-Momentum System (Assignment 4)
 *  Date:     25/03/2026
 *
 * ===========================================================================
 *  PURPOSE OF THIS FILE
 * ===========================================================================
 *  Defines Particle member functions and the stream insertion operator.
 *  Implements name validation, on-shell energy updates, four-momentum
 *  addition and dot product, and Rule of Five with trace output for marking.
 *
 * ===========================================================================
 */

#include "Particle.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
    /*
     * Function:  validateParticleName
     * Purpose:   Ensure particle name is one of the coursework-allowed strings.
     * Input:     const std::string& name — user- or code-supplied label.
     * Output:    void (throws if invalid).
     * Method:    Linear search over kAllowed; on mismatch, throw
     *            std::invalid_argument including the rejected name.
     */
    void validateParticleName(const std::string& name)
    {
        static const char* const kAllowed[] = {
            "electron", "muon", "tau", "antielectron", "antimuon", "antitau"};
        for (const char* allowed : kAllowed)
        {
            if (name == allowed)
            {
                return;
            }
        }

        throw std::invalid_argument("Invalid particle name: " + name);
    }
}

/*
 * Function:  computeEnergy
 * Purpose:   Relativistic on-shell energy from mass and three-momentum.
 * Input:     restMass, px, py, pz — all double, units MeV with c = 1.
 * Output:    double — positive square root of m^2 + p^2.
 * Method:    E = sqrt(m*m + px*px + py*py + pz*pz); branch-free aside from sqrt.
 */
double Particle::computeEnergy(double restMass, double px, double py, double pz) const
{
    return std::sqrt(restMass * restMass + px * px + py * py + pz * pz);
}

/*
 * Function:  restMass
 * Purpose:   Return tabulated rest mass for current name.
 * Input:     (none) — reads member name.
 * Output:    double — MeV; 0 for empty name or unmatched fallback.
 * Method:    Table lookup by string compare; antiparticles share mass with
 *            partners in this simplified model.
 */
double Particle::restMass() const
{
    if (name.empty())
    {
        return 0.0;
    }
    if (name == "electron" || name == "antielectron")
    {
        return 0.511;
    }
    if (name == "muon" || name == "antimuon")
    {
        return 105.7;
    }
    if (name == "tau" || name == "antitau")
    {
        return 1777.0;
    }
    return 0.0;
}

/*
 * Function:  refreshEnergy
 * Purpose:   Sync E with (name -> m) and current (px, py, pz) in momentum_.
 * Input:     (none).
 * Output:    void.
 * Method:    computeEnergy then momentum_.setE; setE validates E >= 0.
 */
void Particle::refreshEnergy()
{
    momentum_.setE(
        computeEnergy(restMass(), momentum_.getPx(), momentum_.getPy(), momentum_.getPz()));
}

/*
 * Function:  Particle (default constructor)
 * Purpose:   Placeholder particle (empty name, zero four-momentum).
 * Input:     (none).
 * Output:    (object constructed).
 * Method:    value-initialises name and default-constructs momentum_. Prints
 *            trace line for coursework marking.
 */
Particle::Particle() : name(""), momentum_()
{
    std::cout << "Calling Particle constructor\n";
}

/*
 * Function:  Particle (parameterized constructor)
 * Purpose:   Construct named particle with three-momentum; E from on-shell relation.
 * Input:     const std::string& name — must be allow-listed; px, py, pz — double.
 * Output:    (object constructed).
 * Method:    validateParticleName; set three-momentum components; refreshEnergy
 *            writes E via FourMomentum::setE. User does not pass E directly.
 */
Particle::Particle(const std::string& name, double px, double py, double pz)
    : name(""), momentum_()
{
    validateParticleName(name);
    std::cout << "Calling Particle constructor\n";
    this->name = name;
    momentum_.setPx(px);
    momentum_.setPy(py);
    momentum_.setPz(pz);
    refreshEnergy();
}

/*
 * Function:  ~Particle (destructor)
 * Purpose:   Destroy particle; nested FourMomentum destructor runs afterward.
 * Input:     (none).
 * Output:    (none).
 * Method:    Trace print; member momentum_ destroyed automatically releasing
 *            its dynamic vector.
 */
Particle::~Particle()
{
    std::cout << "Calling Particle destructor\n";
}

/*
 * Function:  Particle (copy constructor)
 * Purpose:   Duplicate name and deep-copy momentum state.
 * Input:     const Particle& other — source.
 * Output:    (object constructed).
 * Method:    Member-wise copy; FourMomentum copy ctor deep-copies vector data.
 */
Particle::Particle(const Particle& other) : name(other.name), momentum_(other.momentum_)
{
    std::cout << "Calling Particle copy constructor\n";
}

/*
 * Function:  operator= (copy assignment)
 * Purpose:   Assign another particle's name and momentum into this object.
 * Input:     const Particle& other — right-hand side.
 * Output:    Particle& — *this.
 * Method:    Self-test; copy string and invoke FourMomentum copy assignment.
 */
Particle& Particle::operator=(const Particle& other)
{
    std::cout << "Calling Particle copy assignment operator\n";
    if (this != &other)
    {
        name = other.name;
        momentum_ = other.momentum_;
    }
    return *this;
}

/*
 * Function:  Particle (move constructor)
 * Purpose:   Steal resources from an expiring Particle.
 * Input:     Particle&& other — expiring source (moved-from state remains valid).
 * Output:    (object constructed).
 * Method:    move name and momentum_; FourMomentum move leaves source safe.
 */
Particle::Particle(Particle&& other)
    : name(std::move(other.name)), momentum_(std::move(other.momentum_))
{
    std::cout << "Calling Particle move constructor\n";
}

/*
 * Function:  operator= (move assignment)
 * Purpose:   Move-assign name and momentum from other; drop prior state.
 * Input:     Particle&& other — expiring source.
 * Output:    Particle& — *this.
 * Method:    Self-move is no-op. Otherwise move members; ownership transfers
 *            at FourMomentum level per its move assignment implementation.
 */
Particle& Particle::operator=(Particle&& other)
{
    if (this == &other)
    {
        return *this;
    }
    std::cout << "Calling Particle move assignment operator\n";
    name = std::move(other.name);
    momentum_ = std::move(other.momentum_);
    return *this;
}

/*
 * Function:  setName
 * Purpose:   Change species; recompute E for new mass table entry.
 * Input:     const std::string& name — must be allow-listed.
 * Output:    void.
 * Method:    validateParticleName; assign; refreshEnergy.
 */
void Particle::setName(const std::string& name)
{
    validateParticleName(name);
    this->name = name;
    refreshEnergy();
}

/*
 * Function:  setPx / setPy / setPz
 * Purpose:   Update one spatial momentum component; keep E on-shell.
 * Input:     double — new px, py, or pz (sign unrestricted).
 * Output:    void.
 * Method:    Forward to FourMomentum setter then refreshEnergy.
 */
void Particle::setPx(double px)
{
    momentum_.setPx(px);
    refreshEnergy();
}

void Particle::setPy(double py)
{
    momentum_.setPy(py);
    refreshEnergy();
}

void Particle::setPz(double pz)
{
    momentum_.setPz(pz);
    refreshEnergy();
}

/*
 * Function:  getName
 * Purpose:   Read-only access to species label.
 * Input:     (none).
 * Output:    const std::string& — reference to internal name.
 */
const std::string& Particle::getName() const
{
    return name;
}

/*
 * Function:  getE / getPx / getPy / getPz
 * Purpose:   Read components of the composed four-momentum (E, px, py, pz).
 * Input:     (none).
 * Output:    double — forwarded from momentum_; E is the time component.
 * Method:    Delegates; getters mirror FourMomentum physical components.
 */
double Particle::getE() const
{
    return momentum_.getE();
}

double Particle::getPx() const
{
    return momentum_.getPx();
}

double Particle::getPy() const
{
    return momentum_.getPy();
}

double Particle::getPz() const
{
    return momentum_.getPz();
}

/*
 * Function:  operator<<
 * Purpose:   Human-readable output: name and all four-momentum components.
 * Input:     std::ostream& os; const Particle& p.
 * Output:    std::ostream& — os.
 * Method:    Streams getters in fixed label format.
 */
std::ostream& operator<<(std::ostream& os, const Particle& p)
{
    os << p.getName() << " | E=" << p.getE() << ", px=" << p.getPx() << ", py=" << p.getPy()
       << ", pz=" << p.getPz();
    return os;
}

/*
 * Function:  operator+
 * Purpose:   Component-wise sum of two particles' four-momenta (four-vector addition).
 * Input:     const Particle& other — second operand.
 * Output:    FourMomentum — (E1+E2, px1+px2, py1+py2, pz1+pz2).
 * Method:    Reads both four-vectors via getters; constructs FourMomentum.
 *            Physical note: sums momenta like p_total^mu = p1^mu + p2^mu in
 *            flat spacetime; result may not be on-shell for a single mass.
 */
FourMomentum Particle::operator+(const Particle& other) const
{
    return FourMomentum(getE() + other.getE(), getPx() + other.getPx(),
                        getPy() + other.getPy(), getPz() + other.getPz());
}

/*
 * Function:  dotProduct
 * Purpose:   Lorentz scalar product between this particle and other.
 * Input:     const Particle& other — second four-momentum carrier.
 * Output:    double — p·q with metric (+---): E1 E2 - px1 px2 - py1 py2 - pz1 pz2.
 * Method:    Delegates to FourMomentum::dot on composed members.
 */
double Particle::dotProduct(const Particle& other) const
{
    return momentum_.dot(other.momentum_);
}
