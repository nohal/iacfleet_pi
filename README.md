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
