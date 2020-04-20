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


#ifndef INCLUDED_PROJECTCACHE_CRC32_1_BB_H
#define INCLUDED_PROJECTCACHE_CRC32_1_BB_H

#include <projectCACHE/api.h>
#include <gnuradio/block.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace projectCACHE {

    /*!
     * \brief Byte-stream CRC block
     * \ingroup packet_operators_blk
     *
     * \details
     * Input: stream of bytes, which form a packet. The first byte of the packet
     * has a tag with key "length" and the value being the number of bytes in the
     * packet.
     *
     * Output: The same bytes as incoming, but trailing a CRC32 of the packet.
     * The tag is re-set to the new length.
     */
    class PROJECTCACHE_API crc32_1_bb : virtual public tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<crc32_1_bb> sptr;

      /*!
       * \param check Set to true if you want to check CRC, false to create CRC.
       * \param lengthtagname Length tag key
       */
      static sptr make(bool check=false, const std::string& lengthtagname="packet_len");
    };


  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_CRC32_1_BB_H */

