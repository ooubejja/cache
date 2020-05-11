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

using namespace caching;

namespace gr {
  namespace projectCACHE {

    class polarEnc_b_impl : public polarEnc_b
    {
     private:
      const pmt::pmt_t msg_port, cw_port;

     public:
      polarEnc_b_impl(const int N, const int K_w, const int K_s, int m_files, int b_chunks, int nb_strg, int id_user, int spack_len, const std::string &len_tag_key);
      ~polarEnc_b_impl();

      //polar code parameters
      int d_N, d_t, d_r;
      int d_K_w, d_K_s;
      float d_SNR_s, d_SNR_w;
      bool d_gen,d_stop;

      //Data for polar code
      PC PC_w, PC_s;
      float designSNRdb,variance_w, variance_s;
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

      //Caching scheme data
      int d_m_files, d_b_chunks, d_id_demand, d_nb_strg, d_id_user,d_spack_len;
      data_matrix d_data;
      cf_data d_outputForColoring;
      bool d_GRASP, DEBUG;
      int d_n_col, d_maxIter;
      int *d_coloring;
      unsigned int d_packet_remain;

      header_transmission *d_header_data;
      header_transmission *d_hdr_sdata;

      vector< vector<char> > d_coded_data;
      vector< vector<char> > d_strg_data;
      vector< vector<char> > d_PC_data;
      vector<header_polar> d_hX;
      vector<vector<char> > d_transmission;
      vector<char> d_transmission1;
      vector<vector<bool> > G_edges;
      vector<vector<int> >  d_bits_coded;
      vector<int> d_spack_size;

      pmt::pmt_t d_packet_len_pmt;
      pmt::pmt_t d_len_tag_key;
      uint64_t d_next_tag_pos;

      int d_i, d_cc, d_k;
      int d_offset;

      void cleanVar();

      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_POLARENC_B_IMPL_H */
