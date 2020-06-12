/* -*- c++ -*- */
/*
 * Copyright 2020 <+YOU OR YOUR COMPANY+>.
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
#include "map_header_payload_bc_impl.h"

#define SQRT_TWO 0.707107

using namespace std;
using namespace caching;

namespace gr {
  namespace projectCACHE {

    map_header_payload_bc::sptr
    map_header_payload_bc::make(int hx_maptype, int payload_maptype, const std::string &lengthtagname)
    {
      return gnuradio::get_initial_sptr
        (new map_header_payload_bc_impl(hx_maptype, payload_maptype, lengthtagname));
    }

    /*
     * The private constructor
     */
    map_header_payload_bc_impl::map_header_payload_bc_impl(int hx_maptype, int payload_maptype, const std::string &lengthtagname)
      : gr::tagged_stream_block("map_header_payload_bc",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), lengthtagname),
      d_hxmod(hx_maptype),
      d_payloadmod(payload_maptype),
      d_len_tag_key(pmt::string_to_symbol(lengthtagname))
    {

      d_k=0;

      //QPSK constellation map
      d_constellation[0] = gr_complex(-SQRT_TWO, -SQRT_TWO);
      d_constellation[1] = gr_complex(SQRT_TWO, -SQRT_TWO);
      d_constellation[2] = gr_complex(-SQRT_TWO, SQRT_TWO);
      d_constellation[3] = gr_complex(SQRT_TWO, SQRT_TWO);

      // set_tag_propagation_policy(TPP_DONT);

    }

    /*
     * Our virtual destructor.
     */
    map_header_payload_bc_impl::~map_header_payload_bc_impl()
    {
    }

    int
    map_header_payload_bc_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {

      int noutput_items;
      /*The output length is equal to:
        16 (BPSK to the first two byte)
      + (input_bytes length - the first two bytes)* [4(if QPSK mapping) 8(if BPSK mapping)]
      */
      /*int length = ninput_items[0];

      if((length-6)%7==0)
        noutput_items = 8*(length-2)+16;
      else
        noutput_items = 4*(length-2)+16;*/
      noutput_items = 4*ninput_items[0];

      //cout << ninput_items[0] << ", " << noutput_items << endl;
      return noutput_items ;
    }

    int
    map_header_payload_bc_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      int length_subpacket = ninput_items[0];
      vector<unsigned int> data_bits;
      vector<unsigned int> data_symb;
      int out_len;

      //cout << d_k << ", " << length_subpacket << endl;

      //1. Convert the packet char to a stream of bits
      data_bits = conv_stream_char_to_bits(in, length_subpacket);

      //2. Convert to QPSK modulation
      out_len = 4*length_subpacket;
      for(int i=0; i<out_len; i++){
        data_symb.push_back(0);
          for (int k = 0; k < 2; k++){
              data_symb[i] += data_bits[2*i+k]*pow(2,k);
          }
          //Transform packed bits to symbol
          switch(data_symb[i]){
              case 0:
                  out[i] = d_constellation[0];
                  break;
              case 1:
                  out[i] = d_constellation[1];
                  break;
              case 2:
                  out[i] = d_constellation[2];
                  break;
              case 3:
                  out[i] = d_constellation[3];
                  break;
          }
          // if((length_subpacket-6)%7 != 0){
          if((length_subpacket)%7 != 0){
            if(i>=8)  // boost small packet id too
              out[i] /= 4 ;  // 20log10(8) = 18 dB (difference between hdr and pld)
          }
      }


/*********************THIS PART SUPPORT MULTIPLE MODULATION FOR THE DIFFERENT PACKET***********************/
      // 2. Convert the first two bytes that contains the packet ID to BPSK
      /*int twoByteLen = 8;
      for (int i = 0; i < twoByteLen; ++i){
        out[i] = data_bits[i]? gr_complex(1,0): gr_complex(-1,0);
        //cout << data_bits[i] << ", " << out[i] << ", ";
      }*/
      //cout << endl;

      //(length_subpacket-6)%7 this verify if the packet a header or not
      /*if((length_subpacket-6)%7==0){

        if(d_hxmod == 1){
          out_len = 8*length_subpacket;
          //2. MAP to BPSK FOR THE HEADER
          for (int i = twoByteLen; i < out_len; ++i)
          {
            if(data_bits[i]==0)
              out[i] = gr_complex(-1,0);
            else
              out[i] = gr_complex(1,0);
          }
        }

      }else{
        if(d_payloadmod == 2){
          //3. MAP QPSK FOR THE PAYLOAD

          //Pack each two bits into one symb in [0, 1, 2, 3]
          int j=twoByteLen;
          out_len = 4*(length_subpacket-2)+16;
          for(int i=twoByteLen; i<out_len; i++){
            data_symb.push_back(0);
              for (int k = 0; k < 2; k++){
                  data_symb[i-twoByteLen] += data_bits[j++]*pow(2,k);
              }
              //Transform packed bits to symbol
              switch(data_symb[i-twoByteLen]){
                  case 0:
                      out[i] = d_constellation[0];
                      break;
                  case 1:
                      out[i] = d_constellation[1];
                      break;
                  case 2:
                      out[i] = d_constellation[2];
                      break;
                  case 3:
                      out[i] = d_constellation[3];
                      break;
              }
          }
        }

      }*/
/********************************************************************************************/


      //Increment d_k, that store the number of sent packets by this block
      d_k ++;
      //cout << out_len << endl;

      return out_len;

    }

  } /* namespace projectCACHE */
} /* namespace gr */
