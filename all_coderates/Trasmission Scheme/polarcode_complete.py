#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Polarcode Complete
# Generated: Thu Nov 26 14:01:33 2020
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.digital.utils import tagged_streams
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import numpy
import projectCACHE


class polarcode_complete(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Polarcode Complete")

        ##################################################
        # Variables
        ##################################################
        self.snr = snr = 20
        self.variance = variance = 1/pow(10,snr/10.0)
        self.small_packet_len = small_packet_len = 52
        self.size_chunk = size_chunk = 42
        self.samp_rate = samp_rate = 32000
        self.packetlength = packetlength = "packet_len"
        self.id_user = id_user = 2
        self.code_rate = code_rate = 3
        self.Nbfiles = Nbfiles = 20
        self.NbWeakUsers = NbWeakUsers = 3
        self.NbStrgUsers = NbStrgUsers = 1
        self.NbChuncks = NbChuncks = 200
        self.N = N = 2048

        ##################################################
        # Blocks
        ##################################################
        self.projectCACHE_polarEnc_b_0 = projectCACHE.polarEnc_b(N, Nbfiles, NbChuncks, NbStrgUsers, id_user, small_packet_len, "packet_len")
        self.projectCACHE_map_header_payload_bc_0 = projectCACHE.map_header_payload_bc(2, 2, packetlength)
        self.projectCACHE_PolarDec_b_0 = projectCACHE.PolarDec_b(N, Nbfiles, NbChuncks, id_user, NbWeakUsers, small_packet_len, size_chunk, code_rate, packetlength)
        self.blocks_tag_debug_0_0 = blocks.tag_debug(gr.sizeof_char*1, '', ""); self.blocks_tag_debug_0_0.set_display(False)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, numpy.sqrt(variance), numpy.random.randint(0,500,None))

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))    
        self.connect((self.blocks_add_xx_0, 0), (self.projectCACHE_PolarDec_b_0, 0))    
        self.connect((self.projectCACHE_map_header_payload_bc_0, 0), (self.blocks_add_xx_0, 0))    
        self.connect((self.projectCACHE_polarEnc_b_0, 0), (self.blocks_tag_debug_0_0, 0))    
        self.connect((self.projectCACHE_polarEnc_b_0, 0), (self.projectCACHE_map_header_payload_bc_0, 0))    

    def get_snr(self):
        return self.snr

    def set_snr(self, snr):
        self.snr = snr
        self.set_variance(1/pow(10,self.snr/10.0))

    def get_variance(self):
        return self.variance

    def set_variance(self, variance):
        self.variance = variance
        self.analog_noise_source_x_0.set_amplitude(numpy.sqrt(self.variance))

    def get_small_packet_len(self):
        return self.small_packet_len

    def set_small_packet_len(self, small_packet_len):
        self.small_packet_len = small_packet_len

    def get_size_chunk(self):
        return self.size_chunk

    def set_size_chunk(self, size_chunk):
        self.size_chunk = size_chunk

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_packetlength(self):
        return self.packetlength

    def set_packetlength(self, packetlength):
        self.packetlength = packetlength

    def get_id_user(self):
        return self.id_user

    def set_id_user(self, id_user):
        self.id_user = id_user

    def get_code_rate(self):
        return self.code_rate

    def set_code_rate(self, code_rate):
        self.code_rate = code_rate

    def get_Nbfiles(self):
        return self.Nbfiles

    def set_Nbfiles(self, Nbfiles):
        self.Nbfiles = Nbfiles

    def get_NbWeakUsers(self):
        return self.NbWeakUsers

    def set_NbWeakUsers(self, NbWeakUsers):
        self.NbWeakUsers = NbWeakUsers

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


def main(top_block_cls=polarcode_complete, options=None):

    tb = top_block_cls()
    tb.start()
    tb.wait()


if __name__ == '__main__':
    main()
