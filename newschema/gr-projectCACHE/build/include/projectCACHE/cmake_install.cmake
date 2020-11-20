# Install script for directory: /home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/include/projectCACHE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/projectCACHE" TYPE FILE FILES
    "/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/include/projectCACHE/api.h"
    "/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/include/projectCACHE/polarEnc_b.h"
    "/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/include/projectCACHE/PolarDec_b.h"
    "/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/include/projectCACHE/crc32_1_bb.h"
    "/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/include/projectCACHE/ofdm_frame_equalizer1_vcvc.h"
    "/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/include/projectCACHE/map_header_payload_bc.h"
    )
endif()

