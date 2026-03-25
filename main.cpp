#include "Particle.h"
#include <iostream>
#include <utility>

int main()
{
    // Collection: 2 electrons, 4 muons, 2 taus
    Particle electron1("electron", 0.511, 0.5, 0.0, 0.0);
    Particle electron2("electron", 1.0, -0.3, 0.2, 0.1);
    Particle muon1("muon", 105.7, 50.0, 0.0, 0.0);
    Particle muon2("muon", 105.7, -50.0, 0.0, 0.0);
    Particle muon3("muon", 200.0, 10.0, 20.0, 30.0);
    Particle muon4("muon", 150.0, -10.0, -20.0, -30.0);
    Particle tau1("tau", 1777.0, 100.0, 0.0, 0.0);
    Particle tau2("tau", 1777.0, -100.0, 0.0, 0.0);

    // Used only for move constructor / move assignment demos
    Particle antielectron("antielectron", 0.511, -0.4, 0.0, 0.1);
    Particle antimuon("antimuon", 105.7, -40.0, 5.0, 0.0);

    std::cout << "========== Collection (2 electrons, 4 muons, 2 taus) ==========\n";
    std::cout << " electron1: " << electron1 << '\n';
    std::cout << " electron2: " << electron2 << '\n';
    std::cout << "     muon1: " << muon1 << '\n';
    std::cout << "     muon2: " << muon2 << '\n';
    std::cout << "     muon3: " << muon3 << '\n';
    std::cout << "     muon4: " << muon4 << '\n';
    std::cout << "      tau1: " << tau1 << '\n';
    std::cout << "      tau2: " << tau2 << '\n';
    std::cout << "antielectron (move ctor): " << antielectron << '\n';
    std::cout << "   antimuon (move assign): " << antimuon << '\n';

    std::cout << "\n========== Test 1: Sum four-momenta of two electrons ==========\n";
    FourMomentum sumElectrons = electron1 + electron2;
    std::cout << "electron1 + electron2 -> " << sumElectrons << '\n';
    std::cout << "Invariant mass sqrt(E^2 - p^2) of that sum = " << sumElectrons.invariantMass()
              << " (same units as E)\n";
    std::cout << "Invariant mass of electron1 alone = " << electron1.invariantMass() << "\n";

    std::cout << "\n========== Test 2: Dot product of first two muons ==========\n";
    std::cout << "muon1.dotProduct(muon2) = " << muon1.dotProduct(muon2) << '\n';

    std::cout << "\n========== Test 3: Copy assignment (electron -> new electron) ==========\n";
    Particle newElectron;
    newElectron = electron1;
    std::cout << "  Source: " << electron1 << '\n';
    std::cout << "  Copy:   " << newElectron << '\n';

    std::cout << "\n========== Test 4: Copy constructor (first muon -> new muon) ==========\n";
    Particle copiedMuon(muon1);
    std::cout << "  Source: " << muon1 << '\n';
    std::cout << "  Copy:   " << copiedMuon << '\n';

    std::cout << "\n========== Test 5: Move constructor (antielectron) ==========\n";
    Particle fromAntiElectron(std::move(antielectron));
    std::cout << "  Result: " << fromAntiElectron << '\n';

    std::cout << "\n========== Test 6: Move assignment (antimuon) ==========\n";
    Particle fromAntiMuon;
    fromAntiMuon = std::move(antimuon);
    std::cout << "  Result: " << fromAntiMuon << '\n';

    std::cout << "\n========== End of main ==========\n";
    return 0;
}
