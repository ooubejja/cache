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

#ifndef INCLUDED_PROJECTCACHE_MAP_HEADER_PAYLOAD_BC_IMPL_H
#define INCLUDED_PROJECTCACHE_MAP_HEADER_PAYLOAD_BC_IMPL_H

#include <projectCACHE/map_header_payload_bc.h>

using namespace caching;

namespace gr {
  namespace projectCACHE {

    class map_header_payload_bc_impl : public map_header_payload_bc
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      map_header_payload_bc_impl(int hx_maptype, int payload_maptype, const std::string &lengthtagname);
      ~map_header_payload_bc_impl();
      
      gr_complex d_constellation[4];
      int d_k;
      int d_payloadmod, d_hxmod;
      pmt::pmt_t d_packet_len_pmt;      
      const pmt::pmt_t d_len_tag_key;
      
      // Where all the action really happens
      int work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_MAP_HEADER_PAYLOAD_BC_IMPL_H */

