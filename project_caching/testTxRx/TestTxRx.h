#ifndef TESTTXRX_H_INCLUDED
#define TESTTXRX_H_INCLUDED

#include "DataDefinition.h"
#include "utilityForTesting.h"

using namespace std;
namespace caching{

      bool d_GRASP;
      unsigned int nb_strg;
      unsigned int d_m_files;
      unsigned int d_b_chunks;
      unsigned int d_id_demand;


      data_matrix d_data;
      cf_data d_outputForColoring;
      int *d_coloring;
      int d_n_col;
      unsigned int d_total_small_packet_size;

      header_transmission *d_header_data;
      header_transmission *d_hdr_sdata;
      vector< vector<char> > d_coded_data;
      vector< vector<char> > d_strg_data;
      vector< vector<char> > d_PC_data;
      vector<header_polar> d_hX;
      vector<char> d_transmission;

      /*data_matrix generateData(int d_m_files, int d_b_chunks, int d_id_demand);
      cf_data conflictGraphGenerator(data_matrix d_data);
      int* graspGraphColoring(int d_maxIter, cf_data d_outputForColoring, int &d_n_col);
      vector< vector<char>>  codingData(int *d_coloring, int d_n_col, data_matrix d_data, cf_data d_outputForColoring, header_transmission *d_header_data);
      vector< vector<char>> MaxBipartiteGraph(int *d_coloring, int d_n_col, nodo *nodes, 
                int n_nodi, int nb_strg, data_matrix d_data, header_transmission *d_hdr_sdata, vector<vector<bool> >&G_edges);
      void TX_PC_Pack(vector<header_polar> &, vector< vector<char> > , unsigned int , vector<char> &, vector<unsigned int> &);*/
}//end caching namespace
#endif
