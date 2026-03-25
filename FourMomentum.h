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
 *  Declares the FourMomentum class: interface for a relativistic four-vector
 *  (energy and three-momentum) stored in dynamically allocated memory.
 *  Implementation is in FourMomentum.cpp. Particle composes FourMomentum.
 *
 * ===========================================================================
 */

#ifndef FOUR_MOMENTUM_H
#define FOUR_MOMENTUM_H

#include <vector>

/*
 * ===========================================================================
 *  CLASS: FourMomentum
 * ===========================================================================
 *  Represents:  A contravariant four-momentum p^mu = (E, px, py, pz) in units
 *               where c = 1, as used in the coursework model.
 *
 *  Stores:      Four doubles (E, px, py, pz) in heap-allocated std::vector,
 *               addressed via pointer member p_. Indices are fixed: 0 = E,
 *               1 = px, 2 = py, 3 = pz.
 *
 *  Role:        Holds the kinematic four-vector for a particle. Particle
 *               owns a FourMomentum instance; arithmetic helpers (dot) and
 *               valid setters keep energy non-negative where enforced.
 *
 *  Relation:    Particle "has-a" FourMomentum. operator+ on Particle returns
 *               a new FourMomentum composed from two particles' components.
 *
 * ===========================================================================
 */
class FourMomentum
{
private:
    /*
     * Pointer to a dynamically allocated std::vector of four elements.
     * Required by the assignment design; lifetime is managed by this class's
     * Rule of Five. Must not be exposed to callers (encapsulation).
     * Invariant after construction: typically points to a valid vector;
     * move operations leave the source with a fresh zero vector.
     */
    std::vector<double>* p_;

public:
    /* Default ctor: zero four-vector; allocates backing storage. */
    FourMomentum();

    /* Parameterised ctor: sets (E, px, py, pz); validates E >= 0. */
    FourMomentum(double E, double px, double py, double pz);

    /* Destructor: deletes heap vector owned by p_. */
    ~FourMomentum();

    /* Copy ctor: allocates new vector; copies contents (deep copy). */
    FourMomentum(const FourMomentum& other);

    /* Copy assign: self-guard; deep-copy or allocate as needed. */
    FourMomentum& operator=(const FourMomentum& other);

    /* Move ctor: steal vector pointer; leave other with zero vector. */
    FourMomentum(FourMomentum&& other);

    /* Move assign: self-move safe; replace storage with stolen buffer. */
    FourMomentum& operator=(FourMomentum&& other);

    /* Getters: components of four-momentum (E, px, py, pz). Return: double. */
    double getE() const;
    double getPx() const;
    double getPy() const;
    double getPz() const;

    /* setE validates non-negativity; spatial sets write px/py/pz freely. */
    void setE(double E);
    void setPx(double px);
    void setPy(double py);
    void setPz(double pz);

    /*
     * dot: Minkowski inner product with (+---); return p·q as double.
     * Input: const FourMomentum& other.
     */
    double dot(const FourMomentum& other) const;
};

#endif
