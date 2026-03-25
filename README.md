# Particle and Four-Momentum System (Assignment 4)

This project implements a **Particle** and **FourMomentum** subsystem using C++ object-oriented design: separate classes, encapsulation, dynamic storage for the four-vector, validation, operator overloading, and explicit copy/move semantics. The scope is coursework: a small relativistic on-shell model, not a collision simulation.

---

## Compilation

```bash
g++ -Wall -Wextra -std=c++17 main.cpp Particle.cpp FourMomentum.cpp -o assignment4
```

## Running

- **Linux / macOS:** `./assignment4`
- **Windows:** `.\assignment4.exe` (or `assignment4.exe` from a shell)

---

## Class design

**Particle**

- Stores the particle **name** (`std::string`).
- **Contains a `FourMomentum` object** (composition by value).
- **Validates** the name against an allowed list whenever the name is set or the object is constructed from a name.
- Supports **`operator+`**, which returns a **`FourMomentum`**: component-wise sum of this particle’s and another’s four-momenta; and **`dotProduct(const Particle&)`**, which uses the Lorentz scalar product of the two stored four-momenta.
- For a named species, **energy `E`** is derived from fixed rest mass and **`(px, py, pz)`** so the particle stays on-shell (`E² = m² + p²`, with `c = 1`).

**FourMomentum**

- Represents **`(E, px, py, pz)`** in that order using a **dynamically allocated `std::vector<double>`** on the heap (accessed only through the class interface).
- Provides **getters and setters** for each component.
- **Validates** that **energy `E` is non-negative** in the constructor that takes `E` and in **`setE`**.

---

## Physics (short)

The four-momentum is **`(E, px, py, pz)`**. **Addition** in this project is **component-wise** on those four numbers (via `Particle::operator+`).

The **dot product** uses the relativistic convention with metric signature **`(+ − − −)`**:

\[
p \cdot q = E_1 E_2 - p_{x1} p_{x2} - p_{y1} p_{y2} - p_{z1} p_{z2}.
\]

---

## Validation

- **Particle names** must be one of: **electron**, **muon**, **tau**, **antielectron**, **antimuon**, **antitau**. Invalid names throw **`std::invalid_argument`** (message includes the name).
- **Energy `E`** must be **non-negative** wherever `FourMomentum` enforces it (`E` given to the four-argument constructor, or **`setE`**).
- **`px`, `py`, `pz`** may be **positive or negative** (no sign restriction).

On failure, an exception is thrown; **`main`** catches **`std::exception`**, prints **`e.what()`** to **`stderr`**, and exits with a non-zero status.

---

## Rule of Five

Both **`Particle`** and **`FourMomentum`** define the usual five special members: **destructor**, **copy constructor**, **copy assignment**, **move constructor**, and **move assignment**. **`FourMomentum`** uses **deep copy** of the heap-allocated vector when copying; **`Particle`** copies its **`FourMomentum`** accordingly. Several members print **`Calling ...`** lines to **stdout** for marking.

---

## Testing in `main()`

The driver builds a **`std::vector`** of **2 electrons**, **4 muons**, and **2 taus**, plus separate **`antielectron`** and **`antimuon`** for move demos. It then demonstrates:

- Sum of **four-momenta** (two electrons),
- **Dot product** (first two muons),
- **Copy constructor** (first muon),
- **Copy assignment** (electron),
- **Move constructor** (**antielectron**),
- **Move assignment** (**antimuon**),

with **clear section headings** in the program output.

---

## Code design

The project is split into **`.h` / `.cpp` pairs** (**`Particle`**, **`FourMomentum`**) plus **`main.cpp`**. The layout is **modular and readable**, without extra features beyond the assignment brief.

---

## AI assistance

AI tools were used to assist with debugging and improving code comments. All core design decisions, implementation logic, and validation strategies were developed and verified independently.
