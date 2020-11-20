/* -*- c++ -*- */

#define PROJECTCACHE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "projectCACHE_swig_doc.i"

%{
#include "projectCACHE/Tx_1P_b.h"
#include "projectCACHE/Rx_1P_b.h"
#include "projectCACHE/gain_bf.h"
#include "projectCACHE/polarEnc_b.h"
#include "projectCACHE/PolarDec_b.h"
%}


%include "projectCACHE/Tx_1P_b.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, Tx_1P_b);
%include "projectCACHE/Rx_1P_b.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, Rx_1P_b);
%include "projectCACHE/gain_bf.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, gain_bf);
%include "projectCACHE/polarEnc_b.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, polarEnc_b);
%include "projectCACHE/PolarDec_b.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, PolarDec_b);
