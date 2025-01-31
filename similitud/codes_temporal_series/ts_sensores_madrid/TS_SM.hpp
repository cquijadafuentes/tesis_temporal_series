#ifndef TSSM
#define TSSM

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

//  -------------------- Inicio clase Temporal Series - Sensores Madrid -------------------
class TempSeriesSensoresMadrid{
public:
	unsigned int n_sensores;	// Número de sensores
	vector<int> n_sens_group;	// Número de sensores por grupo
	unsigned int k;				// Frecuencia de los referentes
	
	int min_value;
	int max_value;
	vector<int_vector<>> refFirstValue;		//	Primer valor de las series de referencia
	vector<int_vector<>> refs;		//	Series de Referencias
	vector<int_vector<>> series;	//	Series que no son referencia ni fijas
	
	TempSeriesSensoresMadrid(vector<vector<int>>&, vector<int>&, int);
	TempSeriesSensoresMadrid(string);

	bool save(string);
	int size_bytes();
	int size_kbytes();
	int size_mbytes();

	void print();

private:
	void buildRanksSelects();
	unsigned int zigzag_encode(int);
	int zigzag_decode(int);
	bool esFija(vector<int>);
};

#endif