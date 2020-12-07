#ifndef CODINGDECODINGDATA_H_INCLUDED
#define CODINGDECODINGDATA_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <algorithm>
#include "utilityForTesting.h"
#include "DataDefinition.h"
#include "Conversions.h"
#include "FuncsFromMain.h"
#include "PC.h"
#include "EnvironmentSetup.h"

namespace caching{

using namespace std;

bool bpm(vector<vector<bool> > bpGraph, int u, bool seen[], int matchR[]);

vector<vector<bool> > maxBPM(vector<vector<bool> > bpGraph);

void gen_rand_request_zipf(int , double , vector<int> & );

vector< vector<char> > MaxBipartiteGraph(int *d_coloring, int d_n_col, nodo *nodes, 
                int n_nodi, int nb_strg, data_matrix d_data, header_transmission **d_hdr_sdata, vector<vector<bool> >& );

vector<vector<char> > codingDataPolar(vector<vector<char> > weak_data, vector<vector<char> > strg_data, vector<vector<int> > &,
    vector<vector<bool> > G_edges, header_transmission *hdr_weak, header_transmission *hdr_strg, vector<header_polar> &, const int);

PC initialize_PC(int ,int);

vector< vector<char> > codingVarCodeRate(int *coloring, int , data_matrix , cf_data , header_transmission **header_data, vector<int>);

void decodingData(header_transmission , vector<char> &coded_data, unsigned int , unsigned int , unsigned int , unsigned int , unsigned int , int, unsigned int *n_package_remains);

}//end namespace caching

#endif // CODINGDECODINGDATA_H_INCLUDED
