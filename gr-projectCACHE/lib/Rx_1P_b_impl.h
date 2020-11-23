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

#ifndef INCLUDED_PROJECTCACHE_RX_1P_B_IMPL_H
#define INCLUDED_PROJECTCACHE_RX_1P_B_IMPL_H

#include <projectCACHE/Rx_1P_b.h>


using namespace std;
using namespace caching;


namespace gr {
  namespace projectCACHE {

    class Rx_1P_b_impl : public Rx_1P_b
    {
     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      Rx_1P_b_impl(unsigned int id_utente, unsigned int n_utenti, unsigned int m_files, unsigned int b_chunks, unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key, bool DEBUG);
      ~Rx_1P_b_impl();
      
      unsigned int d_id_utente;
      unsigned int d_n_utenti;
      unsigned int d_m_files;
      unsigned int d_b_chunks;
      unsigned int d_L_demands;
      unsigned int d_small_packet_len;
      bool d_DEBUG;

      unsigned int d_id_demand;
      unsigned int d_next_id_demand;

      
      data_matrix d_data;
      unsigned int d_id_requested_file;
      vector<unsigned int> d_files_requested;
      unsigned int d_n_package_remains;

      header_transmission d_header;
      vector<char> d_coded_data;

      unsigned short int d_id_small_packet;
      unsigned short int d_id_expected;
      
      unsigned int d_payload_len;
      unsigned int d_number_small_packet;
      unsigned int d_last_small_packet_len;
      unsigned int d_id_last_small_packet;
      unsigned int d_dummy_bytes;

      double d_time_sum;

      unsigned int d_case;




      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);


      void decodeHeader(const char *in);

      void changeRequest();
      
      void decodeSmallPack(const char *in, unsigned int small_pack_len);

      void fill_lacks();

      void decodeLargePack();

      void computeGain();

    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_RX_1P_B_IMPL_H */

