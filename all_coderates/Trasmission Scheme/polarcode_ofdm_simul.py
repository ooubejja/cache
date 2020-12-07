#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Polar Coding with Coded Caching
# Author: Yasser
# Generated: Sat Nov 21 12:42:22 2020
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio.digital.utils import tagged_streams
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import numpy
import projectCACHE
import random


class polarcode_ofdm_simul(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Polar Coding with Coded Caching")

        ##################################################
        # Variables
        ##################################################
        self.snr = snr = 40.0
        self.pilot_symbols = pilot_symbols = ((1, 1, 1, -1,),)
        self.pilot_carriers = pilot_carriers = ((-21, -7, 7, 21,),)
        self.payload_mod = payload_mod = digital.constellation_qpsk()
        self.packet_length_tag_key = packet_length_tag_key = "packet_len"
        self.occupied_carriers = occupied_carriers = (range(-26, -21) + range(-20, -7) + range(-6, 0) + range(1, 7) + range(8, 21) + range(22, 27),)
        self.length_tag_key = length_tag_key = "frame_len"
        self.header_mod = header_mod = digital.constellation_bpsk()
        self.fft_len = fft_len = 64
        self.Kw = Kw = 70*8
        self.variance = variance = 1/pow(10,snr/10.0)
        self.sync_word2 = sync_word2 = [0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 1, 1, -1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, 1, -1, -1, 1, -1, 0, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, -1, -1, -1, 1, -1, 1, -1, -1, -1, -1, 0, 0, 0, 0, 0] 
        self.sync_word1 = sync_word1 = [0., 0., 0., 0., 0., 0., 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., -1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., -1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 1.41421356, 0., 0., 0., 0., 0., 0.]
        self.small_packet_len = small_packet_len = 52
        self.samp_rate = samp_rate = 32000
        self.payload_equalizer = payload_equalizer = digital.ofdm_equalizer_simpledfe(fft_len, payload_mod.base(), occupied_carriers, pilot_carriers, pilot_symbols, 0, 1)
        self.id_user = id_user = 2
        self.header_formatter = header_formatter = digital.packet_header_ofdm(occupied_carriers, n_syms=1, len_tag_key=packet_length_tag_key, frame_len_tag_key=length_tag_key, bits_per_header_sym=header_mod.bits_per_symbol(), bits_per_payload_sym=payload_mod.bits_per_symbol(), scramble_header=False)
        self.header_equalizer = header_equalizer = digital.ofdm_equalizer_simpledfe(fft_len, header_mod.base(), occupied_carriers, pilot_carriers, pilot_symbols, 0, 1)
        self.code_rate = code_rate = 4
        self.Nbfiles = Nbfiles = 20
        self.NbUsersW = NbUsersW = 2
        self.NbStrgUsers = NbStrgUsers = 1
        self.NbChuncks = NbChuncks = 200
        self.N = N = 2048
        self.Ks = Ks = 2*Kw

        ##################################################
        # Blocks
        ##################################################
        self.projectCACHE_polarEnc_b_0_0 = projectCACHE.polarEnc_b(N, Nbfiles, NbChuncks, NbStrgUsers, id_user, small_packet_len, packet_length_tag_key)
        self.projectCACHE_ofdm_frame_equalizer1_vcvc_0 = projectCACHE.ofdm_frame_equalizer1_vcvc(fft_len, fft_len/4, length_tag_key, True, occupied_carriers, pilot_carriers, pilot_symbols, 0, True)
        self.projectCACHE_map_header_payload_bc_0 = projectCACHE.map_header_payload_bc(1, 2, packet_length_tag_key)
        self.projectCACHE_PolarDec_b_0_0 = projectCACHE.PolarDec_b(N, Nbfiles, NbChuncks, id_user, NbUsersW, small_packet_len, 42, code_rate, packet_length_tag_key)
        self.fft_vxx_1 = fft.fft_vcc(fft_len, True, (), True, 1)
        self.fft_vxx_0_0 = fft.fft_vcc(fft_len, False, (()), True, 1)
        self.fft_vxx_0 = fft.fft_vcc(fft_len, True, (()), True, 1)
        self.digital_packet_headerparser_b_0 = digital.packet_headerparser_b(header_formatter.base())
        self.digital_packet_headergenerator_bb_0 = digital.packet_headergenerator_bb(header_formatter.formatter(), packet_length_tag_key)
        self.digital_ofdm_sync_sc_cfb_0 = digital.ofdm_sync_sc_cfb(fft_len, fft_len/4, False)
        self.digital_ofdm_serializer_vcc_payload = digital.ofdm_serializer_vcc(fft_len, occupied_carriers, length_tag_key, packet_length_tag_key, 1, '', True)
        self.digital_ofdm_serializer_vcc_header = digital.ofdm_serializer_vcc(fft_len, occupied_carriers, length_tag_key, '', 0, '', True)
        self.digital_ofdm_frame_equalizer_vcvc_0 = digital.ofdm_frame_equalizer_vcvc(header_equalizer.base(), fft_len/4, length_tag_key, True, 1)
        self.digital_ofdm_cyclic_prefixer_0 = digital.ofdm_cyclic_prefixer(fft_len, fft_len+fft_len/4, 0, packet_length_tag_key)
        self.digital_ofdm_chanest_vcvc_0 = digital.ofdm_chanest_vcvc((sync_word1), (sync_word2), 1, 0, 3, False)
        self.digital_ofdm_carrier_allocator_cvc_0 = digital.ofdm_carrier_allocator_cvc(fft_len, occupied_carriers, pilot_carriers, pilot_symbols, (sync_word1,sync_word2), packet_length_tag_key)
        self.digital_header_payload_demux_0 = digital.header_payload_demux(
        	  3,
        	  fft_len,
        	  fft_len/4,
        	  length_tag_key,
        	  "",
        	  True,
        	  gr.sizeof_gr_complex,
        	  "rx_time",
                  samp_rate,
                  (),
                  0,
            )
        self.digital_constellation_decoder_cb_0 = digital.constellation_decoder_cb(header_mod.base())
        self.digital_chunks_to_symbols_xx_0_1 = digital.chunks_to_symbols_bc((header_mod.points()), 1)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_tagged_stream_mux_0 = blocks.tagged_stream_mux(gr.sizeof_gr_complex*1, packet_length_tag_key, 0)
        (self.blocks_tagged_stream_mux_0).set_max_output_buffer(8192)
        self.blocks_tag_gate_0 = blocks.tag_gate(gr.sizeof_gr_complex * 1, False)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.blocks_multiply_const_vxx_1 = blocks.multiply_const_vcc((1, ))
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, fft_len+fft_len/4)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, numpy.sqrt(variance)*0, numpy.random.randint(0,500,None))
        self.analog_frequency_modulator_fc_0 = analog.frequency_modulator_fc(-2.0/fft_len)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.digital_packet_headerparser_b_0, 'header_data'), (self.digital_header_payload_demux_0, 'header_data'))    
        self.connect((self.analog_frequency_modulator_fc_0, 0), (self.blocks_multiply_xx_0, 0))    
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))    
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_delay_0, 0))    
        self.connect((self.blocks_add_xx_0, 0), (self.digital_ofdm_sync_sc_cfb_0, 0))    
        self.connect((self.blocks_delay_0, 0), (self.blocks_multiply_xx_0, 1))    
        self.connect((self.blocks_multiply_const_vxx_1, 0), (self.blocks_throttle_0_0, 0))    
        self.connect((self.blocks_multiply_xx_0, 0), (self.digital_header_payload_demux_0, 0))    
        self.connect((self.blocks_tag_gate_0, 0), (self.blocks_multiply_const_vxx_1, 0))    
        self.connect((self.blocks_tagged_stream_mux_0, 0), (self.digital_ofdm_carrier_allocator_cvc_0, 0))    
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_add_xx_0, 0))    
        self.connect((self.digital_chunks_to_symbols_xx_0_1, 0), (self.blocks_tagged_stream_mux_0, 0))    
        self.connect((self.digital_constellation_decoder_cb_0, 0), (self.digital_packet_headerparser_b_0, 0))    
        self.connect((self.digital_header_payload_demux_0, 0), (self.fft_vxx_0, 0))    
        self.connect((self.digital_header_payload_demux_0, 1), (self.fft_vxx_1, 0))    
        self.connect((self.digital_ofdm_carrier_allocator_cvc_0, 0), (self.fft_vxx_0_0, 0))    
        self.connect((self.digital_ofdm_chanest_vcvc_0, 0), (self.digital_ofdm_frame_equalizer_vcvc_0, 0))    
        self.connect((self.digital_ofdm_cyclic_prefixer_0, 0), (self.blocks_tag_gate_0, 0))    
        self.connect((self.digital_ofdm_frame_equalizer_vcvc_0, 0), (self.digital_ofdm_serializer_vcc_header, 0))    
        self.connect((self.digital_ofdm_serializer_vcc_header, 0), (self.digital_constellation_decoder_cb_0, 0))    
        self.connect((self.digital_ofdm_serializer_vcc_payload, 0), (self.projectCACHE_PolarDec_b_0_0, 0))    
        self.connect((self.digital_ofdm_sync_sc_cfb_0, 0), (self.analog_frequency_modulator_fc_0, 0))    
        self.connect((self.digital_ofdm_sync_sc_cfb_0, 1), (self.digital_header_payload_demux_0, 1))    
        self.connect((self.digital_packet_headergenerator_bb_0, 0), (self.digital_chunks_to_symbols_xx_0_1, 0))    
        self.connect((self.fft_vxx_0, 0), (self.digital_ofdm_chanest_vcvc_0, 0))    
        self.connect((self.fft_vxx_0_0, 0), (self.digital_ofdm_cyclic_prefixer_0, 0))    
        self.connect((self.fft_vxx_1, 0), (self.projectCACHE_ofdm_frame_equalizer1_vcvc_0, 0))    
        self.connect((self.projectCACHE_map_header_payload_bc_0, 0), (self.blocks_tagged_stream_mux_0, 1))    
        self.connect((self.projectCACHE_ofdm_frame_equalizer1_vcvc_0, 0), (self.digital_ofdm_serializer_vcc_payload, 0))    
        self.connect((self.projectCACHE_polarEnc_b_0_0, 0), (self.digital_packet_headergenerator_bb_0, 0))    
        self.connect((self.projectCACHE_polarEnc_b_0_0, 0), (self.projectCACHE_map_header_payload_bc_0, 0))    

    def get_snr(self):
        return self.snr

    def set_snr(self, snr):
        self.snr = snr
        self.set_variance(1/pow(10,self.snr/10.0))

    def get_pilot_symbols(self):
        return self.pilot_symbols

    def set_pilot_symbols(self, pilot_symbols):
        self.pilot_symbols = pilot_symbols
        self.set_header_equalizer(digital.ofdm_equalizer_simpledfe(self.fft_len, header_mod.base(), self.occupied_carriers, self.pilot_carriers, self.pilot_symbols, 0, 1))
        self.set_payload_equalizer(digital.ofdm_equalizer_simpledfe(self.fft_len, payload_mod.base(), self.occupied_carriers, self.pilot_carriers, self.pilot_symbols, 0, 1))

    def get_pilot_carriers(self):
        return self.pilot_carriers

    def set_pilot_carriers(self, pilot_carriers):
        self.pilot_carriers = pilot_carriers
        self.set_header_equalizer(digital.ofdm_equalizer_simpledfe(self.fft_len, header_mod.base(), self.occupied_carriers, self.pilot_carriers, self.pilot_symbols, 0, 1))
        self.set_payload_equalizer(digital.ofdm_equalizer_simpledfe(self.fft_len, payload_mod.base(), self.occupied_carriers, self.pilot_carriers, self.pilot_symbols, 0, 1))

    def get_payload_mod(self):
        return self.payload_mod

    def set_payload_mod(self, payload_mod):
        self.payload_mod = payload_mod

    def get_packet_length_tag_key(self):
        return self.packet_length_tag_key

    def set_packet_length_tag_key(self, packet_length_tag_key):
        self.packet_length_tag_key = packet_length_tag_key
        self.set_header_formatter(digital.packet_header_ofdm(self.occupied_carriers, n_syms=1, len_tag_key=self.packet_length_tag_key, frame_len_tag_key=self.length_tag_key, bits_per_header_sym=header_mod.bits_per_symbol(), bits_per_payload_sym=payload_mod.bits_per_symbol(), scramble_header=False))

    def get_occupied_carriers(self):
        return self.occupied_carriers

    def set_occupied_carriers(self, occupied_carriers):
        self.occupied_carriers = occupied_carriers
        self.set_header_formatter(digital.packet_header_ofdm(self.occupied_carriers, n_syms=1, len_tag_key=self.packet_length_tag_key, frame_len_tag_key=self.length_tag_key, bits_per_header_sym=header_mod.bits_per_symbol(), bits_per_payload_sym=payload_mod.bits_per_symbol(), scramble_header=False))
        self.set_header_equalizer(digital.ofdm_equalizer_simpledfe(self.fft_len, header_mod.base(), self.occupied_carriers, self.pilot_carriers, self.pilot_symbols, 0, 1))
        self.set_payload_equalizer(digital.ofdm_equalizer_simpledfe(self.fft_len, payload_mod.base(), self.occupied_carriers, self.pilot_carriers, self.pilot_symbols, 0, 1))

    def get_length_tag_key(self):
        return self.length_tag_key

    def set_length_tag_key(self, length_tag_key):
        self.length_tag_key = length_tag_key
        self.set_header_formatter(digital.packet_header_ofdm(self.occupied_carriers, n_syms=1, len_tag_key=self.packet_length_tag_key, frame_len_tag_key=self.length_tag_key, bits_per_header_sym=header_mod.bits_per_symbol(), bits_per_payload_sym=payload_mod.bits_per_symbol(), scramble_header=False))

    def get_header_mod(self):
        return self.header_mod

    def set_header_mod(self, header_mod):
        self.header_mod = header_mod

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len
        self.set_header_equalizer(digital.ofdm_equalizer_simpledfe(self.fft_len, header_mod.base(), self.occupied_carriers, self.pilot_carriers, self.pilot_symbols, 0, 1))
        self.set_payload_equalizer(digital.ofdm_equalizer_simpledfe(self.fft_len, payload_mod.base(), self.occupied_carriers, self.pilot_carriers, self.pilot_symbols, 0, 1))
        self.blocks_delay_0.set_dly(self.fft_len+self.fft_len/4)
        self.analog_frequency_modulator_fc_0.set_sensitivity(-2.0/self.fft_len)

    def get_Kw(self):
        return self.Kw

    def set_Kw(self, Kw):
        self.Kw = Kw
        self.set_Ks(2*self.Kw)

    def get_variance(self):
        return self.variance

    def set_variance(self, variance):
        self.variance = variance
        self.analog_noise_source_x_0.set_amplitude(numpy.sqrt(self.variance)*0)

    def get_sync_word2(self):
        return self.sync_word2

    def set_sync_word2(self, sync_word2):
        self.sync_word2 = sync_word2

    def get_sync_word1(self):
        return self.sync_word1

    def set_sync_word1(self, sync_word1):
        self.sync_word1 = sync_word1

    def get_small_packet_len(self):
        return self.small_packet_len

    def set_small_packet_len(self, small_packet_len):
        self.small_packet_len = small_packet_len

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)

    def get_payload_equalizer(self):
        return self.payload_equalizer

    def set_payload_equalizer(self, payload_equalizer):
        self.payload_equalizer = payload_equalizer

    def get_id_user(self):
        return self.id_user

    def set_id_user(self, id_user):
        self.id_user = id_user

    def get_header_formatter(self):
        return self.header_formatter

    def set_header_formatter(self, header_formatter):
        self.header_formatter = header_formatter

    def get_header_equalizer(self):
        return self.header_equalizer

    def set_header_equalizer(self, header_equalizer):
        self.header_equalizer = header_equalizer

    def get_code_rate(self):
        return self.code_rate

    def set_code_rate(self, code_rate):
        self.code_rate = code_rate

    def get_Nbfiles(self):
        return self.Nbfiles

    def set_Nbfiles(self, Nbfiles):
        self.Nbfiles = Nbfiles

    def get_NbUsersW(self):
        return self.NbUsersW

    def set_NbUsersW(self, NbUsersW):
        self.NbUsersW = NbUsersW

    def get_NbStrgUsers(self):
        return self.NbStrgUsers

    def set_NbStrgUsers(self, NbStrgUsers):
        self.NbStrgUsers = NbStrgUsers

    def get_NbChuncks(self):
        return self.NbChuncks

    def set_NbChuncks(self, NbChuncks):
        self.NbChuncks = NbChuncks

    def get_N(self):
        return self.N

    def set_N(self, N):
        self.N = N

    def get_Ks(self):
        return self.Ks

    def set_Ks(self, Ks):
        self.Ks = Ks


def main(top_block_cls=polarcode_ofdm_simul, options=None):

    tb = top_block_cls()
    tb.start()
    tb.wait()


if __name__ == '__main__':
    main()
