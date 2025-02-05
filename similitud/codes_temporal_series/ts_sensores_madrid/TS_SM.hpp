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
	unsigned int num_sensores;	//	Número de sensores.
	unsigned int num_groups;	//	Número de grupos.
	unsigned int num_muestras;	//	Número de muestras del conjunto.
	unsigned int k;				//	Frecuencia de las series de referencia.
	
	int min_value;
	int max_value;
	vector<int> sens_x_group;	//	Cantidad de sensores por grupo.
	vector<int> refs_of_group;	//	Cantidad de referencias por grupo.
	
	int_vector<> pgFirstValue;			//	Primer valor de las referencias de los grupos principales.
	vector<int_vector<>> pgReference;	//	Series de referencia para series de los grupos principales.
	vector<int_vector<>> pgSeries;		//	Series de los grupos principales.
	int_vector<> lgFirstValue;			//	Primer valor de las series del último grupo.
	vector<int_vector<>> lgSeries;		//	Series del último grupo.
	
	TempSeriesSensoresMadrid(vector<vector<vector<int>>>&, vector<int>&, vector<int>&, int, int);
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