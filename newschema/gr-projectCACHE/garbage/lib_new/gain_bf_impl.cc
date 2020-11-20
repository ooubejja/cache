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
#include "gain_bf_impl.h"


using namespace std;
using namespace caching;


namespace gr {
  namespace projectCACHE {

    gain_bf::sptr
    gain_bf::make(unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key, bool DEBUG)
    {
      return gnuradio::get_initial_sptr
        (new gain_bf_impl(L_demands, small_packet_len, len_tag_key, DEBUG));
    }

    /*
     * The private constructor
     */
    gain_bf_impl::gain_bf_impl(unsigned int L_demands, unsigned int small_packet_len, const std::string &len_tag_key, bool DEBUG)
      : gr::tagged_stream_block("gain_bf",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(3, 3, sizeof(float)), len_tag_key),


            d_id_transmission(0),

            d_L_demands(L_demands),
            d_small_packet_len(small_packet_len),
            d_DEBUG(DEBUG),

            d_id_small_packet(0),
            d_id_expected(0),

            d_last_small_packet_len(0),
            d_id_last_small_packet(0),

            d_id_demand(0),


            d_header_len(0),
            d_field_len(0),
            d_payload_len(0),
            d_number_small_packet(0),

            d_tot_header_len(0),
            d_tot_payload_len(0),
            d_tot_number_small_packet(0),

            d_tot_naive_header_len(0),
            d_tot_naive_payload_len(0),
            d_tot_naive_number_small_packet(0),


            d_payload_gain(0),
            d_header_gain(0),
            d_small_pck_gain(0),

            d_average_payload_gain(0),
            d_average_header_gain(0),
            d_average_small_pck_gain(0),

            d_tot_payload_gain(0),
            d_tot_header_gain(0),
            d_tot_small_pck_gain(0),

            d_tot_byte_coded(0),
            d_tot_byte_naive(0),
              


            d_case(0)

    {}

    /*
     * Our virtual destructor.
     */
    gain_bf_impl::~gain_bf_impl()
    {
    }

    int
    gain_bf_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = 1;
      return noutput_items ;
    }


    void gain_bf_impl::decodeHeader(const char *in)
    {
        unsigned int i = 2;

        unsigned int header_len = 0;
        unsigned int field_len = 0;
        unsigned int payload_len = 0;
        unsigned int number_small_packet = 0;
        unsigned int crc = 0;

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
                if(d_header.size_package[j] > payload_len)
                {
                    payload_len = d_header.size_package[j];
                }
            }else
            {
                payload_len = d_header.size_package[j];
            }
        }

        if(d_DEBUG)
        {
            cout << endl << endl << "Payload Length = " << payload_len;
        }

        //compute total number of small packet
        number_small_packet = (payload_len / d_small_packet_len) + 1; //+1 for the header small packet;
        
        //compute last small packet length
        d_last_small_packet_len = payload_len % d_small_packet_len;
        
        if(d_last_small_packet_len > 0)
        {
            number_small_packet ++;
            d_id_last_small_packet = number_small_packet - 1;
        }else
        {
            d_id_last_small_packet = number_small_packet - 1;
            d_last_small_packet_len = d_small_packet_len;
        }

        crc = number_small_packet * 4;

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
            cout << endl << " number_small_packet = " << number_small_packet;
            cout << endl << " id_last_small_packet_len = " << d_id_last_small_packet;
            cout << endl << " last_small_packet_len = " << d_last_small_packet_len;
        }


        d_field_len = field_len;
        d_header_len = header_len;
        d_payload_len = payload_len;
        d_number_small_packet = number_small_packet;
        d_crc = crc;

    }/* end void decodeHeader() */



    void gain_bf_impl::computeGain()
    {

        vector<unsigned int> flag (d_field_len, 0);
        vector< vector<unsigned int> > equal_chunk;

        unsigned int naive_field_len = 0;
        float naive_header_len = 0;
        float naive_payload_len = 0;
        float naive_number_small_packet = 0;
        float naive_crc = 0;

        unsigned int num_pck = 0;
        unsigned int naive_last_small_packet = 0;

        float payload_gain = 0;
        float header_gain = 0;
        float small_pck_gain = 0;
        float crc_gain = 0;


        unsigned int i = 0;
        

        for (unsigned int j=0; j<d_field_len; j++)
        {
            if(flag.at(j) == 0)
            {
                equal_chunk.push_back(vector<unsigned int> ());
                equal_chunk.at(i).push_back(j);

                for(unsigned int k=j+1; k<d_field_len; k++)
                {
                    if(d_header.id_files[j] == d_header.id_files[k] && d_header.id_chunks[j] == d_header.id_chunks[k])
                    {
                        flag.at(j) = 1;
                        flag.at(k) = 1;
                        equal_chunk.at(i).push_back(k);
                    }
                }

                i++;
            }
        }

        if(d_DEBUG)
        {
            cout << endl << endl << "CHECK EQUAL PACKAGE : " << endl;
            cout << "Flag :";
            for (unsigned int j=0; j<d_field_len; j++)
            {
                cout << " " << flag.at(j) << " ";
            }
            cout << endl;

            cout << "Equal chunk :";
            for (unsigned int j=0; j<equal_chunk.size(); j++)
            {
                cout<<endl;
                for (unsigned int k=0; k<equal_chunk.at(j).size(); k++)
                {
                    cout << " " << equal_chunk.at(j).at(k) << " ";
                }

            }
            cout << endl;
        }

        //COMPUTE CODED MULTICASTING VALUE
        //compute total value
        d_tot_payload_len += d_payload_len;
        d_tot_header_len += d_header_len,
        d_tot_number_small_packet += d_number_small_packet;
        d_tot_crc += d_crc;

        d_tot_byte_coded += d_payload_len + d_header_len + (2 * d_number_small_packet) + d_crc;

        if(d_DEBUG)
        {
            cout << endl << "Coded Mulicasting";
            cout << endl << "Payload: " << d_payload_len << " byte ";
            cout << endl << "Header overhead: " << d_header_len << " byte ";
            cout << endl << "ID small packet overhead: " << (2 * d_number_small_packet) << " byte ";
            cout << endl << "CRC overhead: " << d_crc << " byte ";
            cout << endl;
        }




        //COMPUTE NAIVE MULTICASTING VALUE
        //for each naive chunk
        for(unsigned int j=0; j<equal_chunk.size(); j++)
        {
            //find field_len
            naive_field_len = equal_chunk.at(j).size();

            //sum header_len
            naive_header_len += (7 * naive_field_len) + 2;

            //sum all payload_len
            naive_payload_len += d_header.size_package[equal_chunk.at(j).at(0)];

            //compute number of small packet
            num_pck = (d_header.size_package[equal_chunk.at(j).at(0)] / d_small_packet_len) + 1; // + 1 for the header
            
            //if there is the last packet, add 1
            naive_last_small_packet = d_header.size_package[equal_chunk.at(j).at(0)] % d_small_packet_len;
            if (naive_last_small_packet > 0)
            {
                num_pck ++;
            }
            //sum all number of small packet
            naive_number_small_packet += num_pck;

            naive_crc += (num_pck * 4);

            if(d_DEBUG)
            {
                cout << endl << "Naive Mulicasting packet " << j << " : ";
                cout << endl << "Payload: " << d_header.size_package[equal_chunk.at(j).at(0)] << " byte ";
                cout << endl << "Header overhead: " << ((7 * naive_field_len) + 1) << " byte ";
                cout << endl << "ID small packet overhead: " << (2 * num_pck) << " byte ";
                cout << endl << "CRC overhead: " << (4 * num_pck) << " byte ";
            }
        }


        if(d_DEBUG)
        {
            cout << endl << endl << "Naive Mulicasting for transmission " << d_id_transmission << " : ";
            cout << endl << "Payload: " << naive_payload_len << " byte ";
            cout << endl << "Header overhead: " << naive_header_len << " byte ";
            cout << endl << "ID small packet overhead: " << (2 * naive_number_small_packet) << " byte ";
            cout << endl << "CRC overhead: " << naive_crc << " byte ";
        }

        //compute total value
        d_tot_naive_payload_len += naive_payload_len;
        d_tot_naive_header_len += naive_header_len;
        d_tot_naive_number_small_packet += naive_number_small_packet;
        d_tot_naive_crc += naive_crc;

        d_tot_byte_naive += naive_payload_len + naive_header_len + (2 * naive_number_small_packet) + naive_crc;


        //Compute Gain for transmission
        payload_gain = (d_payload_len / naive_payload_len)*(100);
        header_gain = (d_header_len / naive_header_len)*(100);
        small_pck_gain  = (d_number_small_packet / naive_number_small_packet)*(100);
        crc_gain = (d_crc / naive_crc)*(100);

        if(d_DEBUG)
        {
            cout << endl << endl << "GAINS for transmission " << d_id_transmission << " : ";
            cout << endl << "Payload Gain: " << payload_gain << " % ";
            cout << endl << "Header Gain: " << header_gain << " % ";
            cout << endl << "ID small packet Gain: " << small_pck_gain << " % ";
            cout << endl << "CRC Gain: " << crc_gain << " % ";
            cout << endl << endl;
        }

        //Compute Average Gain
        d_payload_gain += payload_gain;
        d_header_gain += header_gain;
        d_small_pck_gain += small_pck_gain;
        d_crc_gain += crc_gain;

        d_average_payload_gain = d_payload_gain / d_id_transmission;
        d_average_header_gain = d_header_gain / d_id_transmission;
        d_average_small_pck_gain = d_small_pck_gain / d_id_transmission;
        d_average_crc_gain = d_crc_gain / d_id_transmission;

        if(d_DEBUG)
        {
            cout << endl << endl << "AVERAGE GAINS: ";
            cout << endl << "Payload Gain: " << d_average_payload_gain << " % ";
            cout << endl << "Header Gain: " << d_average_header_gain << " % ";
            cout << endl << "ID small packet Gain: " << d_average_small_pck_gain << " % ";
            cout << endl << "CRC Gain: " << d_average_crc_gain << " % ";
            cout << endl << endl;
        }



        //Compute Total Gain
        d_tot_payload_gain = (d_tot_payload_len / d_tot_naive_payload_len) * (100);
        d_tot_header_gain = (d_tot_header_len / d_tot_naive_header_len) * (100);
        d_tot_small_pck_gain = (d_tot_number_small_packet / d_tot_naive_number_small_packet) * (100);
        d_tot_crc_gain = (d_tot_crc / d_tot_naive_crc) * (100);

        if(d_DEBUG)
        {
            cout << endl << endl << "Total Coded Mulicasting";
            cout << endl << "Payload: " << d_tot_payload_len << " byte ";
            cout << endl << "Header overhead: " << d_tot_header_len << " byte ";
            cout << endl << "ID small packet: " << (2 * d_tot_number_small_packet) << " byte ";
            cout << endl << "CRC overhead: " << d_tot_crc << " byte ";

            cout << endl << endl << "Total Naive Mulicasting ";
            cout << endl << "Payload: " << d_tot_naive_payload_len << " byte ";
            cout << endl << "Header overhead: " << d_tot_naive_header_len << " byte ";
            cout << endl << "ID small packet overhead: " << (2 * d_tot_naive_number_small_packet) << " byte ";
            cout << endl << "CRC overhead: " << d_tot_naive_crc << " byte ";

            cout << endl << endl << "TOTAL GAINS: ";
            cout << endl << "Payload Gain: " << d_tot_payload_gain << " % ";
            cout << endl << "Header Gain: " << d_tot_header_gain << " % ";
            cout << endl << "ID small packet Gain: " << d_tot_small_pck_gain << " % ";
            cout << endl << "CRC Gain: " << d_tot_crc_gain << " % ";
            cout << endl << endl;

            cout << endl << endl << "TOTAL BYTE TRASMITTED: ";
            cout << endl << "Coded Multicasting: " << d_tot_byte_coded << endl;
            cout << endl << "Naive Multicasting: " << d_tot_byte_naive << endl;
            cout << endl << "Coded / (10 * file_size): " << (d_tot_byte_coded / (10 * 337123 ) ) << endl;
            cout << endl << "Naive / Coded: " << (d_tot_byte_naive / d_tot_byte_coded) << endl;
            
            cout << endl << endl;             
        }


    }/* end void computeGain() */


    void gain_bf_impl::cleanVar()
    {
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
        
        d_header_len = 0;
        d_field_len = 0;
        d_payload_len = 0;
        d_number_small_packet = 0;
        d_crc = 0;
        
        d_last_small_packet_len = 0;
        d_id_last_small_packet = 0;

    }/* end void cleanVar() */



    int
    gain_bf_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        float *out_payload_gain = (float *) output_items[0];
        float *out_header_gain = (float *) output_items[1];
        float *out_small_pck_gain = (float *) output_items[2];

        unsigned int i = 0;
        //char id_byte;
        char buff_short[2];
        ofstream stock;

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
            case(0): //Waiting for the id_small_packet = 0
                
                //do nothing
                out_payload_gain[0] = d_average_payload_gain;
                out_header_gain[0] = d_average_header_gain;
                out_small_pck_gain[0] = d_average_small_pck_gain;

                return noutput_items;
                
                break;


            case(1): //Header received

                if(d_DEBUG)
                {
                    d_id_transmission ++;
                    cout << endl << endl << "TRANSMISSION NUMBER : " << d_id_transmission << endl << endl;
                }
                
                decodeHeader(in);
                
                computeGain();

                d_id_expected += 1;

                out_payload_gain[0] = d_average_payload_gain;
                out_header_gain[0] = d_average_header_gain;
                out_small_pck_gain[0] = d_average_small_pck_gain;

                stock.open("gain_results.txt",ios::app);
                stock << d_tot_byte_coded << "\t" << d_tot_byte_naive << endl;
                stock.close();

                return noutput_items;
                
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
                
                d_id_expected += 1;

                out_payload_gain[0] = d_average_payload_gain;
                out_header_gain[0] = d_average_header_gain;
                out_small_pck_gain[0] = d_average_small_pck_gain;

                return noutput_items;
                
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
                
                cleanVar();

                out_payload_gain[0] = d_average_payload_gain;
                out_header_gain[0] = d_average_header_gain;
                out_small_pck_gain[0] = d_average_small_pck_gain;

                return noutput_items;

                break;


            case(4): //Fill part of large packet with dummy bytes

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
                
                    cleanVar();

                
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
                
                    d_id_expected = d_id_small_packet + 1;
                }

                out_payload_gain[0] = d_average_payload_gain;
                out_header_gain[0] = d_average_header_gain;
                out_small_pck_gain[0] = d_average_small_pck_gain;

                return noutput_items;

                break;


            case(5): //Lost the end of the previous large pack, so fill the lack and decode. After check for the next large packet

                cleanVar();

                if(d_id_small_packet == 0)
                {
                    decodeHeader(in);

                    computeGain();
                
                    d_id_expected += 1;

                    /*out_payload_gain[0] = d_average_payload_gain;
                    out_header_gain[0] = d_average_header_gain;
                    out_small_pck_gain[0] = d_average_small_pck_gain;

                    return noutput_items;*/
                
                }else if(d_id_small_packet != 0)
                {
                    //do nothing
                    //Waiting for id_small_packet = 0

                    //return 0;
                }

                out_payload_gain[0] = d_average_payload_gain;
                out_header_gain[0] = d_average_header_gain;
                out_small_pck_gain[0] = d_average_small_pck_gain;

                return noutput_items;

                break;

        }

        // Tell runtime system how many output items we produced.
        //return noutput_items;
    }

  } /* namespace projectCACHE */
} /* namespace gr */

