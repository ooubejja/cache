//============================================================================
// Name        : PolarCodes_2users_v2.cpp
// Author      : Sarah Kamel
// Version     : 23/7/2018
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cmath>
#include <time.h>
#include <random>
#include "PC.h"
using namespace std;

int main() {
	cout << "Polar codes" << endl; // prints Polar codes

	srand (time(NULL));

	PC PC_w, PC_s;

	int N =2048; // length of the codeword
	float designSNRdb = 0;

	// Weak user
	int K_w = 810; // dimension of the weak receiver codeword
	float SNR_w = 1.5;
	float variance_w = pow(10,-(SNR_w/10));
	float sqrtVariance_w = sqrt(variance_w);
	PC_w.constructPC(N, K_w, designSNRdb);

	int * info_w = new int[K_w];
	int * frozen_w = new int[N-K_w];
	double * llr_w = new double[N];
	float * recSymbol_w = new float[N];
	int * recMessage_w = new int[N];
	int * recCodeword_w = new int[N];


/*
 	int * sentMessage = new int[N];
	int * sentCodeword = new int[N];
	int * sentSymbol = new int[N];


	for (int i=0; i<K_w; i++)
		info_w[i] = rand()%2;
	for (int i=K_w; i<N; i++)
		frozen_w[i-K_w] = rand()%2;


	PC_w.encode(info_w, frozen_w, sentMessage, sentCodeword, sentSymbol);
	PC_w.noise(sentSymbol, recSymbol_w, sqrtVariance_w);
	PC_w.computeLLR(llr_w,recSymbol_w,variance_w);
	PC_w.SC(recMessage_w, recCodeword_w, llr_w, sentMessage);

	PC_w.printGenMatrix();
	PC_w.printArrangedBits();
	cout << "info_w =   ";
	PC_w.printVectorInt(info_w,K_w);
	cout << "frozen_w =   ";
	PC_w.printVectorInt(frozen_w,N-K_w);

	cout << endl << "sentMessage =   ";
	PC_w.printVectorInt(sentMessage,N);
	cout << "sentCodeword =  ";
	PC_w.printVectorInt(sentCodeword,N);

	cout << endl << "recMessage_w =  ";
	PC_w.printVectorInt(recMessage_w,N);
	cout << "recCodeword_w = ";
	PC_w.printVectorInt(recCodeword_w,N);
	*/



	int cwErrorSum_w = 0;
	int bitErrorSum_w = 0;
	int error_w;


	// Strong user
	int K_s = 1024; // dimension of the strong receiver codeword
	float SNR_s = 2.8;
	float variance_s = pow(10,-(SNR_s/10));
	float sqrtVariance_s = sqrt(variance_s);

	PC_s.initPC(N, K_s, designSNRdb);
	PC_s.setGenMatrix(PC_w.genMatrix);
	PC_s.setRn(PC_w.Rn);
	PC_s.setArrangedBits(PC_w.arragedBits);


	int * info_s = new int[K_s];
	int * frozen_s = new int[N-K_s];
	double * llr_s = new double[N];
	float * recSymbol_s = new float[N];
	int * recMessage_s = new int[N];
	int * recCodeword_s = new int[N];

	int cwErrorSum_s = 0;
	int bitErrorSum_s = 0;
	int error_s;

	// Same for both users
	int * initialMessage = new int[N];
	int * sentMessage = new int[N];
	int * sentCodeword = new int[N];
	int * sentSymbol = new int[N];


	int tirageMax = 1000;

	for (int tirage = 1; tirage <= tirageMax; tirage++)
	{
		error_w = 0;
		error_s = 0;


		for (int i=0; i<K_w; i++)
			initialMessage[i] = rand()%2;
		for (int i=K_w; i<N; i++)
			initialMessage[i] = 0;

		for (int i=0; i<K_w; i++)
			info_w[i] = initialMessage[i];
		for (int i=K_w; i<N; i++)
			frozen_w[i-K_w] = initialMessage[i];

		for (int i=0; i<K_s; i++)
			info_s[i] = initialMessage[i];
		for (int i=K_s; i<N; i++)
			frozen_s[i-K_s] = initialMessage[i];


		PC_w.encode(info_w, frozen_w, sentMessage, sentCodeword, sentSymbol);


		// Weak receiver channel + decoding
		PC_w.noise(sentSymbol, recSymbol_w, sqrtVariance_w);
		PC_w.computeLLR(llr_w,recSymbol_w,variance_w);
		PC_w.SC(recMessage_w, recCodeword_w, llr_w, frozen_w);


		for (int i=0; i<N; i++)
			if (recMessage_w[i] != initialMessage[i])
				error_w = error_w + 1;

		if (error_w != 0)
			cwErrorSum_w = cwErrorSum_w + 1;

		bitErrorSum_w = bitErrorSum_w + error_w;

		// Strong receiver channel + decoding
		PC_s.noise(sentSymbol, recSymbol_s, sqrtVariance_s);
		PC_s.computeLLR(llr_s,recSymbol_s,variance_s);
		PC_s.SC(recMessage_s, recCodeword_s, llr_s, frozen_s);

		for (int i=0; i<N; i++)
		{
			if (recMessage_s[i] != initialMessage[i])
				error_s = error_s + 1;
		}

		if (error_s != 0)
			cwErrorSum_s = cwErrorSum_s + 1;

		bitErrorSum_s = bitErrorSum_s + error_s;

	}

	float CER_w = float(cwErrorSum_w)/float(tirageMax);
	float BER_w = float(bitErrorSum_w)/float(tirageMax*N);
	float CER_s = float(cwErrorSum_s)/float(tirageMax);
	float BER_s = float(bitErrorSum_s)/float(tirageMax*N);

	cout << endl << endl;
	cout << "Weak user : Polar code (" << N << "," << K_w << ")" << endl;
	cout << "SNR = " << SNR_w << ", Variance = " << variance_w << ", Sqrt Variance = " << sqrtVariance_w << endl;
	cout << "Number of trials = " << tirageMax << endl;
	cout << "Number of error per codeword = " << cwErrorSum_w << endl; 	// CER
	cout << "Number of error per bit = " << bitErrorSum_w << endl;		// BER
	cout << "CER = " << CER_w << endl; 	// CER
	cout << "BER = " << BER_w << endl; 	// BER

	cout << endl;
	cout << "Strong user : Polar code (" << N << "," << K_s << ")" << endl;
	cout << "SNR = " << SNR_s << ", Variance = " << variance_s << ", Sqrt Variance = " << sqrtVariance_s << endl;
	cout << "Number of trials = " << tirageMax << endl;
	cout << "Number of error per codeword = " << cwErrorSum_s << endl; 	// CER
	cout << "Number of error per bit = " << bitErrorSum_s << endl;		// BER
	cout << "CER = " << CER_s << endl; 	// CER
	cout << "BER = " << BER_s << endl; 	// BER


	return 0;

}
