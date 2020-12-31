/* -*- c++ -*- */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CheckFunction.h"
#include "EnvironmentSetup.h"
#include "DecodingInput.h"
#include "ConflictGraph.h"
#include "randomHandler.h"
#include "DataDefinition.h"
#include "grasp.h"
#include "CodingDecodingData.h"
#include "FuncsFromMain.h"
#include "TxRx.h"
//#include "TestTxRx.h"

using namespace caching;
using namespace std;


int main()
{
    int d_m_files = 20;
    int d_b_chunks = 200;
    int d_id_demand = 0;
    int nb_strg = 1;
    int id_user = 2;
    int d_spack_len = 52;
    data_matrix d_data;
    cf_data d_outputForColoring;
    bool d_GRASP=true;
    int d_n_col=0;
    int *d_coloring;
    bool DEBUG = true;
    PC PC_w, PC_s;
    int K_w=70*8, K_s=K_w*2, N=2048;
    float designSNRdb = 0;
    unsigned int packet_remain;
    double SNR=20;
    bool isStr;

    header_transmission *d_header_data;
    header_transmission *d_hdr_sdata;

    vector<int> d_spack_size;
    vector< vector<char> > d_coded_data;
    vector< vector<char> > d_strg_data;
    vector< vector<char> > d_PC_data;
    vector<header_polar> d_hX; //this is the main header to be added directly to the transmitted packets
    //vector<char> d_transmission;
    vector<vector<char> > d_transmission;
    //vector<unsigned int> d_small_packet_size;
    std::vector<int> coderate {1, 2, 3, 4};

    cout << endl << "Data generation process" << endl << "-------------" << endl << endl;

    d_data = generateData(d_m_files, d_b_chunks, d_id_demand);
    int nb_users = d_data.n_utenti;
    /*for(int i=0; i<nb_users; i++){
        for(int j=0; j<d_b_chunks; j++)
            cout << d_data.Q_chuncks[i][j]  << ", ";
        cout << endl << endl;
    } */


    cout << endl << "Conflict-Graph generator process" << endl << "-------------" << endl << endl;
    d_outputForColoring = conflictGraphGenerator(d_data, coderate);
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
        //Coding data to be transmitted for weak users - xor coding
        d_coded_data = codingVarCodeRate_Ref(d_coloring, d_n_col, d_data, d_outputForColoring, &d_header_data,coderate);
        //d_coded_data = codingData(d_coloring, d_n_col, d_data, d_outputForColoring, &d_header_data);

        //Build the schema (graph) packets for strong and weak users
        vector<vector<bool> > G_edges;
        //bool **G_edges;
        d_strg_data = MaxBipartiteGraph(d_coloring, d_n_col, d_outputForColoring.nodes, 
            d_outputForColoring.n_nodi, nb_strg, d_data, &d_hdr_sdata, G_edges);
        
        G_edges = vector<vector<bool> > ();

        for(int i=0; i<d_coded_data.size(); i++){
            vector<bool> temp(d_strg_data.size(), false);
            G_edges.push_back(temp);
        }

        //Coding polarly the weak and strong packets
        vector<vector<int> >  data_bits;
        d_PC_data = codingDataPolar(d_coded_data, d_strg_data, data_bits, G_edges, d_header_data, d_hdr_sdata, d_hX, N);
        cout << "The total number of transmitted packet is: " << d_PC_data.size() << endl;

        //Pack the data and attach the header to be ready for transmission
/*        TX_PC_Pack(d_hX, d_PC_data, d_id_demand, d_transmission, d_spack_len, d_spack_size);
        
        int TXdata_size = d_transmission.size();
        //vector<int> d_data_bits (TXdata_size*8, 0);
        vector<vector<int> > d_data_bits;

        /********Convert a stream of chars to stream of bits***************/
/*        std::vector<unsigned int> bb (8,0);
        for(int i=0; i<TXdata_size; i++){
            int packetSize = d_transmission[i].size();
            d_data_bits.push_back(vector<int>(8*packetSize, 0));

            for(int j=0; j<packetSize; j++){
                bb = conv_char_to_bitsInt(d_transmission[i][j]);
                for(int k=0; k<8; k++){
                    d_data_bits[i][j*8+k] = bb[k];
                }
            }
        }


        /********Pack 1 bit into m bit per byte***************/
        //vector<gr_complex> d_data_Symb;
/*        vector<vector<gr_complex> > d_data_Symb;
        d_data_Symb = BitsToQPSKSymb(d_data_bits);
        
        PC_w = initialize_PC(N,K_w);
        PC_s.initPC(N, K_s, designSNRdb);
        PC_s.setGenMatrix(PC_w.genMatrix);
        PC_s.setRn(PC_w.Rn);
        PC_s.setArrangedBits(PC_w.arragedBits);

        
        if(id_user < d_data.n_utenti)
            packet_remain = readCacheInfo(id_user, 0);
        else 
            packet_remain = d_b_chunks;
        
        if ((d_data.n_utenti-1) < id_user)
            isStr = true;
        else
            isStr = false;
        
        int nbPacket = d_data_Symb.size();
       for(int i=0; i<nbPacket; i++){//nbPacket
            int packetSize = d_data_Symb[i].size();
            gr_complex v[packetSize];
            for(int j=0; j<packetSize; j++)
                v[j] = d_data_Symb[i][j];
            //RX_PC_Pack(v, packet_remain, id_user, PC_w, PC_s);//d_PC_data[i]
            RX_PC_Pack(v, packet_remain, id_user, PC_w, PC_s, d_b_chunks, d_m_files, K_w, K_s, N, SNR, isStr,i,data_bits[i]);
        }

    */
    }/* end if (d_n_col > 0) */
    
    return 0;

}

