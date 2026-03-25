# Assignment 4: Particle and four-momentum

## 1. How to compile

From the project directory, compile all three translation units (C++17):

```bash
g++ -Wall -Wextra -std=c++17 main.cpp Particle.cpp FourMomentum.cpp -o assignment4
```

On Windows (PowerShell or CMD), the same command produces `assignment4.exe`.

## 2. How to run

**Linux / macOS**

```bash
./assignment4
```

**Windows**

```bash
.\assignment4.exe
```

During execution, copy/move/destructor calls print trace lines (e.g. `Calling Particle copy constructor`, `Calling FourMomentum destructor`) so markers can follow object lifetimes.

## 3. How `main()` uses `Particle` and `FourMomentum`

`main()` builds a small **test collection**: two electrons, four muons, two taus, plus an **antielectron** and **antimuon** used only for move demos. Each particle is a `Particle` holding a dynamically allocated `FourMomentum`.

The six labelled tests are:

1. **Sum of four-momenta** — `FourMomentum sumElectrons = electron1 + electron2;` shows that adding particles yields a **four-vector** (`FourMomentum`), not another named particle. Invariant masses of the sum and of `electron1` are printed.
2. **Dot product** — `muon1.dotProduct(muon2)` uses the Minkowski inner product implemented on `FourMomentum` and exposed through `Particle`.
3. **Copy assignment** — default-constructed `newElectron`, then `newElectron = electron1`.
4. **Copy constructor** — `Particle copiedMuon(muon1)`.
5. **Move constructor** — `Particle fromAntiElectron(std::move(antielectron))`.
6. **Move assignment** — default `Particle`, then `fromAntiMuon = std::move(antimuon)`.

Particles are also printed with `operator<<` for readable output (`name | E, px, py, pz`).

## 4. Physics: four-momentum and dot product

**Four-momentum** (with \(c = 1\)) packages energy and spatial momentum into one Lorentz four-vector:

\[
p^\mu = (E,\, p_x,\, p_y,\, p_z).
\]

The **invariant mass** of a single four-momentum uses \(m^2 = E^2 - p_x^2 - p_y^2 - p_z^2\); the code takes the square root after clamping slightly negative \(m^2\) to zero so rounding error does not break `sqrt`.

The **Minkowski dot product** between two four-momenta \(a\) and \(b\) uses metric signature **\((+,-,-,-)\)**:

\[
a \cdot b = E_a E_b - p_{x,a}p_{x,b} - p_{y,a}p_{y,b} - p_{z,a}p_{z,b}.
\]

This scalar is Lorentz-invariant and is what `dot()` / `dotProduct()` compute.

(In code terms: `E1*E2 - px1*px2 - py1*py2 - pz1*pz2`.)

## 5. Code design (short)

- **`FourMomentum`** owns a pointer to a heap-allocated `std::vector<double>` storing \((E, p_x, p_y, p_z)\) in that order. It provides getters/setters (energy cannot be negative), `operator+` for component-wise sum, `dot`, `invariantMass`, and `operator<<`. The raw vector is not exposed publicly.
- **`Particle`** stores a **name** (`std::string`) and a **`FourMomentum*`** to the particle’s momentum. It forwards momentum operations to that object. Adding two `Particle`s returns a `FourMomentum` because the result is total momentum, not a specific species.
- **Names** are restricted to electron/muon/tau and their antiparticles; invalid names trigger a **warning** and fall back to **`electron`**. Invalid **energy** still aborts with an error (via `FourMomentum`).

## 6. Rule of Five

Both **`FourMomentum`** and **`Particle`** manage dynamic resources (pointer to `vector` and pointer to `FourMomentum`, respectively). Each class implements the **Rule of Five**: destructor, copy constructor, copy assignment operator, move constructor, and move assignment operator. Copy operations deep-copy; move operations transfer ownership and null out the source pointer where appropriate.

## File list

| File | Role |
|------|------|
| `FourMomentum.h`, `FourMomentum.cpp` | Four-momentum type and physics helpers |
| `Particle.h`, `Particle.cpp` | Particle name + pointer to `FourMomentum` |
| `main.cpp` | Demonstration aligned with marking tests |
| `README.md` | This document |
