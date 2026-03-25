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
 *  Declares the Particle class: named species with validated type string,
 *  composed FourMomentum, and interfaces for energy consistency, printing,
 *  four-momentum addition, and Lorentz dot product. Implementation is in
 *  Particle.cpp.
 *
 * ===========================================================================
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include "FourMomentum.h"
#include <iosfwd>
#include <string>

/*
 * ===========================================================================
 *  CLASS: Particle
 * ===========================================================================
 *  Represents:  A named elementary particle in a simplified on-shell model:
 *               each name maps to a fixed rest mass; energy E is derived from
 *               m and (px, py, pz), not supplied independently by the user
 *               in the main constructor path.
 *
 *  Stores:      Species label (name) and a FourMomentum (momentum_) holding
 *               (E, px, py, pz). Momentum components may be any sign; E is
 *               kept consistent with mass and three-momentum for this object.
 *
 *  Role:       Bridges physics labels (electron, muon, ...) to numerical
 *               four-vectors; validates names; exposes operator+ (component sum
 *               of two four-vectors) and dotProduct (invariant scalar).
 *
 *  Relation:   Composes FourMomentum. Does not derive from it; ownership is
 *              clear (value semantics with deep management inside FourMomentum).
 *
 * ===========================================================================
 */
class Particle
{
private:
    /* Species string from the coursework allow-list; empty for default ctor. */
    std::string name;

    /* Four-momentum (E, px, py, pz); energy validated via FourMomentum rules. */
    FourMomentum momentum_;

    /*
     * Function:  computeEnergy
     * Purpose:   On-shell energy from invariant mass m and three-momentum.
     * Input:     double restMass — mass in MeV (c = 1); px, py, pz — components.
     * Output:    double — E = sqrt(m^2 + px^2 + py^2 + pz^2), non-negative.
     * Method:    Pure arithmetic; no validation here (mass expected sensible).
     */
    double computeEnergy(double restMass, double px, double py, double pz) const;

    /*
     * Function:  restMass
     * Purpose:   Map particle name to tabulated rest mass for this model.
     * Input:     (none) — uses member name.
     * Output:    double — mass in MeV, or 0 if name empty/unknown fallback.
     * Method:    Branch on name; matches assignment mass table.
     */
    double restMass() const;

    /*
     * Function:  refreshEnergy
     * Purpose:   Recompute E from current name (mass) and three-momentum.
     * Input:     (none).
     * Output:    void — writes through momentum_.setE (which validates E>=0).
     * Method:    Delegates to computeEnergy; keeps four-vector on-shell.
     */
    void refreshEnergy();

public:
    /* Default ctor: empty name, zero momentum; for later assignment. */
    Particle();

    /*
     * Parameterised ctor: validates name, sets (px,py,pz), computes E on-shell.
     * Input: name (allow-list), px, py, pz (any sign). Output: constructed object.
     */
    Particle(const std::string& name, double px, double py, double pz);

    /* Destructor: tears down object; nested FourMomentum cleans its heap. */
    ~Particle();

    /* Copy ctor: deep copy of momentum; duplicate name string. */
    Particle(const Particle& other);

    /* Copy assign: self-test; assign name and copy-assign momentum. */
    Particle& operator=(const Particle& other);

    /* Move ctor: steal name and momentum from expiring other. */
    Particle(Particle&& other);

    /* Move assign: self-move no-op; else move name and momentum. */
    Particle& operator=(Particle&& other);

    /* Accessor: species label. Return: const std::string& to internal name. */
    const std::string& getName() const;

    /*
     * Four-momentum getters: read E, px, py, pz from composed FourMomentum.
     * Return: double each; units MeV with c=1 in this coursework model.
     */
    double getE() const;
    double getPx() const;
    double getPy() const;
    double getPz() const;

    /*
     * setName: validates allow-list, updates mass table key, recomputes E.
     * setPx/setPy/setPz: update one spatial component; refresh on-shell E.
     */
    void setName(const std::string& name);
    void setPx(double px);
    void setPy(double py);
    void setPz(double pz);

    /*
     * operator+: component-wise sum of two four-momenta p^mu + q^mu.
     * Input: const Particle& other. Return: FourMomentum with summed components.
     */
    FourMomentum operator+(const Particle& other) const;

    /*
     * dotProduct: Lorentz invariant p·q with metric (+ - - -).
     * Input: const Particle& other. Return: double scalar.
     */
    double dotProduct(const Particle& other) const;
};

/*
 * Function:  operator<<
 * Purpose:   Stream insertion for readable particle state (marking / debugging).
 * Input:     std::ostream& os — target stream; const Particle& p — object.
 * Output:    std::ostream& — reference to os for chaining.
 * Method:    Prints name and four-momentum components via particle getters.
 */
std::ostream& operator<<(std::ostream& os, const Particle& p);

#endif
