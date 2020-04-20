/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_PROJECTCACHE_POLARDEC_B_H
#define INCLUDED_PROJECTCACHE_POLARDEC_B_H

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
#include <caching/PC.h>
#include <string>

namespace gr {
  namespace projectCACHE {

    /*!
     * \brief <+description of block+>
     * \ingroup projectCACHE
     *
     */
    class PROJECTCACHE_API PolarDec_b : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<PolarDec_b> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of projectCACHE::PolarDec_b.
       *
       * To avoid accidental use of raw pointers, projectCACHE::PolarDec_b's
       * constructor is in a private implementation
       * class. projectCACHE::PolarDec_b::make is the public interface for
       * creating new instances.
       */
      //static sptr make(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s,const std::string &lengthtagname);
      static sptr make(const int , const int , const int , int , int  , int , int , unsigned int , const std::string &);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_POLARDEC_B_H */

