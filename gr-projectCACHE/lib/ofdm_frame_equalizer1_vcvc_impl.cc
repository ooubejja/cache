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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/expj.h>
#include <gnuradio/io_signature.h>
#include "ofdm_frame_equalizer1_vcvc_impl.h"

using namespace std;

#define M_TWOPI (2*M_PI)

static const pmt::pmt_t CARR_OFFSET_KEY = pmt::mp("ofdm_sync_carr_offset");
static const pmt::pmt_t CHAN_TAPS_KEY = pmt::mp("ofdm_sync_chan_taps");

namespace gr {
  namespace projectCACHE {

    ofdm_frame_equalizer1_vcvc::sptr
    ofdm_frame_equalizer1_vcvc::make(int fft_len,	int cp_len,	const std::string &tsb_key,	bool propagate_channel_state,	const vector<vector<int> > &occupied_carriers,const vector<vector<int> > &pilot_carriers, const vector<vector<gr_complex> > &pilot_symbols,int symbols_skipped, bool input_is_shifted)
    {
      return gnuradio::get_initial_sptr (new ofdm_frame_equalizer1_vcvc_impl(fft_len, cp_len, tsb_key, propagate_channel_state, occupied_carriers,pilot_carriers, pilot_symbols,symbols_skipped,input_is_shifted));
    }

    ofdm_frame_equalizer1_vcvc_impl::ofdm_frame_equalizer1_vcvc_impl(
	int fft_len,
	int cp_len,
	const std::string &tsb_key,
	bool propagate_channel_state,
	const vector<vector<int> > &occupied_carriers,
  const vector<vector<int> > &pilot_carriers,
  const vector<vector<gr_complex> > &pilot_symbols,
  int symbols_skipped,
  bool input_is_shifted
  ): tagged_stream_block("ofdm_frame_equalizer1_vcvc",
	  io_signature::make(1, 1, sizeof (gr_complex) * fft_len),
	  io_signature::make(1, 1, sizeof (gr_complex) * fft_len),
	  tsb_key),
      d_fft_len(fft_len),
      d_cp_len(cp_len),
      d_propagate_channel_state(propagate_channel_state),
      d_channel_state(fft_len, gr_complex(1, 0)),
      d_occupied_carriers(fft_len, false),
      d_pilot_carriers(pilot_carriers.size(), std::vector<bool>(fft_len, false)),
      d_pilot_symbols(pilot_symbols.size(), std::vector<gr_complex>(fft_len, gr_complex(0, 0))),
      d_symbols_skipped(symbols_skipped),
      d_pilot_carr_set(pilot_carriers.empty() ? 0 : symbols_skipped % pilot_carriers.size())
    {
      /*******************Initialization of equalizers arguments*******************/
      int fft_shift_width = 0;
      if (input_is_shifted) {
        fft_shift_width = fft_len/2;
      }
      if (!occupied_carriers.size()) {
        std::fill(d_occupied_carriers.begin(), d_occupied_carriers.end(), true);
      } else {
        for (unsigned i = 0; i < occupied_carriers.size(); i++) {
          for (unsigned k = 0; k < occupied_carriers[i].size(); k++) {
            int carr_index = occupied_carriers[i][k];
            if (occupied_carriers[i][k] < 0) {
              carr_index += fft_len;
            }
            if (carr_index >= fft_len || carr_index < 0) {
              throw std::invalid_argument("data carrier index out of bounds.");
            }
            d_occupied_carriers[(carr_index + fft_shift_width) % fft_len] = true;
          }
        }
      }
      if (pilot_carriers.size()) {
        for (unsigned i = 0; i < pilot_carriers.size(); i++) {
          if (pilot_carriers[i].size() != pilot_symbols[i].size()) {
            throw std::invalid_argument("pilot carriers and -symbols do not match.");
          }
          for (unsigned k = 0; k < pilot_carriers[i].size(); k++) {
            int carr_index = pilot_carriers[i][k];
            if (pilot_carriers[i][k] < 0) {
              carr_index += fft_len;
            }
            if (carr_index >= fft_len || carr_index < 0) {
              throw std::invalid_argument("pilot carrier index out of bounds.");
            }
            d_pilot_carriers[i][(carr_index + fft_shift_width) % fft_len] = true;
            d_pilot_symbols[i][(carr_index + fft_shift_width) % fft_len] = pilot_symbols[i][k];
          }
        }
      }

      /*******************Initialization of equalizers arguments*******************/
      if (tsb_key.empty()) {
        throw std::invalid_argument("Specify a TSB tag!");
      }
      set_relative_rate(1.0);
      // Really, we have TPP_ONE_TO_ONE, but the channel state is not propagated
      set_tag_propagation_policy(TPP_DONT);
    }

    ofdm_frame_equalizer1_vcvc_impl::~ofdm_frame_equalizer1_vcvc_impl()
    {
    }

    void
    ofdm_frame_equalizer1_vcvc_impl::parse_length_tags(const vector<vector<tag_t> > &tags, gr_vector_int &n_input_items_reqd) {
        for (unsigned k = 0; k < tags[0].size(); k++) {
          if (tags[0][k].key == pmt::string_to_symbol(d_length_tag_key_str)) {
            n_input_items_reqd[0] = pmt::to_long(tags[0][k].value);
          }
        }

    }


    int
    ofdm_frame_equalizer1_vcvc_impl::work(int noutput_items,
	  gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      int carrier_offset = 0;
      int frame_len = 0;

	    frame_len = ninput_items[0];

      std::vector<tag_t> tags;
      get_tags_in_window(tags, 0, 0, 1);
      for (unsigned i = 0; i < tags.size(); i++) {
        if (pmt::symbol_to_string(tags[i].key) == "ofdm_sync_chan_taps") {
          d_channel_state = pmt::c32vector_elements(tags[i].value);
        }
        if (pmt::symbol_to_string(tags[i].key) == "ofdm_sync_carr_offset") {
          carrier_offset = pmt::to_long(tags[i].value);
        }
      }

      // Copy the frame and the channel state vector such that the symbols are shifted to the correct position
      if (carrier_offset < 0) {
      	memset((void *) out, 0x00, sizeof(gr_complex) * (-carrier_offset));
      	memcpy(
      	    (void *) &out[-carrier_offset], (void *) in,
      	    sizeof(gr_complex) * (d_fft_len * frame_len + carrier_offset)
      	);
      } else {
      	memset((void *) (out + d_fft_len * frame_len - carrier_offset), 0x00, sizeof(gr_complex) * carrier_offset);
      	memcpy(
      	    (void *) out, (void *) (in+carrier_offset),
      	    sizeof(gr_complex) * (d_fft_len * frame_len - carrier_offset)
      	);
      }

      // Correct the frequency shift on the symbols
      gr_complex phase_correction;
      for (int i = 0; i < frame_len; i++) {
      	phase_correction = gr_expj(-M_TWOPI * carrier_offset * d_cp_len / d_fft_len * (i+1));
      	for (int k = 0; k < d_fft_len; k++) {
      	  out[i*d_fft_len+k] *= phase_correction;
      	}
      }

      // Do the equalizing
      //d_eq->reset();
      //d_eq->equalize(out, frame_len, d_channel_state);
      //d_eq->get_channel_state(d_channel_state);
      for (int i = 0; i < frame_len; i++) {
        for (int k = 0; k < d_fft_len; k++) {
          if (!d_occupied_carriers[k]) {
            continue;
          }
          if (!d_pilot_carriers.empty() && d_pilot_carriers[d_pilot_carr_set][k]) {
            d_channel_state[k] = out[i*d_fft_len + k] / d_pilot_symbols[d_pilot_carr_set][k];
            out[i*d_fft_len+k] = d_pilot_symbols[d_pilot_carr_set][k];
          } else {
            out[i*d_fft_len+k] /= d_channel_state[k];
          }
        }
        if (!d_pilot_carriers.empty()) {
          d_pilot_carr_set = (d_pilot_carr_set + 1) % d_pilot_carriers.size();
        }
      }

      // Update the channel state regarding the frequency offset
      phase_correction = gr_expj(M_TWOPI * carrier_offset * d_cp_len / d_fft_len * frame_len);
      for (int k = 0; k < d_fft_len; k++) {
        d_channel_state[k] *= phase_correction;
      }

      // Propagate tags (except for the channel state and the TSB tag)
      get_tags_in_window(tags, 0, 0, frame_len);
      for (size_t i = 0; i < tags.size(); i++) {
        if (tags[i].key != CHAN_TAPS_KEY
            && tags[i].key != pmt::mp(d_length_tag_key_str)) {
          add_item_tag(0, tags[i]);
        }
      }

      // Housekeeping
      if (d_propagate_channel_state) {
      	add_item_tag(0, nitems_written(0),
      	    pmt::string_to_symbol("ofdm_sync_chan_taps"),
      	    pmt::init_c32vector(d_fft_len, d_channel_state));
      }

      return frame_len;
    }

  } /* namespace projectCACHE */
} /* namespace gr */

