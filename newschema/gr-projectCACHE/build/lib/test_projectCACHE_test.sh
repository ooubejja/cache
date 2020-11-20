#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/lib
export PATH=/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/build/lib:$PATH
export LD_LIBRARY_PATH=/home/othmane/Documents/imt/source_code/newschema/gr-projectCACHE/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-projectCACHE 
