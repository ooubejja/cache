#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Polar Code
# Generated: Wed Sep 25 16:02:06 2019
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.digital.utils import tagged_streams
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import numpy
import projectCACHE
import random
import sys


class polar_code(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Polar Code")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Polar Code")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "polar_code")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.Kw = Kw = 70*8
        self.samp_rate = samp_rate = 32000
        self.packetlength = packetlength = "packet_len"
        self.length_tag_key = length_tag_key = "packet_len"
        self.id_user = id_user = 2
        self.SNRw = SNRw = 3
        self.SNRs = SNRs = 8
        self.Nbfiles = Nbfiles = 20
        self.NbStrgUsers = NbStrgUsers = 2
        self.NbChuncks = NbChuncks = 100
        self.N = N = 2048
        self.Ks = Ks = 2*Kw

        ##################################################
        # Blocks
        ##################################################
        self.projectCACHE_polarEnc_b_0 = projectCACHE.polarEnc_b(N, Kw, Ks, SNRw, SNRs, Nbfiles, NbChuncks, NbStrgUsers, id_user, "packet_len")
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_char*1, 'TX', ""); self.blocks_tag_debug_0.set_display(True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.projectCACHE_polarEnc_b_0, 0), (self.blocks_tag_debug_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "polar_code")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_Kw(self):
        return self.Kw

    def set_Kw(self, Kw):
        self.Kw = Kw
        self.set_Ks(2*self.Kw)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_packetlength(self):
        return self.packetlength

    def set_packetlength(self, packetlength):
        self.packetlength = packetlength

    def get_length_tag_key(self):
        return self.length_tag_key

    def set_length_tag_key(self, length_tag_key):
        self.length_tag_key = length_tag_key

    def get_id_user(self):
        return self.id_user

    def set_id_user(self, id_user):
        self.id_user = id_user

    def get_SNRw(self):
        return self.SNRw

    def set_SNRw(self, SNRw):
        self.SNRw = SNRw

    def get_SNRs(self):
        return self.SNRs

    def set_SNRs(self, SNRs):
        self.SNRs = SNRs

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


def main(top_block_cls=polar_code, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
