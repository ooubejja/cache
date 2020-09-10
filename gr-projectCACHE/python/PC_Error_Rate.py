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
import sys
import time

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

        # self.message_port_register_out(pmt.to_pmt("ERROR_MSG"))
        # self.message_port_register_out(pmt.to_pmt("ERROR_CW"))

        self.lock = threading.Lock()

        self.TX_MSG = 0
        self.TX_CW = 0
        self.RX_MSG = 0
        self.RX_CW = 0

        self.MSG_BER = 0
        self.CW_BER = 0
        self.sum_errors_msg = 0
        self.sum_errors_cw = 0

        self.cnt_msg = 0
        self.cnt_cw = 0
        self.total_chunks = 0
        self.size_cw = 1

        self.all_tx_msg = {}
        self.all_tx_cw = {}
        self.all_rx_msg = {}
        self.all_rx_cw = {}

        # self.filename = "../trasmissioni/ERROR_RATE_"+time.strftime("%d%m%Y-%H%M%S")+".txt"
        self.filename = "../trasmissioni/ERROR_RATE.txt"
        with open(self.filename,"w") as f:
            template =                                          \
            "======================"                            \
            +"\n"+"Number of coded chunks/bits sent: "                            \
            +"\n\n"+"======================"        \
            +"\n"+"Number of coded chunks/bits decoded: "                            \
            +"\n\n"+"======================"        \
            +"\n"+"MSG Error sum: "                            \
            +"\n\n"+"======================"        \
            +"\n"+"MSG Bit Error Rate: "                            \
            +"\n\n"+"======================"        \
            +"\n"+"CW Error sum: "                            \
            +"\n\n"+"======================"        \
            +"\n"+"CW Bit Error Rate: "                            \
            +"\n\n"+"======================"        \

            f.write(template)


    def handle_tx_msg(self, msg_pmt):
        with self.lock :
            self.TX_MSG = pmt.to_python(msg_pmt)
            # print "OLE MSG : " + str(type(self.TX_MSG))
            # print "OLE MSG  : " + str(self.TX_MSG[1])

            # self.TX_MSG =

            # exit(1)
            # try :
            key, val = self.TX_MSG[1][0],self.TX_MSG[1][1:]

            val = map(chr,val)
            val = ''.join(val)
            # print "A MSG KEY : " + str(key)
            # print "A MSG VAL : " + str(len(val))
            self.all_tx_msg[key] = val  # Append new chunk
            # except :
            if len(self.TX_MSG[1]) == 1 :  # TX_MSG PDUs were all transmitted
                # try :
                # print "A FINISH : "
                # Compute final metrics (total sent chunks, sizes, etc)
                self.total_chunks = len(self.all_tx_msg)
                self.size_msg = len(self.all_tx_msg.items()[0][1])
                self.size_cw = len(self.all_tx_cw.items()[0][1])

                with open(self.filename,"r") as f:
                    lines = f.readlines()
                    for i in range(len(lines)):
                        if 'bits sent:' in lines[i]:
                            lines[i+1] = str(self.total_chunks) + " | " + str(self.total_chunks*self.size_cw) +'\n'
                with open(self.filename,"w") as f:
                    f.write(''.join(lines))

                # except :
                #     pass
                    # print "Error rate : Couldn't determine CW or MSG size, check inputs"

    def handle_tx_cw(self, msg_pmt):
        with self.lock :
            self.TX_CW = pmt.to_python(msg_pmt)
            # print "OLE CW  : " + str(self.TX_CW[1])
            # print "OLE OLE : " + str(self.TX_CW)
            # print "OLE OLE OLE" + str(self.TX_MSG.items()[0])
            # exit(1)
            key, val = self.TX_CW[1][0],self.TX_CW[1][1:]
            val = map(chr,val)
            val = ''.join(val)
            # print "CW KEY : " + str(key)
            # print "CW VAL : " + str(val)

            self.all_tx_cw[key] = val





    # def handle_tx_msg(self, msg_pmt):
    #     with self.lock :
    #         self.TX_MSG = pmt.to_python(msg_pmt)
    #         print "OLE OL : " + str(type(self.TX_MSG))
    #         print "OLE OLE : " + str(self.TX_MSG)
    #         print "OLE OLE OLE" + str(self.TX_MSG.items()[0])
    #
    #         # self.TX_MSG =
    #
    #         exit(1)
    #         try :
    #             key, val = self.TX_MSG.items()[0]
    #             self.all_tx_msg[key] = val  # Append new chunk
    #         except :
    #             if self.TX_MSG[0][0] == -1 :  # TX_MSG PDUs were all transmitted
    #                 try :
    #                     # Compute final metrics (total sent chunks, sizes, etc)
    #                     self.total_chunks = len(self.all_tx_msg)
    #                     self.size_msg = len(self.all_tx_msg.items()[0][1])
    #                     self.size_cw = len(self.all_tx_cw.items()[0][1])
    #
    #                     with open(self.filename,"r") as f:
    #                         lines = f.readlines()
    #                         for i in range(len(lines)):
    #                             if 'bits sent:' in lines[i]:
    #                                 lines[i+1] = str(self.total_chunks) + " | " + str(self.total_chunks*self.size_cw) +'\n'
    #                     with open(self.filename,"w") as f:
    #                         f.write(''.join(lines))
    #
    #                 except :
    #                     pass
    #                     # print "Error rate : Couldn't determine CW or MSG size, check inputs"
    #
    # def handle_tx_cw(self, msg_pmt):
    #     with self.lock :
    #         self.TX_CW = pmt.to_python(msg_pmt)
    #         print "OLE OL : " + str(type(self.TX_CW))
    #         print "OLE OLE : " + str(self.TX_CW)
    #         print "OLE OLE OLE" + str(self.TX_MSG.items()[0])
    #         exit(1)
    #         key, val = self.TX_CW.items()[0]
    #         self.all_tx_cw[key] = val



    def handle_rx_msg(self, msg_pmt):
        with self.lock :
            self.RX_MSG = pmt.to_python(msg_pmt)
            # key, val = self.RX_MSG.items()[0]
            key, val = self.RX_MSG[1][0],self.RX_MSG[1][1:]
            val = map(chr,val)
            val = ''.join(val)
            self.all_rx_msg[key] = val

            # print "A RX KEY : " + str(key)
            # print "A RX VAL : " + str(len(val))
            # try :
            self.cnt_msg += 1
            # b = self.all_tx_msg[key]    # Extract respective Tx chunk
            b = self.all_tx_cw[key]    # Extract respective Tx chunk
            a = self.xor_two_str(val,b)     # Compare with Rx chunk
            ##########################################
            # Most important metric
            self.sum_errors_msg += sum(map(int,a))
            # print "A MSG SUM OF ERRORS : " + str(self.sum_errors_msg)
            ##########################################

            # self.MSG_BER = self.sum_errors_msg/float((self.cnt_msg*self.size_msg))  # Message Bit Error Rate, updated every iteration
            self.MSG_BER = self.sum_errors_msg/float((self.cnt_msg*self.size_cw))  # Message Bit Error Rate, updated every iteration
            # print "MSG BER (decoded only): " + str(self.MSG_BER)

            with open(self.filename,"r") as f:
                lines = f.readlines()
                for i in range(len(lines)):
                    if 'MSG Bit' in lines[i]:
                        # lines[i+1] = lines[i+1][:-1] + '['+str(key)+'] ' + str(self.MSG_BER)+ " " +'\n'
                        lines[i+1] = lines[i+1][:-1] + '['+ "%02d"%key +'] ' + str(self.MSG_BER)+ " " +'\n'
                    if 'MSG Error' in lines[i]:
                        lines[i+1] = lines[i+1][:-1] + '['+ "%02d"%key +'] ' + str(self.sum_errors_msg)+ " " +'\n'
            with open(self.filename,"w") as f:
                f.write(''.join(lines))
            # except :
            #         print "A MSG Invalid Rx chunk ID. Considered erroneous."
                    # self.sum_errors_msg += self.size_msg  # all bits are considered erroneous


    def handle_rx_cw(self, msg_pmt):
        with self.lock :
            self.RX_CW = pmt.to_python(msg_pmt)
            # key, val = self.RX_CW.items()[0]
            key, val = self.RX_CW[1][0],self.RX_CW[1][1:]
            val = map(chr,val)
            val = ''.join(val)
            self.all_rx_cw[key] = val
            # try :
            self.cnt_cw += 1
            b = self.all_tx_cw[key]    # Extract respective Tx chunk
            a = self.xor_two_str(val,b)     # Compare with Rx chunk
            ##########################################
            # Most important metric
            self.sum_errors_cw += sum(map(int,a))
            # print "A CW SUM OF ERRORS : " + str(self.sum_errors_cw)
            ##########################################
            self.CW_BER = self.sum_errors_cw/float(self.cnt_cw*self.size_cw)  # Message Bit Error Rate, updated every iteration
            # print "CW BER (decoded only): " + str(self.CW_BER)

            with open(self.filename,"r") as f:
                lines = f.readlines()
                for i in range(len(lines)):
                    if 'bits decoded:' in lines[i]:
                        lines[i+1] = str(self.cnt_cw) + " | " + str(self.cnt_cw*self.size_cw) +'\n'
                    if 'CW Bit' in lines[i]:
                        lines[i+1] = lines[i+1][:-1] + '['+ "%02d"%key +'] ' + str(self.CW_BER)+ " " +'\n'
                    if 'CW Error' in lines[i]:
                        lines[i+1] = lines[i+1][:-1] + '['+ "%02d"%key +'] ' + str(self.sum_errors_cw)+ " " +'\n'
            with open(self.filename,"w") as f:
                f.write(''.join(lines))

            # except :
            #     print "A CW Invalid Rx chunk ID. Considered erroneous."
                # self.sum_errors_cw += self.size_cw  # all bits are considered erroneous



    def xor_two_str(self,a,b):
        xored = []
        for i in range(max(len(a), len(b))):
            xored_value = ord(a[i%len(a)]) ^ ord(b[i%len(b)])
            tmp = hex(xored_value)[2:]
            if tmp not in ['0','1'] :   # Bug : In polardec, recCodeword is sometimes different than 0 or 1
                 tmp = '1'
            xored.append(tmp)
        return xored
