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


#ifndef INCLUDED_PROJECTCACHE_TX_1P_B_H
#define INCLUDED_PROJECTCACHE_TX_1P_B_H

#include <projectCACHE/api.h>
#include <gnuradio/sync_block.h>


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
    class PROJECTCACHE_API Tx_1P_b : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Tx_1P_b> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of projectCACHE::Tx_1P_b.
       *
       * To avoid accidental use of raw pointers, projectCACHE::Tx_1P_b's
       * constructor is in a private implementation
       * class. projectCACHE::Tx_1P_b::make is the public interface for
       * creating new instances.
       */
      static sptr make(bool GRASP, unsigned int n_utenti, unsigned int m_files, unsigned int b_chunks, unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_TX_1P_B_H */

