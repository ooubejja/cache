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

#ifndef INCLUDED_PROJECTCACHE_GAIN_BF_IMPL_H
#define INCLUDED_PROJECTCACHE_GAIN_BF_IMPL_H

#include <projectCACHE/gain_bf.h>


using namespace std;
using namespace caching;


namespace gr {
  namespace projectCACHE {

    class gain_bf_impl : public gain_bf
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      gain_bf_impl(unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key, bool DEBUG);
      ~gain_bf_impl();


      unsigned int d_id_transmission;
      
      unsigned int d_L_demands;
      unsigned int d_small_packet_len;
      bool d_DEBUG;

      header_transmission d_header;

      unsigned short int d_id_small_packet;
      unsigned short int d_id_expected;

      unsigned int d_last_small_packet_len;
      unsigned int d_id_last_small_packet;

      unsigned int d_id_demand;

      float d_field_len;
      float d_header_len;
      float d_payload_len;
      float d_number_small_packet;
      float d_crc;

      float d_tot_header_len;                             //sum of all coded header_len
      float d_tot_payload_len;                            //sum of all coded payload_len
      float d_tot_number_small_packet;                    //sum of all coded number_small_packet
      float d_tot_crc;

      float d_tot_naive_header_len;                       //sum of all naive header_len
      float d_tot_naive_payload_len;                      //sum of all naive payload_len
      float d_tot_naive_number_small_packet;              //sum of all naive number_small_packet
      float d_tot_naive_crc;

      float d_payload_gain;                               //sum of all payload_gain
      float d_header_gain;                                //sum of all header_gain
      float d_small_pck_gain;                             //sum of all small_pck_gain
      float d_crc_gain;

      float d_average_payload_gain;                       //average of payload_gain: (d_payload_gain / d_id_transmission)
      float d_average_header_gain;                        //average of header_gain: (d_header_gain / d_id_transmission)
      float d_average_small_pck_gain;                     //average of small_pck_gain: (d_small_pck_gain / d_id_transmission)
      float d_average_crc_gain;

      float d_tot_payload_gain;                           //total payload_gain: (d_tot_payload_len / d_tot_naive_payload_len)
      float d_tot_header_gain;                            //total header_gain: (d_tot_header_len / d_tot_naive_header_len)
      float d_tot_small_pck_gain;                         //total small_pck_gain: (d_tot_number_small_packet / d_tot_naive_number_small_packet)
      float d_tot_crc_gain;


      float d_tot_byte_coded;
      float d_tot_byte_naive;
      
      unsigned int d_case;

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);


      void decodeHeader(const char *in);

      void cleanVar();

      void computeGain();

    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_GAIN_BF_IMPL_H */

