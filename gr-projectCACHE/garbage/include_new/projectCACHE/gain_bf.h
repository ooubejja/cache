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


#ifndef INCLUDED_PROJECTCACHE_GAIN_BF_H
#define INCLUDED_PROJECTCACHE_GAIN_BF_H

#include <projectCACHE/api.h>
#include <gnuradio/tagged_stream_block.h>

#include <caching/CheckFunction.h>
#include <caching/EnvironmentSetup.h>
#include <caching/DecodingInput.h>
#include <caching/ConflictGraph.h>
#include <caching/randomHandler.h>
#include <caching/DataDefinition.h>
#include <caching/grasp.h>
#include <caching/hgcc.h>
#include <caching/CodingDecodingData.h>
#include <caching/FuncsFromMain.h>
#include <caching/Conversions.h>
#include <caching/TxRx.h>

namespace gr {
  namespace projectCACHE {

    /*!
     * \brief <+description of block+>
     * \ingroup projectCACHE
     *
     */
    class PROJECTCACHE_API gain_bf : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<gain_bf> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of projectCACHE::gain_bf.
       *
       * To avoid accidental use of raw pointers, projectCACHE::gain_bf's
       * constructor is in a private implementation
       * class. projectCACHE::gain_bf::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key, bool DEBUG);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_GAIN_BF_H */

