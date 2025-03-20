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
	
	vlc_vector<coder::fibonacci> pgFirstValue;			//	Primer valor de las referencias de los grupos principales.
	vector<vlc_vector<coder::fibonacci>> pgReference;	//	Series de referencia para series de los grupos principales.
	vector<vlc_vector<coder::fibonacci>> pgSeries;		//	Series de los grupos principales.
	vlc_vector<coder::fibonacci> lgFirstValue;			//	Primer valor de las series del último grupo.
	vector<vlc_vector<coder::fibonacci>> lgSeries;		//	Series del último grupo.
	
	TempSeriesSensoresMadrid(vector<vector<vector<int>>>&, vector<int>&, vector<int>&, int, int);		//	Construcción básica original: codificación respecto de referencia
	TempSeriesSensoresMadrid(vector<vector<vector<int>>>&, vector<int>&, vector<int>&, int, int, bool);	//	Construcción para permitir codificación de series respecto la anterior
	TempSeriesSensoresMadrid(vector<vector<vector<int>>>&, vector<int>&, vector<int>&, int);			//	Construcción para usar serie promedio de grupo como referencia
	TempSeriesSensoresMadrid(string);

	bool save(string);
	int size_bytes();
	int size_kbytes();
	int size_mbytes();

	void print();
	void stats();

private:
	void buildRanksSelects();
	unsigned int zigzag_encode(int);
	int zigzag_decode(int);
	bool esFija(vector<int>);
	void encuentraLimites(vector<vector<vector<int>>>&);
	string statsEV(vlc_vector<>);
};

#endif