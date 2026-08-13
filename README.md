# Guide to Create a C App for the Numworks Calculator
<p align="center">
    <img alt="Version" src="https://img.shields.io/badge/Version-0.0.1-blue?style=for-the-badge&color=blue">
    <img alt="Stars" src="https://img.shields.io/github/stars/SaltyMold/C-App-Guide-for-Numworks?style=for-the-badge&color=magenta">
    <img alt="Forks" src="https://img.shields.io/github/forks/SaltyMold/C-App-Guide-for-Numworks?color=cyan&style=for-the-badge&color=purple">
    <img alt="License" src="https://img.shields.io/github/license/SaltyMold/C-App-Guide-for-Numworks?style=for-the-badge&color=blue">
    <br>
    <a href="https://github.com/SaltyMold"><img title="Developer" src="https://img.shields.io/badge/Developer-SaltyMold-red?style=flat-square"></a>
    <img alt="Maintained" src="https://img.shields.io/badge/Maintained-Yes-blue?style=flat-square">
    <img alt="Written In" src="https://img.shields.io/badge/Written%20In-C-yellow?style=flat-square">
</p>

This repository is a short guide to help you create a C application for the Numworks calculator.

## How to build

### Requirements

- `make`
- `gcc`
- `g++`
- `arm-none-eabi-gcc`
- `nodejs`
- `npm`
- `nwlink`

### Build

#### Clean the output

```sh
make clean                    # rm output/
make clean PLATFORM=device    # rm output/device/
make clean PLATFORM=simulator # rm output/simulator/
```

#### Build the app

```sh
make build PLATFORM=device    # build the app for the calculator
make build PLATFORM=simulator # build the app for the simulator
```

#### Run the app

```sh
make run PLATFORM=device    # run the app on the calculator
make run PLATFORM=simulator # run the app on the simulator
```

Linux is recommended for simplicity. If you use another operating system, edit the `Makefile` as needed and build the simulator from the Epsilon repository for your platform.

Prebuilt Linux simulator binaries are included in `sim/` and should work on most modern x86_64 Linux distributions.

## How to create an app

### Workspace

The workspace should look like this:

```tree
.
├── assets
│   ├── icon.png
│   └── input.bin
├── Makefile
├── sim
│   ├── epsilon.bin
│   └── libs
│       └── *.a
└── src
    ├── libs
    │   ├── eadk.h
    │   ├── macro.h
    │   ├── storage.c
    │   └── storage.h
    ├── main.c
    └── sim
        └── sim_eadk.c
```

### Usage

- `assets/icon.png` is a PNG image (55×56 px) used as the app icon.
- `assets/input.bin` is an external data file used as the app's external data.
- `Makefile` automates build and run tasks.
- `sim/epsilon.bin` is the emulator binary.
- `sim/libs/*.a` are the static libraries required by the simulator.
- `src/libs/eadk.h` declares functions to interact with the calculator display, keyboard, and system.
- `src/libs/storage.h` provides functions to use the shared storage on the calculator.
- `src/libs/macro.h` supplies compatibility macros (for example a `snprintf` overlay) so the app builds on both x86_64 and ARMv7.
- `src/sim/sim_eadk.c` implements shim functions that make the app behave under the simulator.
- `src/main.c` is the application source file.

For more details, see https://nwagyu.org.

## Acknowledgements

- [**Yaya-Cout**](https://github.com/Yaya-Cout)
- [**epsilon-sample-app-c**](https://github.com/numworks/epsilon-sample-app-c)

