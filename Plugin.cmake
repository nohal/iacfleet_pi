# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
    "nohal/opencpn-plugins"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "nohal/opencpn-plugins"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "nohal/opencpn-plugins"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

#
#
# -------  Plugin setup --------
#
set(PKG_NAME IACFleet_pi)
set(PKG_VERSION  0.30.0)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME IACFleet)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME IACFleet) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "IAC Fleetcode plugin for OpenCPN")
set(PKG_DESCRIPTION [=[
IAC Fleetcode plugin for OpenCPN
]=])

set(PKG_AUTHOR "Pavel Kalian")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/nohal/iacfleet_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/iacfleet.html)

add_definitions(-DIACFLEET_USE_SVG)
add_definitions(-DocpnUSE_GL)
include_directories(include)

set(SRC
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
)

set(PKG_API_LIB api-16)  #  A dir in opencpn-libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
endmacro()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/plugingl")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/jsoncpp")
  target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)

  # The wxsvg library enables SVG overall in the plugin
  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/wxsvg")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxsvg)
endmacro()
