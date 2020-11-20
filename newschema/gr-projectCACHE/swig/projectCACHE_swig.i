/* -*- c++ -*- */

#define PROJECTCACHE_API

%include "gnuradio.i"			// the common stuff
%include <std_string.i>

//load generated python docstrings
%include "projectCACHE_swig_doc.i"

%{
//#include "projectCACHE/Tx_1P_b.h"
//#include "projectCACHE/Rx_1P_b.h"
//#include "projectCACHE/gain_bf.h"
#include "projectCACHE/polarEnc_b.h"
#include "projectCACHE/PolarDec_b.h"
//#include "projectCACHE/PolarDecStrong_b.h"
#include "projectCACHE/crc32_1_bb.h"
#include "projectCACHE/ofdm_frame_equalizer1_vcvc.h"
#include "projectCACHE/map_header_payload_bc.h"
%}


//%include "projectCACHE/Tx_1P_b.h"
//GR_SWIG_BLOCK_MAGIC2(projectCACHE, Tx_1P_b);
//%include "projectCACHE/Rx_1P_b.h"
//GR_SWIG_BLOCK_MAGIC2(projectCACHE, Rx_1P_b);
//%include "projectCACHE/gain_bf.h"
//GR_SWIG_BLOCK_MAGIC2(projectCACHE, gain_bf);
%include "projectCACHE/polarEnc_b.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, polarEnc_b);
%include "projectCACHE/PolarDec_b.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, PolarDec_b);
//%include "projectCACHE/PolarDecStrong_b.h"
//GR_SWIG_BLOCK_MAGIC2(projectCACHE, PolarDecStrong_b);
%include "projectCACHE/crc32_1_bb.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, crc32_1_bb);
%include "projectCACHE/ofdm_frame_equalizer1_vcvc.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, ofdm_frame_equalizer1_vcvc);
%include "projectCACHE/map_header_payload_bc.h"
GR_SWIG_BLOCK_MAGIC2(projectCACHE, map_header_payload_bc);
