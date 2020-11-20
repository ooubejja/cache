INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_CACHING QUIET gnuradio-core)
FIND_PATH(CACHING_INCLUDE_DIRS 
	NAMES caching/CheckFunction.h
		  caching/CodingDecodingData.h
		  caching/ConflictGraph.h
		  caching/Conversions.h
		  caching/DataDefinition.h
		  caching/DecodingInput.h
		  caching/EnvironmentSetup.h
		  caching/FuncsFromMain.h
		  caching/grasp.h
		  caching/heapSort.h
		  caching/hgcc.h
		  caching/psdes.h
		  caching/randomHandler.h
		  caching/randomUniform.h
		  caching/TxRx.h
		  caching/utilityForTesting.h

	HINTS $ENV{CACHING_DIR}/include
	      ${PC_CACHING_INCLUDE_DIR}
	PATHS /usr/local/include
	      /usr/include
          /usr/pkgs64/include
	)
FIND_LIBRARY(CACHING_LIBRARIES
	NAMES caching
	HINTS $ENV{CACHING_DIR}/lib
	      ${PC_CACHING_LIBDIR}
	PATHS /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          /usr/pkgs64/lib
	)

set(CACHING_FOUND ${PC_CACHING_FOUND})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CACHING DEFAULT_MSG  CACHING_LIBRARIES CACHING_INCLUDE_DIRS)
MARK_AS_ADVANCED(CACHING_LIBRARIES CACHING_INCLUDE_DIRS)