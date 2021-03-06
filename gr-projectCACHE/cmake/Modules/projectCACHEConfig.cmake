INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_PROJECTCACHE projectCACHE)

FIND_PATH(
    PROJECTCACHE_INCLUDE_DIRS
    NAMES projectCACHE/api.h
    HINTS $ENV{PROJECTCACHE_DIR}/include
        ${PC_PROJECTCACHE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    PROJECTCACHE_LIBRARIES
    NAMES gnuradio-projectCACHE
    HINTS $ENV{PROJECTCACHE_DIR}/lib
        ${PC_PROJECTCACHE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PROJECTCACHE DEFAULT_MSG PROJECTCACHE_LIBRARIES PROJECTCACHE_INCLUDE_DIRS)
MARK_AS_ADVANCED(PROJECTCACHE_LIBRARIES PROJECTCACHE_INCLUDE_DIRS)

