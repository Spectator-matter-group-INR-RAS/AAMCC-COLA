# AAMCC-COLA

This repository is an example of using COLA library to assemble a Monte-Carlo pipeline from existing modules. Modules that constitute the AAMCC-MST model are used as an example.

## Prerequisites

- C++17 compiler support (gcc 8+)
- CMake 3.24+
- Root 6.32+ 
    - Versions 6.12+ should generally be fine, as they support C++17, but they were not tested.
- Geant4 v11.4+ **NOTE that SMM (which is required for Deexcitation module) is broken in 11.4.0 and 11.4.1, and patch with fix is yet to be released**. See [#100](https://github.com/Geant4/geant4/pull/100) for info for the fix or use fixMF branch of [this fork of 11.4.1](https://github.com/apBUSampK/geant4/tree/fixMF).
- We encourage you to install [COLA library]() and needed modules ([CGlauber](https://github.com/Spectator-matter-group-INR-RAS/CGlauber), [CPreEquilibrium](https://github.com/Spectator-matter-group-INR-RAS/CClustering), [DeexcitationHandler](https://github.com/Spectator-matter-group-INR-RAS/DeexcitationHandler) and [CRoot](https://github.com/Spectator-matter-group-INR-RAS/CRoot)) to be able to use other modules later, **however this project can use their built-in versions**
## Build

Clone the repository and run the following commands in project directory:
```bash
cmake -B /path/to/build/
cmake --build /path/to/build/
```

If you don't have any of the modules or even thee COLA library, add `-DBUILTIN_MODULES=ON` flag to configure step:

```bash
cmake -DBUILTIN_MODULES=ON -B /path/to/build/
cmake --build /path/to/build/
```

This way any missing modules will be downloaded and compiled in the build directory. *This will not install these modules or COLA library in your system*.

## Run

AAMCC-COLA is run with the following signature:

```bash
./AAMCC-COLA /path/to/config/file/ number_of_events
```

An example configuration file for LHC Run 2 PbPb collisions is present at `data/config_example.xml`. Geometry, energy (A GeV for stationary target and sqrt(s_nn) in GeV for collider) and projectiles are configured in CGlauber generator tag, while the output file and format are configured in CRootWriter tag. For more information on options, please consult respective module repositories.
