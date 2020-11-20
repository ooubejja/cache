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
#include "Rx_1P_b_impl.h"


using namespace std;
using namespace caching;


namespace gr {
  namespace projectCACHE {

    Rx_1P_b::sptr
    Rx_1P_b::make(unsigned int id_utente, unsigned int n_utenti, unsigned int m_files, unsigned int b_chunks, unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key, bool DEBUG)
    {
      return gnuradio::get_initial_sptr
        (new Rx_1P_b_impl(id_utente, n_utenti, m_files, b_chunks, L_demands, small_packet_len, len_tag_key, DEBUG));
    }

    /*
     * The private constructor
     */
    Rx_1P_b_impl::Rx_1P_b_impl(unsigned int id_utente, unsigned int n_utenti, unsigned int m_files, unsigned int b_chunks, unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key, bool DEBUG)
      : gr::tagged_stream_block("Rx_1P_b",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(0, 0, 0), len_tag_key),


              d_id_utente(id_utente),
              d_n_utenti(n_utenti),
              d_m_files(m_files),
              d_b_chunks(b_chunks),
              d_L_demands(L_demands),
              d_small_packet_len(small_packet_len),
              d_DEBUG(DEBUG),

              d_id_demand(0),
              d_next_id_demand(0),
              
              d_id_requested_file(0),
              d_n_package_remains(0),


              d_id_small_packet(0),
              d_id_expected(0),
              d_payload_len(0),
              d_number_small_packet(0),
              d_last_small_packet_len(0),
              d_id_last_small_packet(0),
              d_dummy_bytes(0),

              d_time_sum(0),

              d_case(0)

    {

        //string pathFolder = "../trasmissioni/User_" + my_to_string(id_utente);
        string pathFolder = "/CachingFile/trasmissioni/User_" + my_to_string(id_utente);
        if (mkdir(pathFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            cout << endl << "Error creating folder: " << pathFolder << endl;
            exit(0);
        }

        cout << endl << "Decoding data process:";
        cout << endl << "User id: " << d_id_utente;

        for(unsigned int i = 0; i < d_L_demands; i++)
        {
            //Read environment file
            cout << endl << "Reading Environment" << endl << "-------------" << endl << endl;
            d_data = generateData(d_m_files, d_b_chunks, i);

            d_files_requested.push_back(d_data.Q[d_id_utente]);

            cout << endl << "Request File " << i << " : " << d_files_requested.at(i) << endl << endl;
        }


    }/* end costructor */

    /*
     * Our virtual destructor.
     */
    Rx_1P_b_impl::~Rx_1P_b_impl()
    {
    }

    int
    Rx_1P_b_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = 0;
      return noutput_items ;
    }




    void Rx_1P_b_impl::decodeHeader(const char *in)
    {
        unsigned int i = 2;

        unsigned int header_len = 0;
        unsigned int field_len = 0;

        char byte;
        char buff[4];

        if(d_DEBUG)
        {
            cout << endl << "HEADER: ";
        }



        //read header length
        byte=in[i];
        i++;
        conv_byte_to_int(byte, header_len);
        field_len = (header_len - 2) / 7;
        
        if(d_DEBUG)
        {
            cout << endl << "Header Length = " << header_len;
        }

        //read id_demands
        byte=in[i];
        i++;
        conv_byte_to_int(byte, d_id_demand);
        
        if(d_DEBUG)
        {
            cout << endl << "ID Demand = " << d_id_demand;
        }


        //read header.id_utenti
        for(unsigned int j=0; j<field_len; j++)
        {
            d_header.id_utenti.push_back(0);
            byte=in[i];
            i++;
            conv_byte_to_int(byte, d_header.id_utenti[j]);
            
            if(d_DEBUG)
            {
                cout << endl << "ID Utenti = " << d_header.id_utenti[j];
            }
        }
        

        //read header.id_files
        for(unsigned int j=0; j<field_len; j++)
        {
            d_header.id_files.push_back(0);
            byte=in[i];
            i++;
            conv_byte_to_int(byte, d_header.id_files[j]);
            
            if(d_DEBUG)
            {
                cout << endl << "ID Files = " << d_header.id_files[j];
            }
        }


        //read header.id_chunks
        for(unsigned int j=0; j<field_len; j++)
        {
            d_header.id_chunks.push_back(0);
            byte=in[i];
            i++;
            conv_byte_to_int(byte, d_header.id_chunks[j]);
            
            if(d_DEBUG)
            {
                cout << endl << "ID Chunks = " << d_header.id_chunks[j];
            }
        }


        //read header.size_package and compute payload_len
        d_payload_len = 0;

        for(unsigned int j=0; j<field_len; j++)
        {
            d_header.size_package.push_back(0);
    
            for(int k=0; k<4; k++)
            {   
                buff[k]=in[i];
                i++;
            }
            conv_char_to_int(buff, d_header.size_package[j]);

            if(d_DEBUG)
            {
                cout << endl << "Size Package = " << d_header.size_package[j];
            }

            //find max_payload_len in the header
            if(field_len>1)
            {
                if(d_header.size_package[j] > d_payload_len)
                {
                    d_payload_len = d_header.size_package[j];
                }
            }else
            {
                d_payload_len = d_header.size_package[j];
            }
        }

        if(d_DEBUG)
        {
            cout << endl << endl << "Payload Length = " << d_payload_len;
        }

        //compute total number of small packet
        d_number_small_packet = (d_payload_len / d_small_packet_len) + 1; //+1 for the header small packet;
        
        //compute last small packet length
        d_last_small_packet_len = d_payload_len % d_small_packet_len;
        
        if(d_last_small_packet_len > 0)
        {
            d_number_small_packet ++;
            d_id_last_small_packet = d_number_small_packet - 1;
        }else
        {
            d_id_last_small_packet = d_number_small_packet - 1;
            d_last_small_packet_len = d_small_packet_len;
        }

        // check if number of small packet is minus then 65535
        if (d_number_small_packet > 65535)
        {
            cout << endl << endl << "ERROR: number_small_packet > 65535 " << endl;
            exit(0);
        }
        else if(d_number_small_packet == 65535 && d_last_small_packet_len != 0)
        {
            cout << endl << endl << "ERROR: number_small_packet = 65535  AND  last_small_packet_len != 0" << endl;
            exit(0);
        }

        if(d_DEBUG)
        {
            cout << endl << " number_small_packet = " << d_number_small_packet;
            cout << endl << " id_last_small_packet_len = " << d_id_last_small_packet;
            cout << endl << " last_small_packet_len = " << d_last_small_packet_len;
        }

    }/* end void decodeHeader() */


    void Rx_1P_b_impl::changeRequest()
    {
        d_id_requested_file = d_files_requested.at(d_id_demand);

        //Read cache info
        cout << endl << "Reading Cache Info" << endl << "-------------" << endl << endl;
        d_n_package_remains = readCacheInfo(d_id_utente, d_id_demand);

    }/* end void changeRequest() */


    void Rx_1P_b_impl::decodeSmallPack(const char *in, unsigned int small_pack_len)
    {
        for(unsigned int k = 2; k <= small_pack_len+1; k++)
        {
            d_coded_data.push_back(in[k]);
        }
    }/* end void decodeSmallPack() */


    void Rx_1P_b_impl::fill_lacks()
    {
        for(unsigned int k = 0; k < d_dummy_bytes; k++)
        {
            d_coded_data.push_back(0x00);
        }
    }/* end void fill_lacks() */


    void Rx_1P_b_impl::decodeLargePack()
    {
        decodingData(d_header, d_coded_data, d_m_files, d_b_chunks, d_id_utente, d_id_demand, d_id_requested_file, &d_n_package_remains);

        //Free header and coded_data

        //SHRINK TO FIT DOES NOT COMPILE WITHOUT C++11
        d_coded_data.clear();
        //d_coded_data.shrink_to_fit();
        

        d_header.id_chunks.clear();
        d_header.id_files.clear();
        d_header.id_utenti.clear();
        d_header.size_package.clear();

        /*d_header.id_chunks.shrink_to_fit();
        d_header.id_files.shrink_to_fit();
        d_header.id_utenti.shrink_to_fit();
        d_header.size_package.shrink_to_fit();*/
        //SHRINK TO FIT DOES NOT COMPILE WITHOUT C++11

        //Restore all variable
        d_id_expected = 0;
        d_payload_len = 0;
        d_number_small_packet = 0;
        d_last_small_packet_len = 0;
        d_id_last_small_packet = 0;             //forse potrebbe inficiare qualche condizione nel main. ATTENZIONE!!!!

    }/* end void decodeLargePack() */


    int
    Rx_1P_b_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        char *out = (char *) output_items[0];

        
        clock_t t1, t2;

        t1 = clock();


        unsigned int i = 0;
        //char id_byte;
        char buff_short[2];

        //read id small packet
        buff_short[0]=in[i];
        i++;
        buff_short[1]=in[i];
        i++;
        conv_char_to_short_int(buff_short, d_id_small_packet);
        if(d_DEBUG)
        {
            cout << endl << "ID small packet = " << d_id_small_packet;
            cout << endl << "ID small packet expected = " << d_id_expected;
        }

        //Check condition of working and switch state
        if(d_id_small_packet != d_id_expected)
        {

                if(d_id_expected == 0) //Still don't receive the header
                {
                        d_case = 0;
                
                }else if(d_id_expected > 0)
                {

                        if(d_id_small_packet > d_id_expected) //don't receive some small packet of the Large Packet
                        {
                                d_case = 4;
                        
                        }else if(d_id_small_packet < d_id_expected) //don't receive the end of a Large Packet
                        {
                                d_case = 5;
                        }
                }


        }else if(d_id_small_packet == d_id_expected)
        {

                if(d_id_expected == 0) //If receive the header
                {
                        d_case = 1;
            
                }else if(d_id_expected == d_id_last_small_packet) //If receive the last small packet
                {
                        d_case = 3;
            
                }else
                { //If receive a small packet in right sequece

                        d_case = 2;
                }

        }



        switch(d_case)
        {
            case(0):
                //do nothing
                //Receiver waiting the id_small_packet = 0
                break;


            case(1): //Header received
                
                decodeHeader(in);

                if(d_next_id_demand == d_id_demand)
                {
                    changeRequest();
                    d_next_id_demand += 1;
                }

                d_id_expected += 1;
                
                break;


            case(2): //Small Packet received
                
                if(d_DEBUG)
                {
                    cout << endl << "d_small_packet_len = " << d_small_packet_len << endl;
                }
                if(d_small_packet_len != (ninput_items[0] - 2))
                {
                    cout << endl << "ERROR: small_packet_len != input_packet_len" << endl;
                    exit(0);
                }
                
                decodeSmallPack(in, d_small_packet_len);
                
                d_id_expected += 1;
                
                break;


            case(3): //Last Small Packet received
                
                if(d_DEBUG)
                {
                    cout << endl << "d_last_small_packet_len = " << d_last_small_packet_len << endl;
                }
                if(d_last_small_packet_len != (ninput_items[0] - 2))
                {
                    cout << endl << "ERROR: last_small_packet_len != input_packet_len" << endl;
                    exit(0);
                }
                
                decodeSmallPack(in, d_last_small_packet_len);

                decodeLargePack(); //in this funcion the variable are setted to inizial condition

                break;


            case(4): //Fill part of large packet with dummy bytes

                d_dummy_bytes = (d_id_small_packet - d_id_expected ) * (d_small_packet_len);

                fill_lacks();

                if(d_id_small_packet == d_id_last_small_packet) //If receive the last packet
                {
                    if(d_DEBUG)
                    {
                        cout << endl << "d_last_small_packet_len = " << d_last_small_packet_len << endl;
                    }
                    if(d_last_small_packet_len != (ninput_items[0] - 2))
                    {
                        cout << endl << "ERROR: last_small_packet_len != input_packet_len" << endl;
                        exit(0);
                    }
                
                    decodeSmallPack(in, d_last_small_packet_len);

                    decodeLargePack(); //in this funcion the variable are setted to inizial condition
                
                }else if(d_id_small_packet != d_id_last_small_packet) //If receive a generic small packet
                {

                    if(d_DEBUG)
                    {
                        cout << endl << "d_small_packet_len = " << d_small_packet_len << endl;
                    }
                    if(d_small_packet_len != (ninput_items[0] - 2))
                    {
                        cout << endl << "ERROR: small_packet_len != input_packet_len" << endl;
                        exit(0);
                    }
                
                    decodeSmallPack(in, d_small_packet_len);
                
                    d_id_expected = d_id_small_packet + 1;
                }

                break;


            case(5): //Lost the end of the previous large pack, so fill the lack and decode. After check for the next large packet

                d_dummy_bytes = ((d_id_last_small_packet - d_id_expected) * d_small_packet_len) + (d_last_small_packet_len);

                fill_lacks();

                decodeLargePack(); //in this funcion the variable are setted to inizial condition

                if(d_id_small_packet == 0)
                {
                    decodeHeader(in);

                    if(d_next_id_demand == d_id_demand)
                    {
                        changeRequest();
                        d_next_id_demand += 1;
                    }
                
                    d_id_expected += 1;
                
                }else if(d_id_small_packet != 0)
                {
                    //do nothing
                    //Receiver waiting the id_small_packet = 0
                }

                break;

        }



        t2 = clock();

        d_time_sum += ((double) (t2-t1))/CLOCKS_PER_SEC;

        if(d_DEBUG)
        {
            cout << endl << endl << "TIME ELAPSED : " << d_time_sum << "s" << endl;
        }


        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace projectCACHE */
} /* namespace gr */

