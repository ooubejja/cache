/* -*- c++ -*- */
/* Copyright 2012 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_PROJECTCACHE_OFDM_FRAME_EQUALIZER1_VCVC_IMPL_H
#define INCLUDED_PROJECTCACHE_OFDM_FRAME_EQUALIZER1_VCVC_IMPL_H

#include <projectCACHE/ofdm_frame_equalizer1_vcvc.h>

namespace gr {
  namespace projectCACHE {

    class ofdm_frame_equalizer1_vcvc_impl : public ofdm_frame_equalizer1_vcvc
    {
     private:
      //! If \p d_occupied_carriers[k][l] is true, symbol k, carrier l is carrying data.
      //  (this is a different format than occupied_carriers!)
      std::vector<bool> d_occupied_carriers;
      //! If \p d_pilot_carriers[k][l] is true, symbol k, carrier l is carrying data.
      //  (this is a different format than pilot_carriers!)
      std::vector<std::vector<bool> > d_pilot_carriers;
      //! If \p d_pilot_carriers[k][l] is true, d_pilot_symbols[k][l] is its tx'd value.
      //  (this is a different format than pilot_symbols!)
      std::vector<std::vector<gr_complex> > d_pilot_symbols;
      //! In case the frame doesn't begin with OFDM symbol 0, this is the index of the first symbol
      int d_symbols_skipped;
      //! The current position in the set of pilot symbols
      int d_pilot_carr_set;

     protected:
      

     public:
      void parse_length_tags(
	  const std::vector<std::vector<tag_t> > &tags,
	  gr_vector_int &n_input_items_reqd
      );
      const int d_fft_len;
      const int d_cp_len;
      bool d_propagate_channel_state;
      std::vector<gr_complex> d_channel_state;
      ofdm_frame_equalizer1_vcvc_impl(
	  int fft_len,
	  int cp_len,
	  const std::string &len_tag_key,
	  bool propagate_channel_state,
	  const std::vector<std::vector<int> > &occupied_carriers,
  	  const std::vector<std::vector<int> > &pilot_carriers,
  	  const std::vector<std::vector<gr_complex> > &pilot_symbols,
  	  int symbols_skipped,
  	  bool input_is_shifted
      );
      ~ofdm_frame_equalizer1_vcvc_impl();

      int work(int noutput_items,
	  gr_vector_int &ninput_items,
	  gr_vector_const_void_star &input_items,
	  gr_vector_void_star &output_items);
    };

  } // namespace projectCache
} // namespace gr

#endif /* INCLUDED_PROJECTCACHE_OFDM_FRAME_EQUALIZER_VCVC_IMPL_H */

