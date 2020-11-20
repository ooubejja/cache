
/*
 * This file was automatically generated using swig_doc.py.
 *
 * Any changes to it will be lost next time it is regenerated.
 */




%feature("docstring") is_complex "

Params: (x)"

%feature("docstring") gr::projectCACHE::crc32_1_bb "Byte-stream CRC block.

Input: stream of bytes, which form a packet. The first byte of the packet has a tag with key \"length\" and the value being the number of bytes in the packet.

Output: The same bytes as incoming, but trailing a CRC32 of the packet. The tag is re-set to the new length."

%feature("docstring") gr::projectCACHE::crc32_1_bb::make "

Params: (check, lengthtagname)"

%feature("docstring") gr::projectCACHE::gain_bf "<+description of block+>"

%feature("docstring") gr::projectCACHE::gain_bf::make "Return a shared_ptr to a new instance of projectCACHE::gain_bf.

To avoid accidental use of raw pointers, projectCACHE::gain_bf's constructor is in a private implementation class. projectCACHE::gain_bf::make is the public interface for creating new instances.

Params: (L_demands, small_packet_len, len_tag_key, DEBUG)"

%feature("docstring") gr::projectCACHE::map_header_payload_bc "<+description of block+>"

%feature("docstring") gr::projectCACHE::map_header_payload_bc::make "Return a shared_ptr to a new instance of projectCACHE::map_header_payload_bc.

To avoid accidental use of raw pointers, projectCACHE::map_header_payload_bc's constructor is in a private implementation class. projectCACHE::map_header_payload_bc::make is the public interface for creating new instances.

Params: (, , lengthtagname)"

%feature("docstring") gr::projectCACHE::ofdm_frame_equalizer1_vcvc "OFDM frame equalizer.

Performs equalization in one or two dimensions on a tagged OFDM frame.

This does two things: First, it removes the coarse carrier offset. If a tag is found on the first item with the key 'ofdm_sync_carr_offset', this is interpreted as the coarse frequency offset in number of carriers. Next, it performs equalization in one or two dimensions on a tagged OFDM frame. The actual equalization is done by a ofdm_frame_equalizer object, outside of the block.

Note that the tag with the coarse carrier offset is not removed. Blocks downstream from this block must not attempt to also correct this offset.

Input: a tagged series of OFDM symbols. Output: The same as the input, but equalized and frequency-corrected."

%feature("docstring") gr::projectCACHE::ofdm_frame_equalizer1_vcvc::make "

Params: (fft_len, cp_len, tsb_key, propagate_channel_state, occupied_carriers, pilot_carriers, pilot_symbols, symbols_skipped, input_is_shifted)"

%feature("docstring") gr::projectCACHE::PolarDec_b "<+description of block+>"

%feature("docstring") gr::projectCACHE::PolarDec_b::make "Return a shared_ptr to a new instance of projectCACHE::PolarDec_b.

To avoid accidental use of raw pointers, projectCACHE::PolarDec_b's constructor is in a private implementation class. projectCACHE::PolarDec_b::make is the public interface for creating new instances.

Params: (N, K_w, K_s, SNR_w, SNR_s, lengthtagname)"

%feature("docstring") gr::projectCACHE::polarEnc_b "<+description of block+>"

%feature("docstring") gr::projectCACHE::polarEnc_b::make "Return a shared_ptr to a new instance of projectCACHE::polarEnc_b.

To avoid accidental use of raw pointers, projectCACHE::polarEnc_b's constructor is in a private implementation class. projectCACHE::polarEnc_b::make is the public interface for creating new instances.

Params: (N, K_w, K_s, SNR_w, SNR_s)"

%feature("docstring") gr::projectCACHE::Rx_1P_b "<+description of block+>"

%feature("docstring") gr::projectCACHE::Rx_1P_b::make "Return a shared_ptr to a new instance of projectCACHE::Rx_1P_b.

To avoid accidental use of raw pointers, projectCACHE::Rx_1P_b's constructor is in a private implementation class. projectCACHE::Rx_1P_b::make is the public interface for creating new instances.

Params: (id_utente, n_utenti, m_files, b_chunks, L_demands, small_packet_len, len_tag_key, DEBUG)"

%feature("docstring") gr::projectCACHE::Tx_1P_b "<+description of block+>"

%feature("docstring") gr::projectCACHE::Tx_1P_b::make "Return a shared_ptr to a new instance of projectCACHE::Tx_1P_b.

To avoid accidental use of raw pointers, projectCACHE::Tx_1P_b's constructor is in a private implementation class. projectCACHE::Tx_1P_b::make is the public interface for creating new instances.

Params: (GRASP, n_utenti, m_files, b_chunks, L_demands, small_packet_len, len_tag_key)"