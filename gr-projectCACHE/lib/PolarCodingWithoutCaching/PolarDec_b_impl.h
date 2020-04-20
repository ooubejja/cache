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

#ifndef INCLUDED_PROJECTCACHE_POLARDEC_B_IMPL_H
#define INCLUDED_PROJECTCACHE_POLARDEC_B_IMPL_H

#include <projectCACHE/PolarDec_b.h>
#include <complex.h>

typedef std::complex<float>			gr_complex;
typedef std::complex<double>			gr_complexd;

namespace gr {
  namespace projectCACHE {

    class PolarDec_b_impl : public PolarDec_b
    {
     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      PolarDec_b_impl(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s,const std::string &lengthtagname);
      ~PolarDec_b_impl();

      int d_N;
      int d_K_w;
      int d_K_s;
      float d_SNR_s;
      float d_SNR_w;
      bool d_stop;
      //Data for polar code	
      PC PC_w, PC_s;
      float designSNRdb;
      float variance_w, variance_s;
      float sqrtVariance_w, sqrtVariance_s;
      int * frozen_s, * frozen_w;
      
      double * llr_w;
      //float * recSymbol_w;
      gr_complex * recSymbol_w;
      int * recMessage_w;
      int * recCodeword_w;      

      double * llr_s;
      //float * recSymbol_s;
      gr_complex * recSymbol_s;
      int * recMessage_s;
      int * recCodeword_s;


      //int * sentSymbol;
      gr_complex * sentSymbol;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_POLARDEC_B_IMPL_H */

