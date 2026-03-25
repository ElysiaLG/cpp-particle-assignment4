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
 *  Defines all member functions of class FourMomentum: construction, copying,
 *  moving, destruction, accessors, mutators, energy validation, and Lorentz
 *  dot product. Trace prints support coursework marking for Rule of Five.
 *
 * ===========================================================================
 */

#include "FourMomentum.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
    /* Shared zero four-vector (E, px, py, pz) for default and moved-from state. */
    const std::vector<double> kZeroFourVector{0.0, 0.0, 0.0, 0.0};

    /*
     * Function:  validateEnergy
     * Purpose:   Enforce coursework rule that energy E must not be negative.
     * Input:     double E — candidate energy component.
     * Output:    void (throws on failure).
     * Method:    If E < 0, throw std::invalid_argument with message and value.
     *            px, py, pz are not validated here (any sign permitted).
     */
    void validateEnergy(double E)
    {
        if (E < 0.0)
        {
            throw std::invalid_argument("Four-momentum energy E must be non-negative, invalid E="
                                        + std::to_string(E));
        }
    }
}

/*
 * Function:  FourMomentum (default constructor)
 * Purpose:   Create a null four-momentum (all components zero).
 * Input:     (none).
 * Output:    (object constructed).
 * Method:    Allocates vector copy of kZeroFourVector; p_ owns the allocation.
 */
FourMomentum::FourMomentum() : p_(new std::vector<double>(kZeroFourVector))
{
    std::cout << "Calling FourMomentum constructor\n";
}

/*
 * Function:  FourMomentum (parameterized constructor)
 * Purpose:   Initialise (E, px, py, pz) from explicit components.
 * Input:     double E, px, py, pz — four-momentum components (c = 1 units).
 * Output:    (object constructed).
 * Method:    validateEnergy(E); allocate vector {E, px, py, pz}. Physical
 *            note: E is not recomputed from |p| here; caller must supply a
 *            consistent quadruple if modelling an on-shell particle.
 */
FourMomentum::FourMomentum(double E, double px, double py, double pz) : p_(nullptr)
{
    validateEnergy(E);
    p_ = new std::vector<double>{E, px, py, pz};
    std::cout << "Calling FourMomentum constructor\n";
}

/*
 * Function:  ~FourMomentum (destructor)
 * Purpose:   Release heap storage owned by p_.
 * Input:     (none).
 * Output:    (none).
 * Method:    delete p_; safe if nullptr per C++ rules.
 */
FourMomentum::~FourMomentum()
{
    std::cout << "Calling FourMomentum destructor\n";
    delete p_;
}

/*
 * Function:  FourMomentum (copy constructor)
 * Purpose:   Deep copy of another four-momentum's vector contents.
 * Input:     const FourMomentum& other — source object.
 * Output:    (object constructed).
 * Method:    New vector allocated; copy elements. If other.p_ were null,
 *            falls back to kZeroFourVector (defensive).
 */
FourMomentum::FourMomentum(const FourMomentum& other)
    : p_(new std::vector<double>(other.p_ ? *other.p_ : kZeroFourVector))
{
    std::cout << "Calling FourMomentum copy constructor\n";
}

/*
 * Function:  operator= (copy assignment)
 * Purpose:   Assign another four-momentum's values into this object.
 * Input:     const FourMomentum& other — right-hand side.
 * Output:    FourMomentum& — *this.
 * Method:    Self-assignment guard. Reuse existing vector if possible, else
 *            allocate, so no double-delete; deep-copies numeric components.
 */
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
                p_ = new std::vector<double>(kZeroFourVector);
            }
            else
            {
                *p_ = kZeroFourVector;
            }
        }
    }
    return *this;
}

/*
 * Function:  FourMomentum (move constructor)
 * Purpose:   Transfer ownership of heap vector from other to this object.
 * Input:     FourMomentum&& other — expiring source.
 * Output:    (object constructed).
 * Method:    Steal other.p_ after other receives a new zero vector so the
 *            source stays valid (no dangling getter use). Pointer stolen is
 *            assigned to p_. Local stolen holds the address during handoff.
 */
FourMomentum::FourMomentum(FourMomentum&& other) : p_(nullptr)
{
    std::vector<double>* stolen = other.p_;
    other.p_ = new std::vector<double>(kZeroFourVector);
    p_ = stolen;
    std::cout << "Calling FourMomentum move constructor\n";
}

/*
 * Function:  operator= (move assignment)
 * Purpose:   Move-assign resources from other, releasing previous storage.
 * Input:     FourMomentum&& other — expiring source.
 * Output:    FourMomentum& — *this.
 * Method:    Self-move returns immediately. Otherwise give other a fresh zero
 *            vector first, delete this p_, steal stolen pointer. Avoids leak
 *            and leaves other in a safe state.
 */
FourMomentum& FourMomentum::operator=(FourMomentum&& other)
{
    if (this == &other)
    {
        return *this;
    }
    std::cout << "Calling FourMomentum move assignment operator\n";
    std::vector<double>* stolen = other.p_;
    other.p_ = new std::vector<double>(kZeroFourVector);
    delete p_;
    p_ = stolen;
    return *this;
}

/*
 * Function:  getE
 * Purpose:   Read energy component E of the four-momentum.
 * Input:     (none).
 * Output:    double — E at index 0.
 * Method:    Indexed access through p_; no validation on read.
 */
double FourMomentum::getE() const
{
    return (*p_)[0];
}

/*
 * Function:  getPx, getPy, getPz
 * Purpose:   Read spatial momentum components (three-momentum).
 * Input:     (none).
 * Output:    double — px, py, or pz respectively; any sign allowed by model.
 * Method:    Indexed access (1..3). These are Cartesian components of p_vec.
 */
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

/*
 * Function:  setE
 * Purpose:   Write energy E after validation.
 * Input:     double E — new energy; must be >= 0.
 * Output:    void.
 * Method:    validateEnergy(E); assign (*p_)[0].
 */
void FourMomentum::setE(double E)
{
    validateEnergy(E);
    (*p_)[0] = E;
}

/*
 * Function:  setPx, setPy, setPz
 * Purpose:   Write spatial components of three-momentum.
 * Input:     double px, py, or pz — may be positive or negative.
 * Output:    void.
 * Method:    Direct write to indices 1..3; no sign checks in this layer.
 *            Callers (e.g. Particle::refreshEnergy) may adjust E afterward.
 */
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

/*
 * Function:  dot
 * Purpose:   Lorentz-invariant scalar product p*q (four-vector inner product).
 * Input:     const FourMomentum& other — second four-momentum.
 * Output:    double — p·q = E1*E2 - px1*px2 - py1*py2 - pz1*pz2.
 * Method:    Implements metric signature (+ - - -). For a single on-shell
 *            particle, p·p = E^2 - |p_vec|^2 = m^2 (invariant mass squared).
 */
double FourMomentum::dot(const FourMomentum& other) const
{
    return getE() * other.getE() - getPx() * other.getPx() - getPy() * other.getPy()
         - getPz() * other.getPz();
}
