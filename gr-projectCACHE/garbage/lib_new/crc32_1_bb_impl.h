/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_PROJECTCACHE_CRC32_1_BB_IMPL_H
#define INCLUDED_PROJECTCACHE_CRC32_1_BB_IMPL_H

#include <projectCACHE/crc32_1_bb.h>
#include <boost/crc.hpp>

namespace gr {
  namespace projectCACHE {

    class crc32_1_bb_impl : public crc32_1_bb
    {
     private:
      bool d_check;
      boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true>    d_crc_impl;

     public:
      crc32_1_bb_impl(bool check, const std::string& lengthtagname);
      ~crc32_1_bb_impl();

      int calculate_output_stream_length(const gr_vector_int &ninput_items);
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);

      uint64_t d_npass;
      uint64_t d_nfail;
    };
  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_CRC32_1_BB_IMPL_H */

