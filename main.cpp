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
 *  PURPOSE OF THIS FILE (main.cpp)
 * ===========================================================================
 *  Provides the program entry point and a small anonymous-namespace print
 *  helpers. No class definitions live here; Particle and FourMomentum are
 *  implemented in their respective translation units. This file wires the
 *  assignment test sequence and exception handling.
 *
 * ===========================================================================
 *  AI USAGE DECLARATION
 * ===========================================================================
 *  AI tools were used in a limited way for bug detection and for tightening
 *  up comment wording. I reviewed all final design decisions, implementation
 *  details, and validation rules myself before handing the work in.
 *
 * ===========================================================================
 *  AIM OF THE PROGRAM
 * ===========================================================================
 *  This program models particles and their four-momentum in a simplified
 *  relativistic framework (tabulated masses, on-shell energy). It is really
 *  about organising the problem in C++: classes, dynamic memory done properly,
 *  and a sensible use of operator overloading. It is coursework-grade code,
 *  not a full collision simulation.
 *
 * ===========================================================================
 *  CORE CLASSES
 * ===========================================================================
 *  (1) Particle
 *      - Stores the particle name.
 *      - Contains a FourMomentum object as part of its state (composition).
 *      - Validates the particle type against an allowed list of names.
 *      - Supports operator+ and dotProduct using the underlying four-momentum
 *        data (sum returns a FourMomentum; dot product is Lorentz invariant).
 *
 *  (2) FourMomentum
 *      - Represents the four-momentum (E, px, py, pz) in that order.
 *      - Keeps the components in a dynamically allocated std::vector<double>
 *        (callers cannot see the vector directly).
 *      - Provides getters and setters for each component.
 *      - Validates that energy E is never negative; px, py, pz can be either
 *        positive or negative.
 *
 * ===========================================================================
 *  PROGRAM WORKFLOW
 * ===========================================================================
 *  1. Create a set of test particles inside main().
 *  2. Particle names and four-momentum values are checked through the class
 *     interfaces so invalid input never quietly slips through.
 *  3. Demonstrate addition of four-momenta (two particles, operator+).
 *  4. Demonstrate the dot product (two muons from the test set).
 *  5. Demonstrate the copy constructor.
 *  6. Demonstrate copy assignment.
 *  7. Demonstrate the move constructor (antielectron).
 *  8. Demonstrate move assignment (antimuon).
 *  9. Print clear outputs for each test (section titles plus helper lines).
 *
 * ===========================================================================
 *  VALIDATION / FATAL-ERROR BEHAVIOUR
 * ===========================================================================
 *  Wrong particle names are fatal: the program throws and stops. The same
 *  idea applies to bad four-momentum data, especially negative energy E.
 *  If anything like that happens during setup or updates, execution does not
 *  carry on with partial results: main() reports the error and exits non-zero.
 *  The numbered tests only run to completion when everything that is
 *  validated has been accepted.
 *
 * ===========================================================================
 */

#include "Particle.h"
#include <exception>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

namespace
{
    /*
     * Function:  printParticleLine
     * Purpose:   Pretty-print one labelled row for a Particle (stdout).
     * Input:     const char* label — left column text; const Particle& p — data.
     * Output:    void — writes to std::cout.
     * Method:    Sets field width for alignment then streams p via operator<<.
     */
    void printParticleLine(const char* label, const Particle& p)
    {
        std::cout << "  " << std::left << std::setw(18) << label << " " << p << '\n';
    }

    /*
     * Function:  printFourMomentumLine
     * Purpose:   Pretty-print components of a bare FourMomentum (no name).
     * Input:     const char* label; const FourMomentum& q — (E, px, py, pz).
     * Output:    void — writes to std::cout.
     * Method:    Manual format matching particle output style for markers.
     */
    void printFourMomentumLine(const char* label, const FourMomentum& q)
    {
        std::cout << "  " << std::left << std::setw(18) << label << " E=" << q.getE() << ", px=" << q.getPx()
                   << ", py=" << q.getPy() << ", pz=" << q.getPz() << '\n';
    }
}

/*
 * ===========================================================================
 *  FUNCTION:  main
 * ===========================================================================
 *  Purpose:   Driver for Assignment 4. Builds the required particles, runs
 *              each demonstration from the brief in order, and stops as soon
 *              as something fails validation.
 *
 *  Input:     No command-line arguments. All kinematic inputs are fixed
 *              literals in this translation unit.
 *
 *  Output:    Labelled test sections on stdout; special-member traces from
 *              Particle and FourMomentum; error text on stderr if an
 *              exception is thrown; process exit code 0 on success or 1 on
 *              failure.
 *
 *  Method:    Reserve and fill a std::vector with 2 electrons, 4 muons, and
 *              2 taus, construct antielectron and antimuon for the move tests,
 *              then inside try: four-momentum sum, dot product, copy
 *              assignment, copy construction, move construction, move
 *              assignment. A catch on std::exception prints what went wrong
 *              and returns 1 so invalid input never looks like a clean run.
 *
 * ===========================================================================
 */

int main()
{
    try
    {
        /* Holds 2 electrons, then 4 muons, then 2 taus (assignment test set). */
        std::vector<Particle> particles;
        particles.reserve(8);

        particles.emplace_back("electron", 0.1, 0.0, 0.0);
        particles.emplace_back("electron", -0.05, 0.02, 0.0);

        particles.emplace_back("muon", 50.0, 0.0, 0.0);
        particles.emplace_back("muon", -50.0, 0.0, 0.0);
        particles.emplace_back("muon", 10.0, 5.0, 0.0);
        particles.emplace_back("muon", 0.0, 0.0, 30.0);

        particles.emplace_back("tau", 100.0, 0.0, 0.0);
        particles.emplace_back("tau", 0.0, 50.0, 0.0);

        /* Separate objects reserved for move semantics demonstrations. */
        Particle antielectron("antielectron", -0.4, 0.0, 0.1);
        Particle antimuon("antimuon", -40.0, 5.0, 0.0);

        /* Indices into particles: first two slots are electrons; next four muons. */
        constexpr std::size_t kElectron0 = 0;
        constexpr std::size_t kElectron1 = 1;
        constexpr std::size_t kMuon0 = 2;
        constexpr std::size_t kMuon1 = 3;

        std::cout << "\n=== 1) Sum of four-momenta of the two electrons ===\n";
        printParticleLine("electron [0]", particles[kElectron0]);
        printParticleLine("electron [1]", particles[kElectron1]);
        /* Component-wise four-vector sum; result type is FourMomentum (not Particle). */
        const FourMomentum electronSum = particles[kElectron0] + particles[kElectron1];
        printFourMomentumLine("sum", electronSum);

        std::cout << "\n=== 2) Dot product of the first two muons ===\n";
        printParticleLine("muon [0]", particles[kMuon0]);
        printParticleLine("muon [1]", particles[kMuon1]);
        std::cout << "  Lorentz dot product: " << particles[kMuon0].dotProduct(particles[kMuon1]) << '\n';

        std::cout << "\n=== 3) Copy assignment: electron to new electron ===\n";
        printParticleLine("source electron", particles[kElectron0]);
        Particle electronAssigned;
        electronAssigned = particles[kElectron0];
        printParticleLine("after copy assign", electronAssigned);

        std::cout << "\n=== 4) Copy constructor: first muon to new muon ===\n";
        printParticleLine("source muon", particles[kMuon0]);
        Particle muonCopy(particles[kMuon0]);
        printParticleLine("copy ctor muon", muonCopy);

        std::cout << "\n=== 5) Move constructor using antielectron ===\n";
        printParticleLine("source antielectron", antielectron);
        /* std::move yields xvalue; Particle move ctor steals resources. */
        Particle fromMoveCtor(std::move(antielectron));
        printParticleLine("move ctor result", fromMoveCtor);

        std::cout << "\n=== 6) Move assignment using antimuon ===\n";
        printParticleLine("source antimuon", antimuon);
        Particle moveAssignTarget;
        moveAssignTarget = std::move(antimuon);
        printParticleLine("after move assign", moveAssignTarget);

        std::cout << '\n';
    }
    catch (const std::exception& e)
    {
        /* Validation or other std::exception: report and exit with failure code. */
        std::cerr << "\n=== Error ===\n" << e.what() << '\n';
        return 1;
    }

    return 0;
}
