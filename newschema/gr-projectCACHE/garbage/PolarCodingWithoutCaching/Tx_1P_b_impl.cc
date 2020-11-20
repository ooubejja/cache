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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "Tx_1P_b_impl.h"


using namespace std;
using namespace caching;


namespace gr {
  namespace projectCACHE {

    Tx_1P_b::sptr
    Tx_1P_b::make(bool GRASP, unsigned int n_utenti, unsigned int m_files, unsigned int b_chunks, unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key)
    {
      return gnuradio::get_initial_sptr
        (new Tx_1P_b_impl(GRASP, n_utenti, m_files, b_chunks, L_demands, small_packet_len, len_tag_key));
    }

    /*
     * The private constructor
     */
    Tx_1P_b_impl::Tx_1P_b_impl(bool GRASP, unsigned int n_utenti, unsigned int m_files, unsigned int b_chunks, unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key)
      : gr::sync_block("Tx_1P_b",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(char))),

              d_GRASP(GRASP),
              d_n_utenti(n_utenti),
              d_m_files(m_files),
              d_b_chunks(b_chunks),
              d_L_demands(L_demands),
              d_small_packet_len(small_packet_len),

              d_n_col(0),
              d_total_small_packet_size(0),
              d_id_demand(0),

              d_packet_len_pmt(pmt::from_long(0)),
              d_len_tag_key(pmt::string_to_symbol(len_tag_key)),

              d_gen(true),

              d_i(0),
              d_j(0),
              d_cc(0),
              d_k(0),
              d_last(0),
              d_next_tag_pos(0)

    {}

    /*
     * Our virtual destructor.
     */
    Tx_1P_b_impl::~Tx_1P_b_impl()
    {
    }


    void Tx_1P_b_impl::cleanVar()
    {

        //Free coded_data
        for (unsigned int q = 0; q < d_coded_data.size(); q++)
        {
            d_coded_data.at(q).clear();
            //coded_data.at(q).shrink_to_fit();
        }
        d_coded_data.clear();
        //coded_data.shrink_to_fit();

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

            /*header_data[q].id_chunks.shrink_to_fit();
            header_data[q].id_files.shrink_to_fit();
            header_data[q].id_utenti.shrink_to_fit();
            header_data[q].size_package.shrink_to_fit();*/
        }

        //free transmission vector
        d_transmission.clear();

        //free pack size vector
        d_small_packet_size.clear();

        deallocationAllVariables(&d_outputForColoring, d_data, d_coloring);

        d_i = 0;
        d_j = 0;
        d_cc = 0;
        d_n_col = 0;
        d_total_small_packet_size = 0;

    }/*end cleanVar()*/




    int
    Tx_1P_b_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        char *out = (char *) output_items[0];


/*        if(d_id_demand >= d_L_demands)
            return 0;
*/
        if(d_gen)
        {
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
                }else{
                    d_coloring = hGCC(&d_n_col, d_data, d_outputForColoring);
                    colorRienumeration(d_n_col, &d_coloring, d_outputForColoring.n_nodi);
                }

                cout << endl << "La colorazione e' stata effettuata con successo!" << endl;
                cout << endl << "Numero di colori utilizzati: " << d_n_col << endl;

            }/* end if (d_outputForColoring.n_nodi > 0) */

            if (d_n_col > 0)
            {
                cout << endl << "Coding data process" << endl << "-------------" << endl << endl;
                //Coding data for transmission
                d_coded_data = codingData(d_coloring, d_n_col, d_data, d_outputForColoring, &d_header_data);

                //Coding strong and weak data
                /*vector<vector<bool>> G_edges;
                d_strg_data = MaxBipartiteGraph(d_coloring, d_n_col, d_outputForColoring.nodes, 
                    d_outputForColoring.n_nodi, nb_strg, d_data, &d_hdr_sdata, G_edges);*/

                //Polar codes the weak and strong packets
                //d_PC_data = PolarCode(d_coded_data, d_strg_data, G_edges, &d_header_data, &d_hdr_sdata);

                Transmit(d_header_data, d_coded_data, d_id_demand, d_n_col, d_small_packet_len, d_transmission, d_small_packet_size);

                cout << endl << "Size of Transmission = " << d_transmission.size() << endl << endl;

                cout << endl <<"Size of Packs_size vector = " << d_small_packet_size.size() << endl << endl;

                for(unsigned int i=0; i<d_small_packet_size.size(); i++)
                {
                    //cout << "Size of packet (" << i << ") = " << d_small_packet_size.at(i) << endl;
                    d_total_small_packet_size += d_small_packet_size.at(i);
                }
                cout << endl <<"Sum of all packet size = " << d_total_small_packet_size << endl << endl;

            }/* end if (d_n_col > 0) */

        }/* end if (d_gen) */



        //write transmission on output stream
        if(d_i < (d_transmission.size() - noutput_items))
        {
            for(int k=0; k<noutput_items; k++)
            {
                //cout << endl << "Transmission at (" << d_i << ") = " << d_transmission.at(d_i) << endl << endl;
                out[k]=d_transmission.at(d_i);
                d_i ++;
            }

        }else if(d_i < d_transmission.size())
        {

            d_cc = d_transmission.size() - d_i;

            //compute len_tag_key for the last item
            d_k = noutput_items - d_cc;

            if(d_k > 0)
            {
                d_pack = d_k / d_small_packet_len;

                for(unsigned int i=0; i<d_pack; i++)
                {
                    d_small_packet_size.push_back(d_small_packet_len);
                }

                d_last = d_k % d_small_packet_len;

                if(d_last > 0)
                {
                    d_small_packet_size.push_back(d_last);
                }
            }


            for(int k=0; k<noutput_items; k++)
            {
                if(k<d_cc)
                {
                    //cout << "Transmission at (" << d_i << ") = " << d_transmission.at(d_i) << endl;
                    out[k]=d_transmission.at(d_i); 
                    d_i++;

                }else
                {
                    out[k] = 1;
                }
            }

            //END TRANSMISSION
            d_id_demand ++;
            if(d_id_demand < d_L_demands)
            {
                d_gen = true;
            
            }else{

                cleanVar();
                cout << "-----------END of TRANSMISSION--------------" << endl;
                return -1;
            }

        }else
        {
            for(int k=0; k<noutput_items; k++)
            {
                out[k]=1;
            }
            d_packet_len_pmt = pmt::from_long(noutput_items);
            add_item_tag(0, nitems_written(0), d_len_tag_key, d_packet_len_pmt, pmt::string_to_symbol("TX END"));
        }


        //add tags to the stream
        while(d_next_tag_pos < nitems_written(0) + noutput_items && d_j < d_small_packet_size.size())
        {
            d_packet_len_pmt = pmt::from_long(d_small_packet_size.at(d_j));

            add_item_tag(0, d_next_tag_pos, d_len_tag_key, d_packet_len_pmt, pmt::string_to_symbol("TX"));

            d_next_tag_pos += d_small_packet_size.at(d_j);

            d_j ++;
        }

        if(d_gen)
        {
            cleanVar();
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace projectCACHE */
} /* namespace gr */

