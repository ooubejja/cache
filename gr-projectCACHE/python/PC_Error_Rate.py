#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 Othmane.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

import numpy
from gnuradio import gr
import pmt
import threading

class PC_Error_Rate(gr.basic_block):
    """
    Concatenates message data coming from inputs (Codewords and/or messages)
    """
    def __init__(self):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.basic_block.__init__(
            self,
            name='PC Error Rate',   # will show up in GRC
            in_sig=[],
            out_sig=[]
        )
        self.message_port_register_in(pmt.to_pmt("TX_MSG"))
        self.set_msg_handler(pmt.intern("TX_MSG"), self.handle_tx_msg)
        self.message_port_register_in(pmt.to_pmt("TX_CW"))
        self.set_msg_handler(pmt.intern("TX_CW"), self.handle_tx_cw)
        self.message_port_register_in(pmt.to_pmt("RX_MSG"))
        self.set_msg_handler(pmt.intern("RX_MSG"), self.handle_rx_msg)
        self.message_port_register_in(pmt.to_pmt("RX_CW"))
        self.set_msg_handler(pmt.intern("RX_CW"), self.handle_rx_cw)

        self.message_port_register_out(pmt.to_pmt("ERROR_MSG"))
        self.message_port_register_out(pmt.to_pmt("ERROR_CW"))

        self.lock = threading.Lock()

        self.TX_MSG = 0
        self.TX_CW = 0
        self.RX_MSG = 0
        self.RX_CW = 0

        self.cnt = 0

        self.all_tx_msg = {}
        self.all_tx_cw = {}
        self.all_rx_msg = {}
        self.all_rx_cw = {}


    def handle_tx_msg(self, msg_pmt):
        with self.lock :
        # if self.cnt < 1 :
            self.TX_MSG = pmt.to_python(msg_pmt)
            key, val = self.TX_MSG.items()[0]
            self.all_tx_msg[key] = val

    def handle_tx_cw(self, msg_pmt):
        with self.lock :
            self.TX_CW = pmt.to_python(msg_pmt)
            key, val = self.TX_CW.items()[0]
            self.all_tx_cw[key] = val



    def handle_rx_msg(self, msg_pmt):
        with self.lock :
            self.RX_MSG = pmt.to_python(msg_pmt)
            key, val = self.RX_MSG.items()[0]
            self.all_rx_msg[key] = val

    def handle_rx_cw(self, msg_pmt):
        with self.lock :
            self.RX_CW = pmt.to_python(msg_pmt)
            key, val = self.RX_CW.items()[0]
            self.all_rx_cw[key] = val





            
