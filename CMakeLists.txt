##---------------------------------------------------------------------------
## Author:      Pavel Kalian
## Copyright:   2014
## License:     GPLv2 License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(iacfleet_pi)

SET(PACKAGE_NAME iacfleet_pi)
SET(VERBOSE_NAME IACFleet)
SET(TITLE_NAME IACFleet)
SET(CPACK_PACKAGE_CONTACT "Pavel Kalian")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "21")

SET(NAME_SUFFIX "ov50")

#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_MACOSX_RPATH "ON")

set (CMAKE_CXX_STANDARD 11)

# Definitions
add_definitions (
-D_FILE_OFFSET_BITS=64
-D_LARGEFILE_SOURCE
-DLIBUNRAR_BUILD
-DRARDLL
-DUNRAR
-DSILENT
)

OPTION(IACFLEET_USE_SVG "Use SVG graphics" ON)

INCLUDE("cmake/PluginConfigure.cmake")

IF(IACFLEET_USE_SVG)
  ADD_DEFINITIONS(-DIACFLEET_USE_SVG)
ENDIF(IACFLEET_USE_SVG)

SET(SRC_IACFLEET
src/folder.xpm
src/iacfile.cpp
src/iacfile.h
src/iacfleet.cpp
src/iacfleet.h
src/iacfleet_pi.cpp
src/iacfleet_pi.h
src/TexFont.h
src/TexFont.cpp
src/icon.png
src/icons.cpp
src/icons.h
include/ocpn_plugin.h
        )

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_IACFLEET})


INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
