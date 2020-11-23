/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "PolarDec_b_impl.h"
#include <iostream>
#include <cmath>
#include <time.h>
#include <random>

namespace gr {
  namespace projectCACHE {

    PolarDec_b::sptr
    PolarDec_b::make(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s, const std::string &lengthtagname)
    {
      return gnuradio::get_initial_sptr(new PolarDec_b_impl(N, K_w, K_s, SNR_w, SNR_s,lengthtagname));
    }

    /*
     * The private constructor
     */
    PolarDec_b_impl::PolarDec_b_impl(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s, const std::string &lengthtagname)
      : gr::tagged_stream_block("PolarDec_b",
                            gr::io_signature::make(1, 1, sizeof(float)),
                            gr::io_signature::make(0, 0, 0), lengthtagname),
      d_N(N),
      d_K_w(K_w),
      d_K_s(K_s),
      d_SNR_w(SNR_w),
      d_SNR_s(SNR_s)
    {
      d_stop = false;
      designSNRdb = 0;

      // Weak user
      variance_w = pow(10,-(d_SNR_w/10));
      sqrtVariance_w = sqrt(variance_w);

      PC_w.constructPC(d_N, d_K_w, designSNRdb);

      frozen_w = new int[d_N-d_K_w];
      for (int i = 0; i < d_K_s-d_K_w; i++)
        frozen_w[i] = 1;

      for (int i = d_K_s-d_K_w; i < d_N-d_K_w; i++)
        frozen_w[i] = 0;


      // Strong user
      variance_s = pow(10,-(d_SNR_s/10));
      sqrtVariance_s = sqrt(variance_s);

      PC_s.initPC(d_N, d_K_s, designSNRdb);
      PC_s.setGenMatrix(PC_w.genMatrix);
      PC_s.setRn(PC_w.Rn);
      PC_s.setArrangedBits(PC_w.arragedBits);


      frozen_s = new int[d_N-d_K_s];
      for (int i = 0; i < d_N-d_K_s; i++)
        frozen_s[i] = 0;


      // Same for both users
      sentSymbol = new int[d_N];

      //Receiver
      llr_w = new double[d_N];
      recSymbol_w = new float[d_N];
      recMessage_w = new int[d_N];
      recCodeword_w = new int[d_N];

      llr_s = new double[d_N];
      recSymbol_s = new float[d_N];
      recMessage_s = new int[d_N];
      recCodeword_s = new int[d_N];

    }

    /*
     * Our virtual destructor.
     */
    PolarDec_b_impl::~PolarDec_b_impl()
    {
    }

    int
    PolarDec_b_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = 0;
      return noutput_items ;
    }

    int
    PolarDec_b_impl::work(int noutput_items,
                        gr_vector_int &ninput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      //const char *in2 = (const char *) input_items[1];
      float *out = (float *) output_items[0];
      
      int packet_length = ninput_items[0];


      if (!d_stop)
      {
        d_stop = true;

        for (int i = 0; i < d_N; i++)
          sentSymbol[i] = (int) in[i];
        /*for (int i = 0; i < d_N; i++)
          recSymbol_w[i] = in[i];
        cout << "\n\nThe sent signal is: " << endl;
        for (int i = 0; i < d_N; i++)
          cout << sentSymbol[i] << " " ;*/

        // Weak receiver channel + decoding
        PC_w.noise(sentSymbol, recSymbol_w, sqrtVariance_w);
        /*cout << "\n\nThe received signal is: " << endl;
        for (int i = 0; i < d_N; i++)
          cout << recSymbol_w[i] << " " ;*/

        PC_w.computeLLR(llr_w,recSymbol_w,variance_w);
        PC_w.SC(recMessage_w, recCodeword_w, llr_w, frozen_w);

        /*cout << "\n\nThe received codeword is :" << endl;
        for (int i = 0; i < d_N; i++)
          cout << recCodeword_w[i] << " ";*/

        cout << "\n\nThe weak decoded message is :" << endl;
        for (int i = 0; i < d_K_w; i++)
          cout << recMessage_w[i] << " ";
        cout << endl;


        // Strong receiver channel + decoding
        PC_s.noise(sentSymbol, recSymbol_s, sqrtVariance_s);
        PC_s.computeLLR(llr_s,recSymbol_s,variance_s);
        PC_s.SC(recMessage_s, recCodeword_s, llr_s, frozen_s);

        cout << "\n\nThe Strong decoded message is :" << endl;
        for (int i = 0; i < d_N; i++)
          cout << recMessage_s[i] << " ";
        cout << endl;

        /*for (int i=0; i<d_N; i++)
        {
          if (recMessage_s[i] != sentMessage[i])
            error_s = error_s + 1;
        }

        if (error_s != 0)
          cwErrorSum_s = cwErrorSum_s + 1;

        bitErrorSum_s = bitErrorSum_s + error_s;*/
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
      }

  } /* namespace projectCACHE */
} /* namespace gr */

