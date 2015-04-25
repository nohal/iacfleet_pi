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
SET(VERSION_MINOR "2")

#SET(CMAKE_BUILD_TYPE Debug)

# Definitions
add_definitions (
-D_FILE_OFFSET_BITS=64
-D_LARGEFILE_SOURCE
-DLIBUNRAR_BUILD
-DRARDLL
-DUNRAR
-DSILENT
)

INCLUDE("cmake/PluginConfigure.cmake")

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
src/ocpn_plugin.h
        )

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_IACFLEET})


INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
