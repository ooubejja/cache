/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_PROJECTCACHE_TX_1P_B_IMPL_H
#define INCLUDED_PROJECTCACHE_TX_1P_B_IMPL_H

#include <projectCACHE/Tx_1P_b.h>


using namespace std;
using namespace caching;


namespace gr {
  namespace projectCACHE {

    class Tx_1P_b_impl : public Tx_1P_b
    {
     public:
      Tx_1P_b_impl(bool GRASP, unsigned int n_utenti, unsigned int m_files, unsigned int b_chunks, unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key);
      ~Tx_1P_b_impl();

      bool d_GRASP;
      unsigned int d_n_utenti;
      unsigned int d_m_files;
      unsigned int d_b_chunks;
      unsigned int d_L_demands;
      unsigned int d_small_packet_len;

      data_matrix d_data;
      cf_data d_outputForColoring;
      int *d_coloring;
      int d_n_col;
      unsigned int d_total_small_packet_size;

      header_transmission *d_header_data;
      header_transmission *d_hdr_sdata;
      vector< vector<char> > d_coded_data;
      vector< vector<char> > d_strg_data;
      vector< vector<char> > d_PC_data;
      vector<char> d_transmission;
      vector<unsigned int> d_small_packet_size;
      unsigned int d_id_demand;

      pmt::pmt_t d_packet_len_pmt;
      pmt::pmt_t d_len_tag_key;

      bool d_gen;

      int d_i;
      int d_j;
      int d_cc;
      int d_k;
      int d_pack;
      int d_last;
      uint64_t d_next_tag_pos;


      void cleanVar();
      

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);

      

    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_TX_1P_B_IMPL_H */

