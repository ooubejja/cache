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
#include "polarEnc_b_impl.h"
#include <iostream>
#include <cmath>
#include <time.h>
#include <random>

using namespace caching;

namespace gr {
  namespace projectCACHE {

    polarEnc_b::sptr
    polarEnc_b::make(const int N, const int K_w, const int K_s, int m_files, int b_chunks, int nb_strg, int id_user, int spack_len, const std::string &len_tag_key)//
    {
      return gnuradio::get_initial_sptr
        (new polarEnc_b_impl(N, K_w, K_s, m_files, b_chunks, nb_strg, id_user, spack_len, len_tag_key));
    }

    /*
     * The private constructor
     */
    polarEnc_b_impl::polarEnc_b_impl(const int N, const int K_w, const int K_s, int m_files, int b_chunks, int nb_strg, int id_user, int spack_len, const std::string &len_tag_key)
      : gr::sync_block("polarEnc_b",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
      d_N(N),
      d_K_w(K_w),
      d_K_s(K_s),
      d_m_files(m_files),
      d_b_chunks(b_chunks),
      d_nb_strg(nb_strg),
      d_id_user(id_user),
      d_offset(0),
      d_spack_len(spack_len),
      d_len_tag_key(pmt::string_to_symbol(len_tag_key)),
      ber_info(pmt::mp("BER_INFO")),
      msg_port(pmt::mp("TX_MSG")),
      cw_port(pmt::mp("TX_CW"))
    {
      message_port_register_out(ber_info);
      message_port_register_out(msg_port);
      message_port_register_out(cw_port);
      d_gen = true; d_stop = false;
      d_t = 0; d_r = N; d_i = 0; d_k=0; // d_r is the nb of remaning symbols to send by the grc block
      d_id_demand = 0;
      d_n_col=0;
      DEBUG = true;
      d_GRASP = true;

      designSNRdb = 0;

      d_next_tag_pos = 0;
      d_k=0; // counter on the packet

      // Weak user
      /*variance_w = pow(10,-(d_SNR_w/10));
      sqrtVariance_w = sqrt(variance_w);*/

      PC_w.constructPC(d_N, d_K_w, designSNRdb);

      info_w = new int[d_K_w];
      frozen_w = new int[d_N-d_K_w];

      // Strong user
      /*variance_s = pow(10,-(d_SNR_s/10));
      sqrtVariance_s = sqrt(variance_s);*/

      //PC_s.constructPC(d_N, d_K_s, designSNRdb);
      PC_s.initPC(d_N, d_K_s, designSNRdb);
      PC_s.setGenMatrix(PC_w.genMatrix);
      PC_s.setRn(PC_w.Rn);
      PC_s.setArrangedBits(PC_w.arragedBits);


      info_s = new int[d_K_s];
      frozen_s = new int[d_N-d_K_s];


      // Same for both users
      sentMessage = new int[d_N];
      initialMessage = new int[d_N];
      sentCodeword = new int[d_N];
      sentSymbol = new int[d_N];

    }

    /*
     * Our virtual destructor.
     */
    polarEnc_b_impl::~polarEnc_b_impl()
    {
    }

    void polarEnc_b_impl::cleanVar()
    {
        cout << endl << "START CLEANVAR()" << endl;
        int data_size;
        //Free coded_data

        for (unsigned int q = 0; q < d_coded_data.size(); q++)
            d_coded_data.at(q).clear();

        cout << endl << "A" << endl;
        d_coded_data.clear();

        // OTHMANE
        d_offset=0;

        //Free header
        for (unsigned int q = 0; q < d_n_col; q++)
        {

            d_header_data[q].id_utenti.clear();
            d_header_data[q].id_files.clear();
            d_header_data[q].id_chunks.clear();
            d_header_data[q].size_package.clear();

            // OTHMANE : Core dump bug comes from here
            // d_hdr_sdata[q].id_utenti.clear();
            // d_hdr_sdata[q].id_files.clear();
            // d_hdr_sdata[q].id_chunks.clear();
            // d_hdr_sdata[q].size_package.clear();

        }

        data_size = d_transmission.size();
        for(int i=0; i<data_size; i++)
            d_transmission.at(i).clear();

        data_size = d_PC_data.size();
        for(int i=0; i<data_size; i++)
            d_PC_data.at(i).clear();

        data_size = d_hX.size();
        for(int i=0; i<data_size; i++){
          d_hX[i].id_utenti.clear();
          d_hX[i].id_files.clear();
          d_hX[i].id_chunks.clear();
          d_hX[i].size_package.clear();
        }
        d_hX.clear();

        cout << endl << "B" << endl;

        data_size = d_strg_data.size();
        for(int i=0; i<data_size; i++)
            d_strg_data.at(i).clear();

        data_size = G_edges.size();
        for(int i=0; i<data_size; i++)
            G_edges.at(i).clear();

        data_size = d_bits_coded.size();
        for(int i=0; i<data_size; i++)
            d_bits_coded.at(i).clear();

        //free transmission vector
        d_transmission1.clear();

        //free pack size vector
        d_spack_size.clear();

        deallocationAllVariables(&d_outputForColoring, d_data, d_coloring);
        cout << endl << "C" << endl;

    }/*end cleanVar()*/

    int
    polarEnc_b_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      //const char *in = (const char *) input_items[0];
      char *out = (char *) output_items[0];


      if(d_gen){

        // cout << "Polar codes" << endl; // prints Polar codes
        d_gen = false;

        cout << endl << "Data generation process" << endl << "-------------" << endl << endl;

        d_data = generateData(d_m_files, d_b_chunks, d_id_demand);


        cout << endl << "Conflict-Graph generator process" << endl << "-------------" << endl << endl;
        d_outputForColoring = conflictGraphGenerator(d_data);
        cout << endl << "Numero nodi del grafo: " << d_outputForColoring.n_nodi << endl << endl;

        if (d_outputForColoring.n_nodi > 0)
        {
            cout << endl << "Graph Coloring process" << endl << "-------------" << endl << endl;
            // Coloring
            if (d_GRASP)
            {
                int d_maxIter = 20;
                d_coloring = graspGraphColoring(d_maxIter, d_outputForColoring, &d_n_col);
                colorRienumeration(d_n_col, &d_coloring, d_outputForColoring.n_nodi);
            }

            cout << endl << "La colorazione e' stata effettuata con successo!" << endl;
            cout << endl << "Numero di colori utilizzati: " << d_n_col << endl;

        }/*end if (d_outputForColoring.n_nodi > 0)*/


        if (d_n_col > 0)
        {
            cout << endl << "Coding data process" << endl << "-------------" << endl << endl;
            //Coding data for transmission
            d_coded_data = codingData(d_coloring, d_n_col, d_data, d_outputForColoring, &d_header_data);


            //Coding strong and weak data
            d_strg_data = MaxBipartiteGraph(d_coloring, d_n_col, d_outputForColoring.nodes,
                d_outputForColoring.n_nodi, d_nb_strg, d_data, &d_hdr_sdata, G_edges);






            vector<vector<int>> sentCodewords_all, sentMessages_all;
            //Polar codes the weak and strong packets
            d_PC_data = codingDataPolar(d_coded_data, d_strg_data, d_bits_coded, G_edges, d_header_data, d_hdr_sdata, d_hX, d_N, sentCodewords_all, sentMessages_all);

            cout << endl << "EEE" << endl;

            // vector<uint8_t> vec_dict_msg;



            // message_port_pub(msg_port, dict_msg_end);
            // message_port_pub(cw_port, dict_msg_end);
            // cout << endl << "GGG" << endl;

            // exit(0);

            /////////////////////////////////////////////////////

            TX_PC_Pack(d_hX, d_PC_data, d_id_demand, d_transmission, d_spack_len, d_spack_size);


        }/* end if (d_n_col > 0) */

        int TXdata_size = d_transmission.size();
        cout << endl;
        for(int i=0; i<TXdata_size; i++){
          int ss = d_transmission.at(i).size();
          cout << ss << ", ";
          for(int j=0; j<ss; j++)
            d_transmission1.push_back(d_transmission[i][j]);
        }
        cout << endl;

        /*------------------------TO LOOK INTO LATER - SHOULD BE ADAPTED TO THE NEW MODIFICATIONS-----------------------*/

        //Repeat the last packet for many times to resolve hardware problem
        int k=0;
        char buff_short[2];
        while(k<300){
          conv_short_int_to_char(20, buff_short);
          for(int j=0; j<2; j++)
              d_transmission1.push_back(buff_short[j]);
          for(int j=0; j<d_spack_len; j++)
              d_transmission1.push_back(0);
          d_spack_size.push_back(d_spack_len+2);
          k++;
        }
        /*---------------------------------------------------*/


        //add tags to the stream
        int totSize = d_spack_size.size();
        while(d_k<totSize)
        {
            d_packet_len_pmt = pmt::from_long(d_spack_size.at(d_k));
            add_item_tag(0, d_next_tag_pos, d_len_tag_key, d_packet_len_pmt, pmt::string_to_symbol("TX"));//d_next_tag_pos
            add_item_tag(0, d_next_tag_pos, pmt::string_to_symbol("packet num"), pmt::from_long(d_k), pmt::string_to_symbol("TX"));
            d_next_tag_pos += d_spack_size.at(d_k);
            d_k ++;
        }
        cout << "\n\nTotal Number of transmitted small packets: " << d_k << endl;
      }


      if(d_offset >= d_transmission1.size() ){

          /************************** OTHMANE : BER INFO ********************************/
          // Last is for strong, rest is weak
          // Each row has 3 elements: user_id | file id | chunks matrix (0 isn't cached, 1 is cached)
          vector<string> normal;
          string tmp_str;
          // Add Weak Info
          for(int i=0; i<d_data.n_utenti; i++)
          {
            tmp_str = to_string(i) + " " + to_string(d_data.Q[i]) + " ";
            for (int k=0; k<d_b_chunks; k++)
              tmp_str += to_string(d_data.Q_chuncks[i][k]);  // requested chunks matrix
            normal.push_back(tmp_str); // weak user ID
          }

          // Add Strong Info
          for(int i=0; i<d_nb_strg; i++)
          {
            tmp_str = to_string(d_hdr_sdata[i*d_b_chunks].id_utenti[0]) + " " + to_string(d_hdr_sdata[i*d_b_chunks].id_files[0]) + " ";
            cout << endl << tmp_str << endl ;
            for (int k=0; k<d_b_chunks; k++)
              tmp_str += to_string(1);  // adds 100 zeros because strong has no cache
            normal.push_back(tmp_str);
          }

          // Debug for Tx BER
          sleep(6);
          cout << endl << "BER VECTOR DEBUG : " << endl ;
          for(int i=0; i<normal.size(); i++)
            cout << normal[i] << endl;

          for (int k = 0; k < normal.size(); k++)
          {
            vector<uint8_t> vec_dict_msg(normal[k].begin(), normal[k].end());
            pmt::pmt_t TEST_msg = pmt::init_u8vector(vec_dict_msg.size(), vec_dict_msg);

            pmt::pmt_t dict_pdu(pmt::make_dict());
            dict_pdu = pmt::cons(pmt::make_dict(), TEST_msg);
            message_port_pub(ber_info, dict_pdu);
            vec_dict_msg.clear();
          }

          vector<uint8_t> V_TMP{0};
          pmt::pmt_t dict_msg_end = pmt::cons(pmt::make_dict(), pmt::init_u8vector(1,V_TMP));
          // pmt::pmt_t dict_msg = pmt::cons(dict_msg, pmt::from_long(-1), pmt::intern("TX MSG END"));
          // cout << endl << "FFF" << endl;


          cout << "Done!" << endl;
          cleanVar();
          return -1;  // Done!

          // string repo_file, name_file;
          // repo_file = "../repository/file_0.xml";
          // name_file = "../trasmissioni/User_" + to_string(d_id_user) + "/decoded_file_" + to_string(d_id_demand) + ".xml";
          // // name_file = "../trasmissioni/User_" + my_to_string(d_id_user) + "/decoded_file_0.xml";
          // mycompare(repo_file, name_file, d_b_chunks);
          // cout << endl << "OTHMANE END :" << endl;
      }

      unsigned int n = std::min((unsigned)d_transmission1.size() - d_offset,
                              (unsigned)noutput_items);
      for(unsigned int i = 0; i < n; i++) {
        out[i] = d_transmission1[d_offset + i];
      }

      // vector<unsigned int> TMP;
      // conv_char_to_bitsInt

      d_offset += n;
      return n;

    }/* work function */
  } /* namespace projectCACHE */
} /* namespace gr */






            // // OTHMANE
            // // Create and Send PDU message with sent messages and CWs
            // for (int k = 0; k < d_hX.size(); k++){
            //   if(d_hX[k].strong){
            //     pmt::pmt_t dict_msg(pmt::make_dict());
            //     pmt::pmt_t dict_cw(pmt::make_dict());
            //     stringstream str_msg, str_cw;
            //     int strg_ind;
            //     // cout << endl << "AAA" << endl;
            //
            //     if(d_hX[k].weak){   // If Polar header concerns hybrid packet (weak+strong)
            //       int n = d_hX[k].id_chunks.size();
            //       strg_ind = d_hX[k].id_chunks[n-1];
            //     }
            //     else    // Polar header concerns Strictly Strong Packet
            //       strg_ind = d_hX[k].id_chunks[0];
            //
            //     // Fill Tx CWs and Msgs in the same loop to optimize runtime
            //     for (int i=0; i<d_N; i++){
            //       str_cw << sentCodewords_all[k][i] ;
            //       if(i<d_K_s)
            //         str_msg << sentMessages_all[k][i] ;
            //     }
            //
            //     // dict_msg = pmt::dict_add(dict_msg, pmt::from_long(strg_ind), pmt::intern(str_msg.str()));
            //     // dict_cw = pmt::dict_add(dict_cw, pmt::from_long(strg_ind), pmt::intern(str_cw.str()));
            //     // vector<uint8_t> vec(str_msg.str().begin(), str_msg.str().end());
            //
            //     string str_msg_str = str_msg.str();
            //     string str_cw_str = str_cw.str();
            //
            //     std::vector<uint8_t> vec_dict_msg, vec_dict_cw;
            //
            //     vec_dict_msg.assign(str_msg_str.begin(), str_msg_str.end());
            //     vec_dict_cw.assign(str_cw_str.begin(), str_cw_str.end());
            //
            //     vec_dict_msg.insert(vec_dict_msg.begin(), strg_ind);
            //     vec_dict_cw.insert(vec_dict_cw.begin(), strg_ind);
            //
            //     pmt::pmt_t TEST_msg = pmt::init_u8vector(vec_dict_msg.size(), vec_dict_msg);
            //     pmt::pmt_t TEST_cw = pmt::init_u8vector(vec_dict_cw.size(), vec_dict_cw);
            //     // cout << endl << "BBB" << endl;
            //     // cout << endl << "OTHMANE :" << strg_ind << endl;
            //
            //     dict_msg = pmt::cons(pmt::make_dict(), TEST_msg);
            //     dict_cw = pmt::cons(pmt::make_dict(), TEST_cw);
            //
            //     message_port_pub(msg_port, dict_msg);
            //     message_port_pub(cw_port, dict_cw);
            //
            //     // intrusive_ptr_release(dict_msg);
            //     // intrusive_ptr_release(dict_cw);
            //     // cout << endl << "CCC" << endl;
            //
            //     vec_dict_msg.clear();
            //     vec_dict_cw.clear();
            //     // cout << endl << "DDD" << endl;
            //
            //   }
            // }
