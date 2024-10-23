IAC Fleetcode plugin Plugin for OpenCPN
=======================================

[![Hosted By: Cloudsmith](https://img.shields.io/badge/OSS%20hosting%20by-cloudsmith-blue?logo=cloudsmith&style=flat-square)](https://cloudsmith.com)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/4964/badge.svg)](https://scan.coverity.com/projects/nohal-iacfleet_pi)

Compiling
=========
You have to be able to compile OpenCPN itself - Get the info at http://opencpn.org/ocpn/developers_manual

* This plugin now builds out of the OpenCPN source tree
```
git clone --recurse-submodules https://github.com/nohal/iacfleet_pi.git
```

### Build:
```
mkdir iacfleet_pi/build
cd iacfleet_pi/build
cmake ..
cmake --build .
```

## Contributing

### General

The project is developed in C++, language features up to C++17 can be used.
Please format your code using `clang-format` before submitting pull requests, they are checked for compliance with the `.clang-format` file used by the project during the CI workflow and can't be accepted if the test is not passing.
To make this as simple as possible, a [pre-commit](https://pre-commit.com) configuration is provided in the project, which activates a Git pre-commit hook taking care of the formatting automatically. To use it run the following commands:

```bash
pip3 install pre-commit
cd <plugin source directory>
pre-commit install
```

#### pre-commit on Windows

Use [WSL](https://learn.microsoft.com/en-us/windows/wsl/) for pre-commit and save yourself the headache.


License
=======
The plugin code is licensed under the terms of the GPL v2 or, at your will, later.

## TODO
* ~~Finish the OpenGL rendering implementation~~
* Support all the available datasources:
  * ~~fleet.nadi to query@saildocs.com~~
  * ~~http://www.mar.mil.br/dhn/chm/meteo/prev/iac/iac.htm~~
  * ~~http://weather.noaa.gov/pub/data/raw/as/asxx21.egrr..txt~~
* ~~Automatic download from NOAA and BR~~
* ~~Refine the GUI to save space~~
* Extend beyond IACFleet - at least to data from http://weather.gmdss.org/index.html
