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


#ifndef INCLUDED_PROJECTCACHE_MAP_HEADER_PAYLOAD_BC_H
#define INCLUDED_PROJECTCACHE_MAP_HEADER_PAYLOAD_BC_H

#include <projectCACHE/api.h>
#include <gnuradio/tagged_stream_block.h>
#include <caching/Conversions.h>

namespace gr {
  namespace projectCACHE {

    /*!
     * \brief <+description of block+>
     * \ingroup projectCACHE
     *
     */
    class PROJECTCACHE_API map_header_payload_bc : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<map_header_payload_bc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of projectCACHE::map_header_payload_bc.
       *
       * To avoid accidental use of raw pointers, projectCACHE::map_header_payload_bc's
       * constructor is in a private implementation
       * class. projectCACHE::map_header_payload_bc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int , int , const std::string &lengthtagname);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_MAP_HEADER_PAYLOAD_BC_H */

