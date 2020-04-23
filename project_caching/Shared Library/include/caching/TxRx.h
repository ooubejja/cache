#ifndef TxRx_H_INCLUDED
#define TxRx_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <vector>

#include "utilityForTesting.h"
#include "DataDefinition.h"
#include "Conversions.h"
#include "FuncsFromMain.h"
#include "CodingDecodingData.h"
#include "PC.h"

#include <complex.h>

#include <cstdlib>
#include <iterator>
#include <sstream>


typedef std::complex<float> gr_complex;

namespace caching{

void TX_PC_Pack(vector<header_polar> &, vector< vector<char> > coded_data, unsigned int , vector<vector<char> > &, int, vector<int> &);//vector<char> &

vector<char> Process_Data(vector<gr_complex> , int , unsigned int &, int , int , bool , int , int , int , double , PC, PC, header_polar);

vector<vector<gr_complex> > BitsToQPSKSymb(vector<vector<int> >);

/*
othmane :
*/
std::string execute( std::string cmd );

std::string LoadFile(const char* FileLocation);

int find_index(vector<unsigned int>, int);

void mycompare(string, string, int);

// othmane
// void decodeDataStrong(int N,int K_w,int K_s,double d_SNR, gr_complex* coded_symb,string ,PC ,PC, bool ,unsigned int & );
vector<char> decodeDataStrong(int N,int K_w,int K_s,double d_SNR, vector<gr_complex> coded_symb,string ,PC ,PC, bool ,unsigned int & );

void toHeaderWeak(header_polar , header_transmission &header);

void Transmit(header_transmission* header_data, vector< vector<char> > coded_data, unsigned int id_demand, int n_col, unsigned int small_packet_len, vector<char> &trasmissione, vector<unsigned int> &small_pack_size);

void Receive(header_transmission &header, vector<char> &coded_data, unsigned int small_packet_len, FILE *RX_file);

void write_byte(vector<char> trasmissione);

}//end namespace caching

#endif // TxRx_H_INCLUDED
