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

#ifndef INCLUDED_PROJECTCACHE_POLARENC_B_IMPL_H
#define INCLUDED_PROJECTCACHE_POLARENC_B_IMPL_H

#include <projectCACHE/polarEnc_b.h>

namespace gr {
  namespace projectCACHE {

    class polarEnc_b_impl : public polarEnc_b
    {
     private:
      // Nothing to declare in this block.

     public:
      polarEnc_b_impl(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s);
      ~polarEnc_b_impl();

      int d_N, d_t, d_i, d_r;
      int d_K_w;
      int d_K_s;
      float d_SNR_s;
      float d_SNR_w;
      bool d_gen;
      bool d_stop;

      //Data for polar code	
      PC PC_w, PC_s;
      float designSNRdb;
      float variance_w, variance_s;
      float sqrtVariance_w, sqrtVariance_s;
      
      int * info_w;
      int * frozen_w;
      float * recSymbol_w;
      
      int * info_s;
      int * frozen_s;
      int * initialMessage;
      int * sentMessage;
      int * sentCodeword;
      int * sentSymbol;

      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_POLARENC_B_IMPL_H */

