# sense-hat-cli

Basic cli for the Sense-HAT.

## Features
* temperature readout
* pressure readout
* humidity readout
* magnetic field readout
* linear acceleration readout
* angular rate readout
* led display manipulation

## Usage

```sh
# read data from sensors and display them as tsv to stdout
sense-hat-cli

# put RGB data to connected display
sense-hat-cli --leds "255 255 255 0 0 0 128 128 128..."

# display help message
sense-hat-cli --help
```

## Installation

The project uses thirdparty submodules which all need to be git init and updated.

Then the modules have to be manually patched to work correctly on modern C++ systems. A semi-automated process is already in the works and will be included in the repo in the future. For now, these lines have to be inserted into each of the main thirdparty library files:

```c++
extern "C" {
   #include <linux/i2c.h>
   #include <linux/i2c-dev.h>
   #include <i2c/smbus.h>
}
```

Afterwards, the project should be compilable as any other cmake project.
