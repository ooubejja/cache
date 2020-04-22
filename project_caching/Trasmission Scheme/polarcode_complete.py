#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Polarcode Complete
# GNU Radio version: 3.7.14.0
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import digital
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
        self.snr = snr = 15
        self.Kw = Kw = 70*8
        self.variance = variance = 1/pow(10,snr/10.0)
        self.small_packet_len = small_packet_len = 52
        self.samp_rate = samp_rate = 1e6
        self.payload_mod = payload_mod = digital.constellation_qpsk()
        self.packetlength = packetlength = "packet_len"
        self.n_users = n_users = 5
        self.id_user = id_user = 5
        self.Nbfiles = Nbfiles = 20
        self.NbStrgUsers = NbStrgUsers = 1
        self.NbChuncks = NbChuncks = 100
        self.N = N = 2048
        self.Ks = Ks = 2*Kw

        ##################################################
        # Blocks
        ##################################################
        self.projectCACHE_polarEnc_b_0 = projectCACHE.polarEnc_b(N, Kw, Ks, Nbfiles, NbChuncks, NbStrgUsers, id_user, small_packet_len, "packet_len")
        self.projectCACHE_PolarDec_b_0 = projectCACHE.PolarDec_b(N, Kw, Ks, Nbfiles, NbChuncks, id_user, n_users, small_packet_len, packetlength)
        self.digital_chunks_to_symbols_xx_0_0 = digital.chunks_to_symbols_bc((payload_mod.points()), 1)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_repack_bits_bb_0_1 = blocks.repack_bits_bb(8, 2, packetlength, False, gr.GR_LSB_FIRST)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, numpy.sqrt(variance), numpy.random.randint(0,500,None))



        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_throttle_0_0, 0))
        self.connect((self.blocks_repack_bits_bb_0_1, 0), (self.digital_chunks_to_symbols_xx_0_0, 0))
        self.connect((self.blocks_throttle_0_0, 0), (self.projectCACHE_PolarDec_b_0, 0))
        self.connect((self.digital_chunks_to_symbols_xx_0_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.projectCACHE_polarEnc_b_0, 0), (self.blocks_repack_bits_bb_0_1, 0))

    def get_snr(self):
        return self.snr

    def set_snr(self, snr):
        self.snr = snr
        self.set_variance(1/pow(10,self.snr/10.0))

    def get_Kw(self):
        return self.Kw

    def set_Kw(self, Kw):
        self.Kw = Kw
        self.set_Ks(2*self.Kw)

    def get_variance(self):
        return self.variance

    def set_variance(self, variance):
        self.variance = variance
        self.analog_noise_source_x_0.set_amplitude(numpy.sqrt(self.variance))

    def get_small_packet_len(self):
        return self.small_packet_len

    def set_small_packet_len(self, small_packet_len):
        self.small_packet_len = small_packet_len

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)

    def get_payload_mod(self):
        return self.payload_mod

    def set_payload_mod(self, payload_mod):
        self.payload_mod = payload_mod

    def get_packetlength(self):
        return self.packetlength

    def set_packetlength(self, packetlength):
        self.packetlength = packetlength

    def get_n_users(self):
        return self.n_users

    def set_n_users(self, n_users):
        self.n_users = n_users

    def get_id_user(self):
        return self.id_user

    def set_id_user(self, id_user):
        self.id_user = id_user

    def get_Nbfiles(self):
        return self.Nbfiles

    def set_Nbfiles(self, Nbfiles):
        self.Nbfiles = Nbfiles

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


def main(top_block_cls=polarcode_complete, options=None):

    tb = top_block_cls()
    tb.start()
    tb.wait()


if __name__ == '__main__':
    main()
