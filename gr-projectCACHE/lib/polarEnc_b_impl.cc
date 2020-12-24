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
    polarEnc_b::make(const int N, int m_files, int b_chunks, int nb_strg, int id_user, int spack_len, const std::string &len_tag_key)//
    {
      return gnuradio::get_initial_sptr
        (new polarEnc_b_impl(N, m_files, b_chunks, nb_strg, id_user, spack_len, len_tag_key));
    }

    /*
     * The private constructor
     */
    polarEnc_b_impl::polarEnc_b_impl(const int N, int m_files, int b_chunks, int nb_strg, int id_user, int spack_len, const std::string &len_tag_key)
      : gr::sync_block("polarEnc_b",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
      d_N(N),
      d_m_files(m_files),
      d_b_chunks(b_chunks),
      d_nb_strg(nb_strg),
      d_id_user(id_user),
      d_offset(0),
      d_spack_len(spack_len),
      ber_info(pmt::mp("BER_INFO")),
      d_len_tag_key(pmt::string_to_symbol(len_tag_key))
    {
      message_port_register_out(ber_info);
      d_gen = true; d_stop = false;
      d_t = 0; d_r = N; d_i = 0; d_k=0; // d_r is the nb of remaning symbols to send by the grc block
      d_id_demand = 0;
      d_n_col=0;
      DEBUG = true;
      d_GRASP = true;

      designSNRdb = 0;

      d_next_tag_pos = 0;
      d_k=0; // counter on the packet
      //The code rate to be given later on as a parameter
      // d_coderate = {1, 2, 3, 4 , 4}; //{3, 2, 2, 2, 3, 4};
      d_coderate = {1, 2, 3, 4, 4}; //{3, 2, 2, 2, 3, 4};
      // Weak user
      /*variance_w = pow(10,-(d_SNR_w/10));
      sqrtVariance_w = sqrt(variance_w);*/

      /*PC_w.constructPC(d_N, d_K_w, designSNRdb);

      info_w = new int[d_K_w];
      frozen_w = new int[d_N-d_K_w];

      // Strong user
      /*variance_s = pow(10,-(d_SNR_s/10));
      sqrtVariance_s = sqrt(variance_s);*/

      //PC_s.constructPC(d_N, d_K_s, designSNRdb);
      /*PC_s.initPC(d_N, d_K_s, designSNRdb);
      PC_s.setGenMatrix(PC_w.genMatrix);
      PC_s.setRn(PC_w.Rn);
      PC_s.setArrangedBits(PC_w.arragedBits);


      info_s = new int[d_K_s];
      frozen_s = new int[d_N-d_K_s];


      // Same for both users
      sentMessage = new int[d_N];
      initialMessage = new int[d_N];
      sentCodeword = new int[d_N];
      sentSymbol = new int[d_N];*/

    }

    /*
     * Our virtual destructor.
     */
    polarEnc_b_impl::~polarEnc_b_impl()
    {
    }

    void polarEnc_b_impl::cleanVar()
    {
        int data_size;
        //Free coded_data
        for (unsigned int q = 0; q < d_coded_data.size(); q++)
            d_coded_data.at(q).clear();

        d_coded_data.clear();

        //Free header
        for (unsigned int q = 0; q < d_n_col; q++)
        {

            d_header_data[q].id_utenti.clear();
            d_header_data[q].id_files.clear();
            d_header_data[q].id_chunks.clear();
            d_header_data[q].size_package.clear();

            d_hdr_sdata[q].id_utenti.clear();
            d_hdr_sdata[q].id_files.clear();
            d_hdr_sdata[q].id_chunks.clear();
            d_hdr_sdata[q].size_package.clear();

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


        data_size = d_strg_data.size();
        for(int i=0; i<data_size; i++)
            d_strg_data.at(i).clear();

        data_size = G_edges.size();
        for(int i=0; i<data_size; i++)
            G_edges.at(i).clear();

        data_size = bits_coded.size();
        for(int i=0; i<data_size; i++)
            bits_coded.at(i).clear();

        //free transmission vector
        d_transmission1.clear();

        //free pack size vector
        d_spack_size.clear();

        deallocationAllVariables(&d_outputForColoring, d_data, d_coloring);

    }/*end cleanVar()*/

    int
    polarEnc_b_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      //const char *in = (const char *) input_items[0];
      char *out = (char *) output_items[0];


      if(d_gen){

        cout << "Polar codes" << endl; // prints Polar codes
        d_gen = false;

        cout << endl << "Data generation process" << endl << "-------------" << endl << endl;


        d_data = generateData(d_m_files, d_b_chunks, d_id_demand);

        cout << endl << "Conflict-Graph generator process" << endl << "-------------" << endl << endl;
        d_outputForColoring = conflictGraphGenerator(d_data, d_coderate);
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
            cout << "The expected gain is: " << (100*(d_outputForColoring.n_nodi-d_n_col)/d_outputForColoring.n_nodi) << "%" << endl;

        }/*end if (d_outputForColoring.n_nodi > 0)*/


        if (d_n_col > 0)
        {
            cout << endl << "Coding data process" << endl << "-------------" << endl << endl;
            //Coding data for transmission
            //d_coded_data = codingData(d_coloring, d_n_col, d_data, d_outputForColoring, &d_header_data);
            d_coded_data = codingVarCodeRate(d_coloring, d_n_col, d_data, d_outputForColoring, &d_header_data,d_coderate);

            //Coding strong and weak data
            d_strg_data = MaxBipartiteGraph(d_coloring, d_n_col, d_outputForColoring.nodes,
                d_outputForColoring.n_nodi, d_nb_strg, d_data, &d_hdr_sdata, G_edges);

            //Polar codes the weak and strong packets
            d_PC_data = codingDataPolar(d_coded_data, d_strg_data, bits_coded, G_edges, d_header_data, d_hdr_sdata, d_hX, d_N);
            //cout << "The total number of transmitted packet is: " << d_PC_data.size() << endl;


            TX_PC_Pack(d_hX, d_PC_data, d_id_demand, d_transmission, d_spack_len, d_spack_size);

            /*int TXdata_size = d_transmission.size();

            for(int i=0; i<TXdata_size; i++){
              d_packet_size.push_back(d_transmission[i].size());
            }*/

        }/* end if (d_n_col > 0) */

        //d_transmission1 is the vector stream whereas d_transmission is the matrix with all data
        int TXdata_size = d_transmission.size();
        //cout << endl;
        for(int i=0; i<TXdata_size; i++){
          int ss = d_transmission.at(i).size();
          //cout << ss << ", ";
          for(int j=0; j<ss; j++)
            d_transmission1.push_back(d_transmission[i][j]);
        }
        //cout << endl;

        /*------------------------TO LOOK INTO LATER - SHOULD BE ADAPTED TO THE NEW MODIFICATIONS-----------------------*/

        /*-----------------------CORTEXLAB -- MUST BE ADDED------------*/

        //Repeat the last packet for many times to resolve some hardware problem
        //Because witout doing that, the x last packets are not received for unknown
        //reasons (maybe stuck inside the buffer)
        /*int k=0;
        char buff_short[2];
        while(k<0){
          conv_short_int_to_char(20, buff_short);
          for(int j=0; j<2; j++)
              d_transmission1.push_back(buff_short[j]);
          for(int j=0; j<d_spack_len; j++)
              d_transmission1.push_back(0);
          d_spack_size.push_back(d_spack_len+2);
          k++;
        }*/
        /*---------------------------------------------------*/


        //add tags to the stream
        int totSize = d_spack_size.size();
        while(d_k<totSize)
        {
            d_packet_len_pmt = pmt::from_long(d_spack_size.at(d_k));
            add_item_tag(0, d_next_tag_pos, d_len_tag_key, d_packet_len_pmt, pmt::string_to_symbol("TX"));
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
          sleep(8);
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
          /**********************************************************************/


          cout << "Done!" << endl;
          //cleanVar();
          return -1;  // Done!
      }

      unsigned int n = std::min((unsigned)d_transmission1.size() - d_offset,
                              (unsigned)noutput_items);
      for(unsigned int i = 0; i < n; i++) {
        out[i] = d_transmission1[d_offset + i];
      }

      d_offset += n;

      return n;

    }/* work function */
  } /* namespace projectCACHE */
} /* namespace gr */
