#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Polar Coding with Coded Caching
# Author: Yasser
# GNU Radio version: 3.7.14.0
##################################################

from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import zeromq
from gnuradio.digital.utils import tagged_streams
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import numpy
import projectCACHE
import random


class OFDM_SINK(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Polar Coding with Coded Caching")

        ##################################################
        # Blocks
        ##################################################
        self.zeromq_sub_msg_source_0_2 = zeromq.sub_msg_source('tcp://mnode13:5556', 100)
        self.zeromq_sub_msg_source_0_1 = zeromq.sub_msg_source('tcp://mnode14:5557', 100)
        self.zeromq_sub_msg_source_0_0 = zeromq.sub_msg_source('tcp://mnode14:5558', 100)
        self.zeromq_sub_msg_source_0 = zeromq.sub_msg_source('tcp://mnode13:5555', 100)
        self.projectCACHE_PC_Error_Rate_0 = projectCACHE.PC_Error_Rate()



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.zeromq_sub_msg_source_0, 'out'), (self.projectCACHE_PC_Error_Rate_0, 'TX_MSG'))
        self.msg_connect((self.zeromq_sub_msg_source_0_0, 'out'), (self.projectCACHE_PC_Error_Rate_0, 'RX_CW'))
        self.msg_connect((self.zeromq_sub_msg_source_0_1, 'out'), (self.projectCACHE_PC_Error_Rate_0, 'RX_MSG'))
        self.msg_connect((self.zeromq_sub_msg_source_0_2, 'out'), (self.projectCACHE_PC_Error_Rate_0, 'TX_CW'))


def main(top_block_cls=OFDM_SINK, options=None):

    tb = top_block_cls()
    tb.start()
    tb.wait()


if __name__ == '__main__':
    main()
