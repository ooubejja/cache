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


#ifndef INCLUDED_PROJECTCACHE_POLARENC_B_H
#define INCLUDED_PROJECTCACHE_POLARENC_B_H

#include <projectCACHE/api.h>
#include <gnuradio/sync_block.h>

#include <caching/PC.h>

namespace gr {
  namespace projectCACHE {

    /*!
     * \brief <+description of block+>
     * \ingroup projectCACHE
     *
     */
    class PROJECTCACHE_API polarEnc_b : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<polarEnc_b> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of projectCACHE::polarEnc_b.
       *
       * To avoid accidental use of raw pointers, projectCACHE::polarEnc_b's
       * constructor is in a private implementation
       * class. projectCACHE::polarEnc_b::make is the public interface for
       * creating new instances.
       */
      static sptr make(const int N, const int K_w, const int K_s, float SNR_w, float SNR_s);
    };

  } // namespace projectCACHE
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_POLARENC_B_H */

