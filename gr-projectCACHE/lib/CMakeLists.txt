# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND projectCACHE_sources
    gain_bf_impl.cc
    polarEnc_b_impl.cc
    PolarDec_b_impl.cc
    ofdm_frame_equalizer1_vcvc_impl.cc
    crc32_1_bb_impl.cc
    #PolarDecStrong_b_impl.cc
    map_header_payload_bc_impl.cc
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(projectCACHE_sources "${projectCACHE_sources}" PARENT_SCOPE)
if(NOT projectCACHE_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT projectCACHE_sources)

add_library(gnuradio-projectCACHE SHARED ${projectCACHE_sources})
target_link_libraries(gnuradio-projectCACHE ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${CACHING_LIBRARIES} ${GSL_LIBRARIES})
set_target_properties(gnuradio-projectCACHE PROPERTIES DEFINE_SYMBOL "gnuradio_projectCACHE_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-projectCACHE PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-projectCACHE RUNTIME_COMPONENT "projectCACHE_runtime" DEVEL_COMPONENT "projectCACHE_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_projectCACHE_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_projectCACHE.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_projectCACHE.cc
)

add_executable(test-projectCACHE ${test_projectCACHE_sources})

target_link_libraries(
  test-projectCACHE
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  ${CACHING_LIBRARIES}
  ${GSL_LIBRARIES}
  gnuradio-projectCACHE
)

GR_ADD_TEST(test_projectCACHE test-projectCACHE)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

