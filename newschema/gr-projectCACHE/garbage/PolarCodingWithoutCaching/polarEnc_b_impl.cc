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
#include "polarEnc_b_impl.h"
#include <iostream>
#include <cmath>
#include <time.h>
#include <random>

namespace gr {
  namespace projectCACHE {

    polarEnc_b::sptr
    polarEnc_b::make(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s)
    {
      return gnuradio::get_initial_sptr
        (new polarEnc_b_impl(N, K_w, K_s, SNR_w, SNR_s));
    }

    /*
     * The private constructor
     */
    polarEnc_b_impl::polarEnc_b_impl(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s)
      : gr::sync_block("polarEnc_b",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(1, 2, sizeof(char))),
      d_N(N),
      d_K_w(K_w),
      d_K_s(K_s),
      d_SNR_w(SNR_w),
      d_SNR_s(SNR_s)
    {
      d_gen = true; d_stop = false;
      d_t = 0; d_r = N; d_i = 0; // d_r is the nb of remaning symbols to send by the grc block

      designSNRdb = 0;

      // Weak user
      /*variance_w = pow(10,-(d_SNR_w/10));
      sqrtVariance_w = sqrt(variance_w);*/

      PC_w.constructPC(d_N, d_K_w, designSNRdb);

      info_w = new int[d_K_w];
      frozen_w = new int[d_N-d_K_w];


      // Strong user
      variance_s = pow(10,-(d_SNR_s/10));
      sqrtVariance_s = sqrt(variance_s);

      //PC_s.constructPC(d_N, d_K_s, designSNRdb);
      PC_s.initPC(d_N, d_K_s, designSNRdb);
      PC_s.setGenMatrix(PC_w.genMatrix);
      PC_s.setRn(PC_w.Rn);
      PC_s.setArrangedBits(PC_w.arragedBits);


      info_s = new int[d_K_s];
      frozen_s = new int[d_N-d_K_s];


      // Same for both users
      sentMessage = new int[d_N];
      initialMessage = new int[d_N];
      sentCodeword = new int[d_N];
      sentSymbol = new int[d_N];

    }

    /*
     * Our virtual destructor.
     */
    polarEnc_b_impl::~polarEnc_b_impl()
    {
    }


    int
    polarEnc_b_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];
      char *out = (char *) output_items[0];


      if(d_gen){
        // Do 
        cout << "Polar codes" << endl; // prints Polar codes
        d_gen = false;

        srand (time(NULL));

        for (int i=0; i<d_K_w; i++){
          if(i%2==0){
            initialMessage[i] = rand()%2;//0
            info_w[i] = initialMessage[i];
          } else{
            initialMessage[i] = rand()%2;//1
            info_w[i] = initialMessage[i];
          }
          //initialMessage[i] = rand()%2;
        }

        for (int i=d_K_w; i<d_K_s; i++){
         initialMessage[i] = 1;
         frozen_w[i-d_K_w] = initialMessage[i];
         info_s[i-d_K_w] = initialMessage[i];
        }

        for (int i=d_K_s; i<d_N; i++){
          initialMessage[i] = 0;
          frozen_w[i-d_K_w] = initialMessage[i];
          frozen_s[i-d_K_s] = initialMessage[i];
        }


        PC_w.encode(info_w, frozen_w, sentMessage, sentCodeword, sentSymbol);
        cout << "The sent message is :" << endl;
        for (int i=0; i<d_N; i++){
         cout << initialMessage[i] << " ";
        }

        /*cout << "\n\nThe sent codeword is :" << endl;
        for (int i = 0; i < d_N; i++)
        {
          cout << sentCodeword[i] << " ";
        }*/
      
      }

        /*if(!d_stop){
          if (noutput_items < d_r)
          {
            d_r -= noutput_items;
            //cout << "remain: " << d_r << endl;
            for (int i = 0; i <noutput_items ; i++){
              out[i] = (char) sentCodeword[i];
              //cout << sentSymbol[i] << " ";
            }
            return noutput_items;  
              
          } else{
            d_stop = true;
            for (int i = 0; i <d_r ; i++){
             out[i] = (char) sentCodeword[i];
             //cout << sentSymbol[i] << " ";
            }
            return noutput_items;          
          }
        } else {
          return -1;
        }*/
      if(!d_stop){
        d_stop = true;
        for (int i = 0; i <d_N ; i++){
              out[i] = (char) sentCodeword[i];
        }
        cout << "\noutput size is: " << noutput_items << endl;
        // Tell runtime system how many output items we produced.
        return noutput_items;//d_N
      }
      else 
        return -1;
    }/* work function */
  } /* namespace projectCACHE */
} /* namespace gr */

