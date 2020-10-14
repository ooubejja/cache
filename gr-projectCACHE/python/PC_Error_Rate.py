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
    def __init__(self, usr_id, is_strong = True):  # only default arguments here
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

        self.message_port_register_in(pmt.to_pmt("SNR"))
        self.set_msg_handler(pmt.intern("SNR"), self.handle_snr)
        self.message_port_register_in(pmt.to_pmt("CH_USE"))
        self.set_msg_handler(pmt.intern("CH_USE"), self.handle_ch_use)

        # self.message_port_register_out(pmt.to_pmt("ERROR_MSG"))
        # self.message_port_register_out(pmt.to_pmt("ERROR_CW"))

        self.lock = threading.Lock()

        self.usr_id = str(usr_id)

        if is_strong :
            self.usr_type = "Strong"
        else :
            self.usr_type = "Weak"

        self.SNR = []
        self.cnt_snr = 0

        self.cnt_ch_use = 0

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
        self.ready_msg = False
        self.ready_cw = False

        # self.filename = "../trasmissioni/ERROR_RATE_"+time.strftime("%d%m%Y-%H%M%S")+".txt"
        self.filename = "../trasmissioni/ERROR_RATE_" + self.usr_id + ".txt"
        with open(self.filename,"w") as f:
            template =                                          \
            "============================================"                            \
            +"\n"+"USER ID: " + self.usr_id + " | " + self.usr_type + " | "                        \
            +"\n"+"============================================"        \
            +"\n"+"Number of uncoded chunks/bits sent: "                            \
            +"\n\n"+"============================================"        \
            +"\n"+"Number of uncoded chunks/bits decoded: "                            \
            +"\n\n"+"============================================"        \
            +"\n"+"MSG Error sum: "                            \
            +"\n\n"+"============================================"        \
            +"\n"+"MSG Bit Error Rate: "                            \
            +"\n\n"+"============================================"        \
            +"\n"+"CW Error sum: "                            \
            +"\n\n"+"============================================"        \
            +"\n"+"CW Bit Error Rate: "                            \
            +"\n\n\n\n"+"============================================"        \
            +"\n"+"Average SNR: "                            \
            +"\n\n\n\n"+"============================================"        \
            +"\n"+"Channel Use | Correct Bits  | Throughput (Bits/Channel use): "                            \
            +"\n\n"+"============================================"        \


            f.write(template)


    def handle_tx_msg(self, msg_pmt):
        with self.lock :
            self.TX_MSG = pmt.to_python(msg_pmt)

            key, val = self.TX_MSG[1][0],self.TX_MSG[1][1:]
            val = map(chr,val)
            val = ''.join(val)

            # print "TX CW END :"
            # print self.TX_CW
            # Compute final metrics (total sent chunks, sizes, etc)
            if len(self.TX_MSG[1]) == 1 :  # TX_MSG PDUs were all transmitted

                self.total_chunks = len(self.all_tx_msg)
                self.size_msg = len(self.all_tx_msg.items()[0][1])
                self.ready_msg = True
                # self.total_chunks = len(self.all_tx_cw)

                with open(self.filename,"r") as f:
                    lines = f.readlines()
                    for i in range(len(lines)):
                        if 'bits sent:' in lines[i]:
                            lines[i+1] = str(self.total_chunks) + " | " + str(self.total_chunks*self.size_msg) +'\n'
                            with open(self.filename,"w") as f:
                                f.write(''.join(lines))
            else :
                self.all_tx_msg[key] = val  # Append new chunk
                # print "OYOYOYO"
                # print val

    def handle_tx_cw(self, msg_pmt):
        with self.lock :
            self.TX_CW = pmt.to_python(msg_pmt)
            key, val = self.TX_CW[1][0],self.TX_CW[1][1:]
            val = map(chr,val)
            val = ''.join(val)

            if len(self.TX_CW[1]) == 1 :  # TX_CW PDUs were all transmitted
                # self.total_chunks = len(self.all_tx_cw)
                self.size_cw = len(self.all_tx_cw.items()[0][1])
                self.ready_cw = True

                # if self.total_chunks == 0 :
                #     with open(self.filename,"r") as f:
                #         lines = f.readlines()
                #         for i in range(len(lines)):
                #             if 'bits sent:' in lines[i]:
                #                 lines[i+1] = str(self.total_chunks) + " | " + str(self.total_chunks*self.size_msg) +'\n'
                #                 with open(self.filename,"w") as f:
                #                     f.write(''.join(lines))
            else :
                self.all_tx_cw[key] = val   # Append new chunk

    def handle_rx_msg(self, msg_pmt):
        with self.lock :
            if self.ready_msg:
                self.RX_MSG = pmt.to_python(msg_pmt)
                # key, val = self.RX_MSG.items()[0]
                key, val = self.RX_MSG[1][0],self.RX_MSG[1][1:]
                val = map(chr,val)
                val = ''.join(val)
                self.all_rx_msg[key] = val

                # print "AYAYAYAYA"
                # print len(self.RX_MSG[1])

                self.cnt_msg += 1
                # try :
                b = self.all_tx_msg[key]    # Extract respective Tx chunk
                # print "MSG COMPARISON"
                # print val
                # print b
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

                print str(self.cnt_msg) + ": CHUNK " + str(key) + " DECODED."



    def handle_rx_cw(self, msg_pmt):
        with self.lock :
            if self.ready_cw:
                self.RX_CW = pmt.to_python(msg_pmt)
                key, val = self.RX_CW[1][0],self.RX_CW[1][1:]
                val = map(chr,val)
                val = ''.join(val)
                self.all_rx_cw[key] = val
                # try :
                self.cnt_cw += 1
                b = self.all_tx_cw[key]    # Extract respective Tx chunk
                # print "CW COMPARISON"
                # print val
                # print b
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
                            try :
                                lines[i+1] = str(self.cnt_cw) + " | " + str(self.cnt_cw*self.size_msg) + " | Packet loss rate: " + str(1 - self.cnt_cw/float(self.total_chunks)) + '\n'
                            except :
                                pass
                        if 'CW Bit' in lines[i]:
                            lines[i+1] = lines[i+1][:-1] + '['+ "%02d"%key +'] ' + str(self.CW_BER)+ " " +'\n'
                        if 'CW Error' in lines[i]:
                            lines[i+1] = lines[i+1][:-1] + '['+ "%02d"%key +'] ' + str(self.sum_errors_cw)+ " " +'\n'
                with open(self.filename,"w") as f:
                    f.write(''.join(lines))


                try :
                    total_bits_rx = self.cnt_cw*self.size_msg
                    total_errors = self.sum_errors_msg
                    successful_bits = total_bits_rx - total_errors

                    with open(self.filename,"r") as f:
                        lines = f.readlines()
                        for i in range(len(lines)):
                            if 'Channel' in lines[i]:
                                lines[i+1] = str(self.cnt_ch_use) + "\t\t| " + str(successful_bits) + "\t\t\t| " + str(successful_bits/float(self.cnt_ch_use)) + '\n'
                    with open(self.filename,"w") as f:
                        f.write(''.join(lines))

                except :
                    pass

    def handle_snr(self, msg_pmt):
        with self.lock :
            self.cnt_snr += 1
            tmp = pmt.to_python(msg_pmt)
            self.SNR += [float(tmp)]

            with open(self.filename,"r") as f:
                lines = f.readlines()
                for i in range(len(lines)):
                    if 'SNR' in lines[i]:
                        lines[i+1] = str(sum(self.SNR)/float(self.cnt_snr)) + '\n'
            with open(self.filename,"w") as f:
                f.write(''.join(lines))


    def handle_ch_use(self, msg_pmt):
        with self.lock :
            self.cnt_ch_use += 1



    def xor_two_str(self,a,b):
        xored = []
        for i in range(max(len(a), len(b))):
            xored_value = ord(a[i%len(a)]) ^ ord(b[i%len(b)])
            tmp = hex(xored_value)[2:]
            if tmp not in ['0','1'] :   # Bug : In polardec, recCodeword is sometimes different than 0 or 1
                 tmp = '1'
            xored.append(tmp)
        return xored




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
