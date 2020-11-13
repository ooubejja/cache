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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "PolarDec_b_impl.h"
#include <iostream>
#include <cmath>
#include <time.h>
#include <random>

// #include <experimental/filesystem>
// namespace fs = std::experimental::filesystem::v1;

#define SQRT_TWO 0.707107

using namespace caching;
namespace gr {
  namespace projectCACHE {

    PolarDec_b::sptr
    PolarDec_b::make(const int N, const int K_w, const int K_s, int m_files, int  b_chunks, int id_user, int n_users, unsigned int spack_len,const std::string &lengthtagname)
    {
      return gnuradio::get_initial_sptr(new PolarDec_b_impl(N, K_w, K_s, m_files, b_chunks, id_user, n_users, spack_len,lengthtagname));
    }

    /*
     * The private constructor
     */
    PolarDec_b_impl::PolarDec_b_impl(const int N, const int K_w, const int K_s, int m_files, int  b_chunks, int id_user, int n_users, unsigned int spack_len, const std::string &lengthtagname)
      : gr::tagged_stream_block("PolarDec_b",
                            gr::io_signature::make(1, 1, sizeof(gr_complex)), //float
                            gr::io_signature::make(0, 0, 0), lengthtagname),
      d_N(N),
      d_K_w(K_w),
      d_K_s(K_s),
      d_b_chunks(b_chunks),
      d_id_user(id_user),
      d_n_users(n_users),
      d_spack_len(spack_len),
      d_m_files(m_files),
      ch_use_port(pmt::mp("CH_USE"))
      // cw_port(pmt::mp("RX_CW"))
    {
      message_port_register_out(ch_use_port);
      // message_port_register_out(msg_port);
      // message_port_register_out(cw_port);
      d_stop = false;
      designSNRdb = 0;
      d_k=0;
      d_id_expected=0;
      // d_crc = true;
      d_crc = false;

      d_y1 = 0;
      d_y2 = 0;
      d_alpha = 0.001;
      d_beta = 1.0- d_alpha;

      if(d_id_user < d_n_users)
            d_packet_remain = caching::readCacheInfo(d_id_user, 0);
      else
            d_packet_remain = d_b_chunks;

      d_isStr = (id_user >= d_n_users)? true:false;

      PC_w.constructPC(d_N, d_K_w, designSNRdb);

      PC_s.initPC(d_N, d_K_s, designSNRdb);
      PC_s.setGenMatrix(PC_w.genMatrix);
      PC_s.setRn(PC_w.Rn);
      PC_s.setArrangedBits(PC_w.arragedBits);

      string pathFolder = "../trasmissioni/User_" + my_to_string(d_id_user); //"/CachingFile/trasmissioni/User_"
      mkdir(pathFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

      string cacheFolder = "../cache/UserCache/user_" + my_to_string(d_id_user);//"/CachingFile/cache/UserCache/user_"
      mkdir(cacheFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);


      cnt = 0;
      // othmane : add verification of previously existing cacheFolder

      // string str = "exec rm " + cacheFolder + "/*";
      // const char *command = str.c_str();
      // if(system(command)){
      //   cout << endl << endl << "ERROR: Cache Folder not emptied " << endl;
      //   exit(0);
      // }

    }

    /*
     * Our virtual destructor.
     */
    PolarDec_b_impl::~PolarDec_b_impl()
    {
    }

    int
    PolarDec_b_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = 0;
      return noutput_items ;
    }

    int
    PolarDec_b_impl::update(int noutput_items, const gr_complex *input)
    {
      for(int i = 0; i < noutput_items; i++) {
        double x = abs(input[i+1]);
        double x1 = abs(input[i]);
        double y1 = (x*x)*(x1*x1);
        d_y1 = d_alpha*y1 + d_beta*d_y1;

        double y2 = x*x*x*x;
        d_y2 = d_alpha*y2 + d_beta*d_y2;
      }
      return noutput_items;
    }

    double
    PolarDec_b_impl::snr()
    {
      double x = d_y1 / (d_y2 - d_y1);
      return 10.0*log10(x - 1 + sqrt(x*(x-1)));
    }

    int PolarDec_b_impl::decode_header(const gr_complex *in)
    {
        unsigned int i = 8; // first 2 bytes for header id
        unsigned int d_id_demand;
        unsigned int header_len = 0, field_len;
        char byte;
        char buff[4];
        gr_complex buff_4qpsk[4];
        bool DEBUG = false;


        /*****STUPID TEST ONLY****/
        // cout << endl << "Hello" << endl;
        // cout << "Hello" << endl;
        // cout << "Hello" << endl;
        // cout << "Hello" << endl;

        for(int k=0; k<4; k++)
            buff_4qpsk[k] = in[i++];
        conv_4QPSKsymb_to_int(buff_4qpsk, header_len);

        // header_len--; // To ignore CRC byte in next computations

        field_len = (header_len - 4 - 1) / 7;

        if(DEBUG)
            cout << endl << "Header Length = " << header_len;

        // // /*--------------------------------- Check CRC --------------------------------------------------*/
        // // OTHMANE : CRC processes
        // // Header starts with a 4QPSK=1byte=8bits CRC linked to the next 12QPSK=3bytes=24bits
        // // --> total for CRC check 32 bits = 16 QPSK Symbols
        // // |   CRC  |  pkt_id | hdl_len|
        // // | 1-byte | 2-bytes | 1-byte |
        // // | 4-symb |  8-symb | 4-symb |
        // unsigned int crc_symb_len = 4; // first 4 QPSK symbols (8bits) are for CRC
        // char crc;
        // int k = 0;
        // gr_complex buff_qpsk[16];
        // vector<char> hdr_plusCRC;
        //
        // // in -= 4 ; // remove offset to include CRC
        //
        // for(int t=0; t<4; t++){
        //   for(int j=0; j<4; j++)
        //     buff_qpsk[j] = in[k++];
        //   conv_4QPSKsymb_to_char(buff_qpsk, crc);
        //   hdr_plusCRC.push_back(crc);
        //   cout << endl << "CRC INPUT : " << int(crc) ;
        // }
        // // rotate(hdr_plusCRC.begin(), hdr_plusCRC.begin()+1, hdr_plusCRC.end());  // Rotate 1 to the left to queue CRC
        // crc = compute_CRC8(hdr_plusCRC);
        // cout << endl << "CRC CHECK : " << int(crc) << endl;
        //
        // // get rid of CRC symbols by pointer offset
        // i += 4;
        // // /*----------------------------------------------------------------------------------------------*/
        //


        //read id_demands
        /*byte=in[i++];
        conv_byte_to_int(byte, d_id_demand);*/
        for(int k=0; k<4; k++)
            buff_4qpsk[k] = in[i++];
        conv_4QPSKsymb_to_int(buff_4qpsk, d_id_demand);

        if(DEBUG)
            cout << endl << "ID Demand = " << d_id_demand;

        //read header.id_utenti
        for(unsigned int j=0; j<field_len; j++)
        {
            d_header.id_utenti.push_back(0);
            /*byte=in[i++;];
            conv_byte_to_int(byte, d_header.id_utenti[j]);*/
            for(int k=0; k<4; k++)
                buff_4qpsk[k] = in[i++];
            conv_4QPSKsymb_to_int(buff_4qpsk, d_header.id_utenti[j]);

            // if(DEBUG)
            //     cout << endl << "ID Utenti = " << d_header.id_utenti[j];
        }

        //read header.id_files
        for(unsigned int j=0; j<field_len; j++)
        {
            d_header.id_files.push_back(0);
            /*byte=in[i++];
            conv_byte_to_int(byte, d_header.id_files[j]);*/
            for(int k=0; k<4; k++)
                buff_4qpsk[k] = in[i++];
            conv_4QPSKsymb_to_int(buff_4qpsk, d_header.id_files[j]);

            // if(DEBUG)
            //     cout << endl << "ID Files = " << d_header.id_files[j];
        }


        //read header.id_chunks
        for(unsigned int j=0; j<field_len; j++)
        {
            d_header.id_chunks.push_back(0);
            /*byte=in[i++];
            conv_byte_to_int(byte, d_header.id_chunks[j]);*/
            for(int k=0; k<4; k++)
                buff_4qpsk[k] = in[i++];
            conv_4QPSKsymb_to_int(buff_4qpsk, d_header.id_chunks[j]);

            // if(DEBUG)
            //     cout << endl << "ID Chunks = " << d_header.id_chunks[j];
        }


        //read header.size_package
        d_payload_len = 0;
        for(unsigned int j=0; j<field_len; j++)
        {
            d_header.size_package.push_back(0);

            for(int k=0; k<4; k++)
            {
                //buff[k]=in[i++];
                for(int l=0; l<4; l++)
                  buff_4qpsk[l] = in[i++];
                conv_4QPSKsymb_to_char(buff_4qpsk, buff[k]);
            }
            conv_char_to_int(buff, d_header.size_package[j]);

            // if(DEBUG)
            //     cout << endl << "Size Package = " << d_header.size_package[j];
            //find max_payload_len in the header
            /*if(d_header.size_package[j] > d_payload_len)
                d_payload_len = d_header.size_package[j];
            else
                d_payload_len = d_header.size_package[j];*/

        }


        // /*--------------------------------- Check CRC --------------------------------------------------*/
        // OTHMANE : CRC processes
        // unsigned int crc_symb_len = 2*(5+7*field_len); // symbols for CRC
        unsigned int crc_symb_len = header_len; // symbols for CRC
        char crc;
        int k = 0;
        gr_complex buff_qpsk[4];
        vector<char> hdr_plusCRC;

        for(int t=0; t<crc_symb_len; t++){
          copy(in+k, in+k+3, buff_qpsk);
          k+=4;
          conv_4QPSKsymb_to_char(buff_qpsk, crc);
          hdr_plusCRC.push_back(crc);
          // cout << endl << "CRC INPUT : " << int(crc) ;
        }
        crc = compute_CRC8(hdr_plusCRC);
        // cout << endl << "CRC CHECK : " << int(crc) << endl;

        // get rid of CRC symbols by pointer offset
        i += 4;
        // /*----------------------------------------------------------------------------------------------*/


        //Read if there strong and weak, only weak, or two strog packets
        for(int l=0; l<4; l++)
            buff_4qpsk[l] = in[i++];
        conv_4QPSKsymb_to_char(buff_4qpsk, byte);
        if(DEBUG)
            cout << "Strong user? " << (unsigned int) byte <<endl;

        if(byte == 0x01)
            d_header.strong = true;
        else
            d_header.strong = false;

        //byte = in[i++];
        for(int l=0; l<4; l++)
            buff_4qpsk[l] = in[i++];
        conv_4QPSKsymb_to_char(buff_4qpsk, byte);
        if(DEBUG)
            cout << "Weak user? " << (unsigned int) byte << endl;

        if(byte == 0x01)
            d_header.weak = true;
        else
            d_header.weak = false;


        //compute total number of small packet
        d_payload_len = d_N/8;

        if(d_crc){
          d_nb_spack = (d_payload_len /(d_spack_len-4))  + 1; //+1 for the header small packet;
          //compute last small packet length
          d_last_spack_len = d_payload_len % (d_spack_len-4);
        } else{
          d_nb_spack = (d_payload_len / d_spack_len) + 1; //+1 for the header small packet;
          //compute last small packet length
          d_last_spack_len = d_payload_len % (d_spack_len);
        }

        if(d_last_spack_len > 0)
        {
            d_nb_spack ++;
            d_id_last_spack = d_nb_spack - 1;
        }else
        {
            d_id_last_spack = d_nb_spack - 1;
            d_last_spack_len = d_spack_len;
        }

        // check if number of small packet is minus then 65535
        if (d_nb_spack > 65535)
        {
            cout << endl << endl << "ERROR: number_small_packet > 65535 " << endl;
            exit(0);
        }
        else if(d_nb_spack == 65535 && d_last_spack_len != 0)
        {
            cout << endl << endl << "ERROR: number_small_packet = 65535  AND  last_small_packet_len != 0" << endl;
            exit(0);
        }

        if(DEBUG)
        {
            cout << endl << " number_small_packet = " << d_nb_spack;
            cout << endl << " id_last_small_packet_len = " << d_id_last_spack;
            cout << endl << " last_small_packet_len = " << d_last_spack_len;
        }

        return i;

    }

    void PolarDec_b_impl::reinitialize()
    {
      d_header.id_chunks.clear();
      d_header.id_files.clear();
      d_header.id_utenti.clear();
      d_header.size_package.clear();
      d_id_expected = 0;
      d_payload_len = 0;
      d_nb_spack = 0;
      d_last_spack_len = 0;
      d_id_last_spack = 0;
      // OTHMANE
      d_coded_packet.clear();
      decoded_data.clear();

    }

    int
    PolarDec_b_impl::work(int noutput_items,
                        gr_vector_int &ninput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items)
    {
      //const float *in = (const float *) input_items[0];
      const gr_complex *in = (const gr_complex *) input_items[0];

      int d_sSymb = ninput_items[0];
      gr_complex v[d_spack_len];
      double num;
      double snr_avg;
      vector<int> vt(d_N, 0);
      gr_complex buff_qpsk[16];
      gr_complex aa = gr_complex(-SQRT_TWO, -SQRT_TWO);
      bool DEBUG = true;
      int d_case;
      int dummy_bytes;


      cout << "\n" << d_k << ", ";
      cout << "Size: " << d_sSymb << ", ";

      /*-------------------------SNR Estimation------------------------*/
      update(d_sSymb, in);
      double SNR_tmp = snr();
      cout << "SNR = " << SNR_tmp << ", ";

      d_snr.push_back(SNR_tmp);
      d_size.push_back(d_sSymb);

      num=0;
      for (int i = 0; i < d_snr.size(); ++i)
        num += d_snr[i]*d_size[i];

      // snr_avg = (double) num/accumulate(d_size.begin(), d_size.end(),0);
      snr_avg = 10;
      cout << "avg snr = " << snr_avg << endl;

      /*---------------------------------------------------------------*/

      /*------------------------ Read the received data --------------------------*/
      // in += 4 ; // offset to omit CRC
      //read the ID of the header
      // for(int j=0+4; j< 8+4; j++)
      for(int j=0; j< 8; j++)
        buff_qpsk[j] = in[j];
      conv_8QPSKsymb_to_int(buff_qpsk, d_id_spack);

      if(DEBUG)
      {
          cout << "\nID S_PACKET = " << d_id_spack ;
          cout << ", ID S_PACKET expected = " << d_id_expected << ", ";
      }

      //Check the state of the received packets
      if(d_id_spack != d_id_expected)
      {
          if(d_id_expected == 0) //Header not received
              d_case = 0;
          else if(d_id_expected > 0)
          {
              if(d_id_spack > (d_nb_spack + (d_last_spack_len)?1:0))
                d_case = 0;
              else if(d_id_spack > d_id_expected) //Some small packet of the Large Packet are not received
                  d_case = 4;
              else if(d_id_spack < d_id_expected) //The end of a Large Packet is not received
                  d_case = 5;
          }

      }else if(d_id_spack == d_id_expected)
      {
          if(d_id_expected == 0) //The header is received
              d_case = 1;
          else if(d_id_expected == d_id_last_spack) //If the last small packet is received
              d_case = 3;
          else
              d_case = 2;
      }

      string repo_file, name_file;
      int recCodeword_s[d_N], recMessage_s[d_N];
      vector<unsigned int> cw_raw(d_N);

      //cout << "Case: " << d_case;
      switch(d_case)
      {
          case(0):
              //do nothing
              //Receiver waiting the id_small_packet = 0
              break;


          case(1): //Header received

              decode_header(in);
              // decode_header(in_crc);
              d_id_expected += 1;
              break;

          case(2): //Small Packet received
              if(DEBUG)
                cout << endl << "d_small_packet_len = " << d_spack_len << endl;

              if(d_spack_len != (ninput_items[0]/4 - 2))
              {
                cout << endl << "ERROR: small_packet_len != input_packet_len" << endl;
                cout << endl << "ERROR: small_packet_len != input_packet_len" << endl;
                  exit(0);
              }
              for(unsigned int k = 8; k < d_sSymb; k++)
                  d_coded_packet.push_back(in[k]);

              d_id_expected += 1;
              break;


          case(3): //Last Small Packet received
              if(DEBUG)
                cout << endl << "d_last_small_packet_len = " << d_last_spack_len << endl;

              if(d_last_spack_len != (ninput_items[0]/4 - 2))
              {
                  cout << endl << "ERROR: last_small_packet_len != input_packet_len" << endl;
                  exit(0);
              }

              for(unsigned int k = 8; k < d_sSymb; k++)
                  d_coded_packet.push_back(in[k]);

              //Proceed to decoding of the whole packet
              decoded_data = Process_Data(d_coded_packet,d_id_user,d_packet_remain,d_m_files,d_b_chunks,d_isStr,d_N,d_K_s,d_K_w,snr_avg,PC_w,PC_s,d_header, recCodeword_s, recMessage_s, cw_raw);

              // reinitialize(); //Restore the header and the global main variables

              break;


          case(4): //Fill part of large packet with dummy bytes

              dummy_bytes = (d_id_spack - d_id_expected ) * d_sSymb;

              for(unsigned int k = 8; k < dummy_bytes; k++)
                  d_coded_packet.push_back(aa);

              if(d_id_spack == d_id_last_spack) //If receive the last packet
              {
                  if(DEBUG)
                      cout << endl << "d_last_small_packet_len = " << d_id_last_spack << endl;

                  if(d_last_spack_len != (ninput_items[0]/4 - 2))
                  {
                      cout << endl << "ERROR: last_small_packet_len != input_packet_len" << endl;
                      exit(0);
                  }

                  for(unsigned int k = 8; k < d_sSymb; k++)
                    d_coded_packet.push_back(in[k]);

                  //Proceed to decoding of the whole packet
                  decoded_data = Process_Data(d_coded_packet,d_id_user,d_packet_remain,d_m_files,d_b_chunks,d_isStr,d_N,d_K_s,d_K_w,snr_avg,PC_w,PC_s,d_header,recCodeword_s, recMessage_s, cw_raw);

              }else if(d_id_spack != d_id_last_spack) //If receive a generic small packet
              {
                  if(DEBUG)
                      cout << endl << "d_small_packet_len = " << d_spack_len << endl;

                  if(d_spack_len != (ninput_items[0]/4 - 2))
                  {
                      cout << endl << "ERROR: small_packet_len != input_packet_len" << endl;
                      exit(0);
                  }

                  for(unsigned int k = 8; k < d_sSymb; k++)
                    d_coded_packet.push_back(in[k]);

                  d_id_expected = d_id_spack + 1;
              }

              break;

          case(5): //Lost the end of the previous large pack, so fill the lack and decode. After check for the next large packet

              dummy_bytes = (d_id_last_spack - d_id_expected)*d_spack_len*4 + (d_last_spack_len)*4;

              for(unsigned int k = 0; k < dummy_bytes; k++)
                  d_coded_packet.push_back(aa);

              decoded_data = Process_Data(d_coded_packet,d_id_user,d_packet_remain,d_m_files,d_b_chunks,d_isStr,d_N,d_K_s,d_K_w,snr_avg,PC_w,PC_s,d_header,recCodeword_s,recMessage_s, cw_raw);

              if(d_id_spack == 0)
              {
                  decode_header(in);

                  d_id_expected += 1;

              }else if(d_id_spack != 0)
              {
                  //do nothing, Receiver waiting the id_small_packet = 0
              }
              break;
      }

      if(d_case >= 3){
        // cw_raw.clear();
        reinitialize();
      }



      std::vector<uint8_t> vec_dict_ch_use;
      string str_msg = to_string(d_k);
      // cout << "AZAZ: " << str_msg << endl;
      vec_dict_ch_use.assign(str_msg.begin(), str_msg.end());

      pmt::pmt_t TEST_msg = pmt::init_u8vector(vec_dict_ch_use.size(), vec_dict_ch_use);
      pmt::pmt_t dict_msg(pmt::make_dict());

      dict_msg = pmt::cons(pmt::make_dict(), TEST_msg);

      message_port_pub(ch_use_port, dict_msg);

      vec_dict_ch_use.clear();



      if(d_packet_remain <5){
        cout << "Remaining packets: " << d_packet_remain << endl;
        if(d_packet_remain==0)
          return -1;
      }
      d_k++;



      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace projectCACHE */
} /* namespace gr */





      // if(d_case == 3 || d_case == 4 || d_case == 5){
      // // if(d_case == 3 || d_case == 4 ){
      //   int index = find_index(d_header.id_utenti, d_id_user);
      //   if(index != -1 ) {
          // pmt::pmt_t dict_msg(pmt::make_dict());
          // pmt::pmt_t dict_cw(pmt::make_dict());
          // stringstream str_msg, str_cw;
          // int strg_ind = d_header.id_chunks.at(index);

          // Fill Rx CWs and Msgs in the same loop to optimize runtime
          // for (int i=0; i<d_N; i++){
          //   // str_cw << recCodeword_s[i] ;
          //   str_cw << cw_raw[i] ;
          //   // if(i<d_K_s)
          //   //   str_msg << recMessage_s[i] ;
          //   if (recCodeword_s[i]!=0 && recCodeword_s[i]!=1)   // Bug : In PC.cpp, recCodeword is sometimes different than 0 or 1
          //     str_msg << "X" ;
          //   else
          //     str_msg << recCodeword_s[i] ;
          // }

          ///////////////////////////////////////////////////////////////
          /***************************************************************************/
          // for (int i = 0; i < d_K_s; i++){
          //   str_msg << recMessage_s[i];
          // }
          // for (int i = 0; i < d_N; i++){
          //   str_cw << cw_raw[i] ;
          //
          //   if (d_isStr && i<d_K_s)
          //     str_msg << recMessage_s[i];
          //   else if (!d_isStr && i<d_K_w)
          //     str_msg << recMessage_s[i];
          // }
          /***************************************************************************/
          /***************************************************************************/
          // ///////////////////////////////////////////////////////////////
          // ofstream debug_file_coded;
          // string file_name_debug = "../trasmissioni/debug_file_coded";
          // file_name_debug = file_name_debug.append(to_string(d_id_user));
          //
          // debug_file_coded.open(file_name_debug,ios::app);
          //
          // /***************************************************************************/
          // debug_file_coded << endl << "RECEIVED MESSAGE : " << d_header.id_chunks.at(index) << endl;
          // for (int i = 0; i < d_K_s; i++){
          //   debug_file_coded << recMessage_s[i] << "";
          //   str_msg << recMessage_s[i];
          // }
          // /***************************************************************************/
          // debug_file_coded << endl << "----------------------------" << endl ;
          // debug_file_coded << endl << "RECEIVED CW : " << d_header.id_chunks.at(index) << endl;
          // for (int i = 0; i < d_N; i++){
          //   // Correcting a bug where some walues are corrupt :
          //   // It is a bug in PC.cpp, recCodeword is sometimes different than 0 or 1
          //   str_cw << cw_raw[i] ;
          //   if (recCodeword_s[i]!=0 && recCodeword_s[i]!=1){
          //     debug_file_coded << "X" ;
          //     // str_msg << "X" ;
          //   }
          //   else{
          //     debug_file_coded << recCodeword_s[i];
          //     // str_msg << recCodeword_s[i];
          //   }
          // }
          // /***************************************************************************/
          // debug_file_coded << endl << "----------------------------" << endl ;
          // debug_file_coded << endl << "RECEIVED SYMBOLS : " << d_header.id_chunks.at(index) << endl;
          // for (int i = 0; i < d_N; i++){
          //   debug_file_coded << cw_raw[i];
          // }
          // debug_file_coded << endl << "----------------------------" << endl ;
          // debug_file_coded.close();
          // /***************************************************************************/


          // string str_msg_str = str_msg.str();
          // string str_cw_str = str_cw.str();
          //
          // std::vector<uint8_t> vec_dict_msg, vec_dict_cw;
          //
          // vec_dict_msg.assign(str_msg_str.begin(), str_msg_str.end());
          // vec_dict_cw.assign(str_cw_str.begin(), str_cw_str.end());
          //
          // vec_dict_msg.insert(vec_dict_msg.begin(), strg_ind);
          // vec_dict_cw.insert(vec_dict_cw.begin(), strg_ind);
          //
          // pmt::pmt_t TEST_msg = pmt::init_u8vector(vec_dict_msg.size(), vec_dict_msg);
          // pmt::pmt_t TEST_cw = pmt::init_u8vector(vec_dict_cw.size(), vec_dict_cw);
          // // cout << endl << "BBB" << endl;
          // // cout << endl << "OTHMANE :" << strg_ind << endl;
          //
          // dict_msg = pmt::cons(pmt::make_dict(), TEST_msg);
          // dict_cw = pmt::cons(pmt::make_dict(), TEST_cw);
          //
          // message_port_pub(msg_port, dict_msg);
          // message_port_pub(cw_port, dict_cw);

          // intrusive_ptr_release(dict_msg);
          // intrusive_ptr_release(dict_cw);
          // cout << endl << "CCC" << endl;
          //
          // vec_dict_msg.clear();
          // vec_dict_cw.clear();

        // }

        // cw_raw.clear();
        // reinitialize();
      // }
