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

using namespace caching;

typedef std::complex<float> gr_complex;
typedef std::complex<double> gr_complexd;

namespace gr {
  namespace projectCACHE {

    class PolarDec_b_impl : public PolarDec_b
    {
     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      //PolarDec_b_impl(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s,const std::string &lengthtagname);
      PolarDec_b_impl(const int , const int , const int , int , int  , int , int , unsigned int , const std::string &);
      ~PolarDec_b_impl();

      //Values to calculate the SNR
      double d_y1, d_y2;
      double d_alpha, d_beta;
      std::vector<double> d_snr;
      std::vector<int> d_size;

      int d_N, d_K_w, d_K_s;
      float d_SNR;
      bool d_stop, d_isStr;
      int d_m_files, d_b_chunks, d_n_users, d_id_user, d_k;
      unsigned int d_packet_remain;
      int d_payload_len;
      unsigned int d_last_spack_len, d_id_last_spack, d_spack_len, d_id_spack, d_nb_spack;
      unsigned int d_id_expected;
      bool d_crc;

      //Data for polar code
      header_polar d_header;
      PC PC_w, PC_s;
      float designSNRdb;
      float variance_w, variance_s;
      float sqrtVariance_w, sqrtVariance_s;
      int * frozen_s, * frozen_w;
      vector<gr_complex> d_coded_packet;
      vector<char> decoded_data;

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

      // othmane
      int cnt ;

      //int * sentSymbol;
      gr_complex * sentSymbol;

      void reinitialize();
      int update(int , const gr_complex *);
      double snr();
      int decode_header(const gr_complex *in);

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_POLARDEC_B_IMPL_H */
