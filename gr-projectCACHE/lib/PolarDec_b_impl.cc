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
      d_m_files(m_files)
    {
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
        cout << endl << "Hello" << endl;
        cout << "Hello" << endl;
        cout << "Hello" << endl;
        cout << "Hello" << endl;

        for(int k=0; k<4; k++)
            buff_4qpsk[k] = in[i++];
        conv_4QPSKsymb_to_int(buff_4qpsk, header_len);
        field_len = (header_len - 4) / 7;

        // cout << endl << "A" << endl;

        if(DEBUG)
            cout << endl << "Header Length = " << header_len;

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

            if(DEBUG)
                cout << endl << "ID Utenti = " << d_header.id_utenti[j];
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

            if(DEBUG)
                cout << endl << "ID Files = " << d_header.id_files[j];
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

            if(DEBUG)
                cout << endl << "ID Chunks = " << d_header.id_chunks[j];
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

            if(DEBUG)
                cout << endl << "Size Package = " << d_header.size_package[j];
            //find max_payload_len in the header
            /*if(d_header.size_package[j] > d_payload_len)
                d_payload_len = d_header.size_package[j];
            else
                d_payload_len = d_header.size_package[j];*/

        }

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

        //if(DEBUG)
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
      gr_complex buff_qpsk[8];
      gr_complex aa = gr_complex(-SQRT_TWO, -SQRT_TWO);
      bool DEBUG = true;
      int d_case;
      int dummy_bytes;

      // vector<char> decoded_data;



      cout << "\n" << d_k << ", ";
      std::vector<gr::tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0)+d_sSymb,pmt::string_to_symbol("packet_num"));
      //cout << tags.size() << endl;
      //cout << pmt::to_long(tags[0].value) << endl;
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

      snr_avg = (double) num/accumulate(d_size.begin(), d_size.end(),0);
      cout << "avg snr = " << snr_avg << endl;

      /*---------------------------------------------------------------*/


      /*------------------------ Read the received data --------------------------*/
      //read the ID of the header
      for(int j=0; j< 8; j++)
          buff_qpsk[j] = in[j];
      conv_8QPSKsymb_to_int(buff_qpsk, d_id_spack);

      if(DEBUG)
      {
          cout << "\nID header = " << d_id_spack ;
          cout << ", ID header expected = " << d_id_expected << ", ";
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

      //cout << "Case: " << d_case;
      switch(d_case)
      {
          case(0):
              //do nothing
              //Receiver waiting the id_small_packet = 0
              break;


          case(1): //Header received

              decode_header(in);
              d_id_expected += 1;
              break;

          case(2): //Small Packet received
              if(DEBUG)
                cout << endl << "d_small_packet_len = " << d_spack_len << endl;

              if(d_spack_len != (ninput_items[0]/4 - 2))
              {
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
              decoded_data = Process_Data(d_coded_packet,d_id_user,d_packet_remain,d_m_files,d_b_chunks,d_isStr,d_N,d_K_s,d_K_w,snr_avg,PC_w,PC_s,d_header);

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
                  decoded_data = Process_Data(d_coded_packet,d_id_user,d_packet_remain,d_m_files,d_b_chunks,d_isStr,d_N,d_K_s,d_K_w,snr_avg,PC_w,PC_s,d_header);

                  // OTHMANE
                  // Create and Send PDU message with chunks
                  // pmt::pmt_t dict(pmt::make_dict());
                  // string mystr(decoded_data.begin(), decoded_data.end());
                  // cout << endl << "OTHMANE DECODED DATA (Case :"<< d_case << ") :" << endl;
                  // cout << mystr ;
                  // cout << endl << "OTHMANE END " << endl;
                  // dict = pmt::dict_add(dict, pmt::from_long(i), pmt::intern(mystr));
                  // message_port_pub(d_port, dict);


                  // cout << endl << "OTHMANE DECODED DATA :" << endl;
                  // for (int i = 0; i < decoded_data.size(); ++i)
                  //     cout << (int)decoded_data[i] << ", ";
                  // cout << endl << "OTHMANE END :" << endl;

                  // reinitialize(); //Restore the header and the global main variables


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

              decoded_data = Process_Data(d_coded_packet,d_id_user,d_packet_remain,d_m_files,d_b_chunks,d_isStr,d_N,d_K_s,d_K_w,snr_avg,PC_w,PC_s,d_header);

              // string mystr(decoded_data.begin(), decoded_data.end());
              // cout << endl << "OTHMANE DECODED DATA (Case :"<< d_case << ") :" << endl;
              // cout << mystr ;
              // cout << endl << "OTHMANE END " << endl;

              // reinitialize(); //Restore the header and the global main variables

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

      if(d_case == 3 || d_case == 4 || d_case == 5){
        int index = find_index(d_header.id_utenti, d_id_user);
        // cout << endl << "OTHMANE DECODED DATA (Case :"<< d_case << ") : ";
        // cout << index << endl;
        if(index != -1 ) {
            string mystr(decoded_data.begin(), decoded_data.end());
            cout << endl << "OTHMANE RX-BYTES-CODED (Case :"<< d_case << ") :" << endl;
            cout << mystr ;
            cout << endl << "CHUNK ID : ";
            cout << d_header.id_chunks.at(index) ;
            cout << endl << "OTHMANE END " << endl;
        }
        reinitialize();
          // decoded_data.clear();
      }

      //     cnt++;
      // if(cnt>=4)
      //     exit(0);

      if(d_packet_remain <5){
        cout << "Remaining packets: " << d_packet_remain << endl;

      }
      d_k++;

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace projectCACHE */
} /* namespace gr */
