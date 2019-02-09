[![Build Status](https://travis-ci.org/londonhackspace/VendingMachineReborn.svg?branch=v2.0)](https://travis-ci.org/londonhackspace/VendingMachineReborn)

# THE VENDING MACHINE WILL RISE AGAIN!
written for the hackspace vending machine. The code is ugly, feel free to reuse it for your vending needs.

## Building
To build this you need:

* CMake
* Linux/macOS (I'll get Windows going at some point)

### Make sure you have initialised submodules
This codebase uses submodules a lot for external dependencies. Make sure you have initialised them properly (and on every pull to be sure anything hasn't changed) with:

```
git submodule update --init --recursive
```

### Get the toolchains
Firstly, you need the toolchains and utlitities for building for the target devices. You can skip this step if you're only building the tests or virtual vending machine functionality.

```
./getToolchain.sh
```

This is the only command that will touch the source directory. Everything else happens in the build directory. The toolchains are (mostly) from the relevant Arduino-like project.

### Choose your target and initialise

Right now this code supports:

* [TI Connected Launchpad](http://www.ti.com/tool/EK-TM4C1294XL)
* [ST NUCLEO-H743ZI](https://www.st.com/en/evaluation-tools/nucleo-h743zi.html)

Choose your target from the `targets` directory, and initialise CMake. For example:
```
mkdir -p build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../targets/tivac-launchpad/EK-TM4C1294XL.cmake
```

or for the ST Nucleo:

```
mkdir -p build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../targets/stm32/STM32H743ZI.cmake
```

To build for PC (for testing), omit the toolchain file and build like a normal CMake project:

```
mkdir -p build
cd build
cmake ..
```

Note that not all functionality may be present on all targets.

### Make the code

```
make -j5
```
See? that was easy! change the number after -j to use more or fewer cores. the number of cores in your system plus one is a good default.

### Deploy to a device

There's a makefile target for deploying to a target device:

```
make flash
```

## Code Structure

The code uses Arduino cores and libraries for the main functionality. Libraries are in the `lib` directory, and exist as CMake targets for linking against.

### Directory structure

* cmake - contains CMake scripts
* lib - contains Arduino libraries as submodules
* src - contains the source code for this project
  * src/Core - contains the core vending machine logic and interface header files
  * src/HAL - contains hardware-specific implementations
  * src/Modules - contains modules, such as display implementations
* targets - contains board-specific configuration and core libraries

### Modularity Goals

The aim of this code structure is to allow experimentation and flexibility. Depending on the hardware in use, you might want to use a different display, or interface to a different coin mechanism. This is allowed for by a modular code structure. In addition, it should be possible to run as much of the code as possible on desktop systems, for easier testing and debugging. 

### Rules of thumb of where to put code

* If it's generic and reusable, put it in src/Core
* If it's something you might or might not want to use, put it in src/Modules
* If it's fairly low level, and specific to a single piece of hardware or board, put it in src/HAL

## Code Style

I don't want to be too rigid here, however some common style makes things easier.

### General

* Spaces not tabs (four of them please)
* Try to keep lines short. I'm not going to specify a hard limit though
* Prefer inline functions to macros
* Prefer static const variables to preprocessor defines
* Remember this runs on microcontrollers so don't go crazy!
* braces generally go on their own line
  * I find this makes blocks easier to read
  * Except namespace definitions - they can be inline to keep them out the way
* Make sure any virtual classes have a virtual destructor or inherit from a class that does
* Make use of the override keyword where it makes sense
* The last line of a file should be blank
  * This both feels cleaner, and avoids some weird results when including files where the last part is a comment

### Naming

* Classes are camel case, starting with a capital letter and are UpperCamelCase
  * AThingThatDoesStuff
  * SomeOtherClass
* Interfaces start with I and are otherwise names like classes
  * IFooBar
  * IAnotherThing
* Variables start with a lower case letter and are camelCase
  * aVariable
  * somethingElse

### Files

* Keep files short
  * One class per file preferably
* Name the file with the class or interface it contains
* Use `#pragma once`
  * The compilers we care about all support it, and it saves the mess of usual include guards


## Current Notes

Here are a few things to watch out for:

* Ethernet currently isn't supported on the Nucleo - this is because it's too new and the libraries don't yet support it

