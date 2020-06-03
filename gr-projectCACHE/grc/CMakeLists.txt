# Copyright 2011 Free Software Foundation, Inc.
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

install(FILES
#    projectCACHE_Tx_1P_b.xml
#    projectCACHE_Rx_1P_b.xml
    projectCACHE_gain_bf.xml
    projectCACHE_polarEnc_b.xml
    projectCACHE_PolarDec_b.xml
    #projectCACHE_PolarDecStrong_b.xml
    projectCACHE_crc32_1_bb.xml
    projectCACHE_ofdm_frame_equalizer1_vcvc.xml
    projectCACHE_PC_Error_Rate.xml
    projectCACHE_map_header_payload_bc.xml DESTINATION share/gnuradio/grc/blocks
)
