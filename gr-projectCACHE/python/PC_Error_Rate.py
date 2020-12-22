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
import random

class PC_Error_Rate(gr.basic_block):
    """
    Concatenates message data coming from inputs (Codewords and/or messages)
    """
    def __init__(self, usr_id, nb_chunks):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.basic_block.__init__(
            self,
            name='PC Error Rate',   # will show up in GRC
            in_sig=[],
            out_sig=[]
        )

        self.message_port_register_in(pmt.to_pmt("CH_USE"))
        self.set_msg_handler(pmt.intern("CH_USE"), self.handle_ch_use)
        self.message_port_register_in(pmt.to_pmt("SNR"))
        self.set_msg_handler(pmt.intern("SNR"), self.handle_snr)

        self.message_port_register_in(pmt.to_pmt("BER_INFO"))
        self.set_msg_handler(pmt.intern("BER_INFO"), self.handle_ber_info)

        self.lock = threading.Lock()

        self.usr_id = str(usr_id)

        self.nb_chunks = nb_chunks

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

        self.cnt_rx_chnk = 0
        self.ready_ber = False

        self.chunk_size = 0

        # self.filename = "../trasmissioni/ERROR_RATE_"+time.strftime("%d%m%Y-%H%M%S")+".txt"
        self.filename = "../trasmissioni/ERROR_RATE_" + self.usr_id + ".txt"
        with open(self.filename,"w") as f:
            template =                                          \
            "============================================"                            \
            +"\n"+"USER ID: " + self.usr_id + " | "                        \
            +"\n"+"============================================"        \
            +"\n"+"Number of chunks | bits sent: "                            \
            +"\n\n"+"============================================"        \
            +"\n"+"Number of chunks | bits decoded: "                            \
            +"\n\n"+"============================================"        \
            +"\n"+"Error sum: "                            \
            +"\n\n"+"============================================"        \
            +"\n"+"Bit Error Rate: "                            \
            +"\n\n\n\n"+"============================================"        \
            +"\n"+"Average SNR: "                            \
            +"\n\n\n\n"+"============================================"        \
            +"\n"+"Number of OFDM symbols | Correct Bits  | Throughput (Bits/Channel use): "                            \
            +"\n\n"+"============================================"        \

            f.write(template)



    def handle_snr(self, msg_pmt):
        with self.lock :
            self.cnt_snr += 1
            tmp = pmt.to_python(msg_pmt)
            self.SNR += [float(tmp)]


    def handle_ch_use(self, msg_pmt):
        with self.lock :
            tmp = pmt.to_python(msg_pmt)
            # tmp = map(chr,map(int,tmp[1]))
            tmp = tmp[1]
            # print len(tmp)
            # tmp = ''.join(tmp)
            # self.cnt_ch_use = int(tmp)
            self.cnt_ch_use += len(tmp)


    def handle_ber_info(self, msg_pmt):
        with self.lock :
            tmp = pmt.to_python(msg_pmt)
            tmp = map(chr,map(int,tmp[1]))
            tmp = ''.join(tmp)
            vec = tmp.split()

            # print "BER FILE"
            # print vec[0]
            # print self.usr_id

            if vec[0] == self.usr_id :
                ## self.usr_id | file_id | chunks_mx
                usr_id = self.usr_id
                file_id = vec[1]
                chunks_mx = vec[2]
                tx_filename = "../repository/file_" + file_id + ".xml"
                with open(tx_filename,"r") as f:
                    text = f.read()

                self.chunk_size = len(text)/self.nb_chunks+1    # it's 70 (ceiled). Last chunk is 4 bytes
                tx_chunk = ""
                rx_chunk = ""
                total_bits_tx = -1

                time.sleep(2)

                nb_tx_chunk = sum([int(digit) for digit in chunks_mx])

                for i in range(len(chunks_mx)):
                    ## if current chunk is not in cache, compute error rate
                    ## Get Tx bits size and write it once for all to avoid reading last chunk (smaller)
                    if i == 0 :
                        with open(self.filename,"r") as f:
                            lines = f.readlines()
                            for j in range(len(lines)):
                                if 'bits sent:' in lines[j] :
                                    tx_chunk = text[self.chunk_size*i:self.chunk_size*(i+1)]
                                    total_bits_tx = nb_tx_chunk*len(tx_chunk)*8
                                    lines[j+1] = str(nb_tx_chunk) + " | " + str(total_bits_tx) +'\n'
                                    with open(self.filename,"w") as f:
                                        f.write(''.join(lines))
                    if chunks_mx[i] == '1' :
                        ## Get Tx chunk :
                        try :
                            tx_chunk = text[self.chunk_size*i:self.chunk_size*(i+1)]
                        except :
                            tx_chunk = text[self.chunk_size*i:-1]

                        ## Get Rx chunk :
                        ## Looks like this : ../cache/UserCache/user_XX/YY_ZZ.cache
                        rx_filename = "../cache/UserCache/user_" + usr_id + "/" + file_id + "_" + str(i) + ".cache"
                        try :
                            ## SUCCESSFULLY Decoded chunk counter
                            with open(rx_filename,"r") as f:
                                rx_chunk = f.read()
                                self.cnt_rx_chnk += 1

                            print "Found Valid RX Chunk : " + rx_filename
                            # print "BER XOR"
                            # print tx_chunk
                            # print rx_chunk
                            tx_chunk = ''.join(format(ord(x), 'b') for x in tx_chunk)
                            rx_chunk = ''.join(format(ord(x), 'b') for x in rx_chunk)
                            rx_chunk = rx_chunk[:len(tx_chunk)]

                            # print a

                            a = self.xor_two_str(tx_chunk,rx_chunk)     # Compare with Rx chunk

                            ##########################################
                            # Most important metric
                            if sum(map(int,a)) > 1 :
                                self.sum_errors_cw += sum(map(int,a))
                            ##########################################
                            self.CW_BER = self.sum_errors_cw/float(total_bits_tx)  # Message Bit Error Rate, updated every iteration


                            with open(self.filename,"r") as f:
                                lines = f.readlines()
                                for j in range(len(lines)):

                                    if 'bits decoded:' in lines[j]:
                                        try :
                                            lines[j+1] = str(self.cnt_rx_chnk) + " | " + str(self.cnt_rx_chnk*self.chunk_size*8) + '\n'
                                        except :
                                            pass
                                    if 'Bit Error Rate' in lines[j]:
                                        lines[j+1] = lines[j+1][:-1] + '[' + "%02d"%i + '] ' + str(self.CW_BER)+ " " + '\n'
                                    if 'Error sum' in lines[j]:
                                        lines[j+1] = lines[j+1][:-1] + '[' + "%02d"%i + '] ' + str(self.sum_errors_cw)+ " " + '\n'
                                    if 'SNR' in lines[j] and self.cnt_snr > 0:
                                        lines[j+1] = str(sum(self.SNR)/float(self.cnt_snr)) + '\n'
                                    if 'Channel' in lines[j] and self.cnt_ch_use > 0 and self.chunk_size > 0:
                                        # print "HERE HERE"
                                        total_bits_rx = self.cnt_rx_chnk*self.chunk_size*8
                                        total_errors = self.sum_errors_cw
                                        successful_bits = total_bits_rx - total_errors

                                        lines[j+1] = str(self.cnt_ch_use) + "\t\t\t\t\t| " + str(successful_bits) + "\t\t\t| " + str(successful_bits/float(self.cnt_ch_use)) + '\n'


                            with open(self.filename,"w") as f:
                                f.write(''.join(lines))


                        except :
                            print "RX Chunk Not Found: " + rx_filename
                            # break



    def xor_two_str(self,a,b):
        xored = []
        tmp = ''
        for i in range(max(len(a), len(b))):
            try :
                xored_value = ord(a[i%len(a)]) ^ ord(b[i%len(b)])
                tmp = hex(xored_value)[2:]
                if tmp not in ['0','1'] :   # Bug : In polardec, recCodeword is sometimes different than 0 or 1
                     tmp = '1'
            except:
                tmp = '1'
            xored.append(tmp)
        return xored
