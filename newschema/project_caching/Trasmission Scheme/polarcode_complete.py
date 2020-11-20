#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Polarcode Complete
# GNU Radio version: 3.7.14.0
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

    def __init__(self, id_user=0):
        gr.top_block.__init__(self, "Polarcode Complete")

        ##################################################
        # Parameters
        ##################################################
        self.id_user = id_user

        ##################################################
        # Variables
        ##################################################
        self.snr = snr = 25
        self.variance = variance = 1/pow(10,snr/10.0)
        self.small_packet_len = small_packet_len = 52
        self.samp_rate = samp_rate = 1e6
        self.packetlength = packetlength = "packet_len"
        self.n_users = n_users = 3
        self.coderate = coderate = [3,3,3,4]
        self.Nbfiles = Nbfiles = 20
        self.NbStrgUsers = NbStrgUsers = 1
        self.NbChuncks = NbChuncks = 200
        self.N = N = 2048

        ##################################################
        # Blocks
        ##################################################
        self.projectCACHE_polarEnc_b_0 = projectCACHE.polarEnc_b(N, Nbfiles, NbChuncks, NbStrgUsers, id_user, small_packet_len, "packet_len")
        self.projectCACHE_map_header_payload_bc_0 = projectCACHE.map_header_payload_bc(4, 2, packetlength)
        self.projectCACHE_PolarDec_b_0 = projectCACHE.PolarDec_b(N, Nbfiles, NbChuncks, id_user, n_users, small_packet_len, 42, coderate[id_user], packetlength)
        self.projectCACHE_PC_Error_Rate_0 = projectCACHE.PC_Error_Rate(id_user, NbChuncks)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_tag_debug_0_0 = blocks.tag_debug(gr.sizeof_char*1, '', ""); self.blocks_tag_debug_0_0.set_display(False)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, numpy.sqrt(variance), numpy.random.randint(0,500,None))



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.projectCACHE_PolarDec_b_0, 'CH_USE'), (self.projectCACHE_PC_Error_Rate_0, 'CH_USE'))
        self.msg_connect((self.projectCACHE_polarEnc_b_0, 'BER_INFO'), (self.projectCACHE_PC_Error_Rate_0, 'BER_INFO'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.projectCACHE_PolarDec_b_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.projectCACHE_map_header_payload_bc_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.projectCACHE_polarEnc_b_0, 0), (self.blocks_tag_debug_0_0, 0))
        self.connect((self.projectCACHE_polarEnc_b_0, 0), (self.projectCACHE_map_header_payload_bc_0, 0))

    def get_id_user(self):
        return self.id_user

    def set_id_user(self, id_user):
        self.id_user = id_user

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

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_packetlength(self):
        return self.packetlength

    def set_packetlength(self, packetlength):
        self.packetlength = packetlength

    def get_n_users(self):
        return self.n_users

    def set_n_users(self, n_users):
        self.n_users = n_users

    def get_coderate(self):
        return self.coderate

    def set_coderate(self, coderate):
        self.coderate = coderate

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


def argument_parser():
    parser = OptionParser(usage="%prog: [options]", option_class=eng_option)
    parser.add_option(
        "-U", "--id-user", dest="id_user", type="intx", default=0,
        help="Set User [default=%default]")
    return parser


def main(top_block_cls=polarcode_complete, options=None):
    if options is None:
        options, _ = argument_parser().parse_args()

    tb = top_block_cls(id_user=options.id_user)
    tb.start()
    tb.wait()


if __name__ == '__main__':
    main()
