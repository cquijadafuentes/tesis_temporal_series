#ifndef TSSMFACTOR
#define TSSMFACTOR

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

//  -------------------- Inicio clase Temporal Series - Sensores Madrid -------------------
class TempSeriesSensoresMadridFactor{
public:
	unsigned int num_sensores;	//	Número de sensores.
	unsigned int num_groups;	//	Número de grupos.
	unsigned int num_muestras;	//	Número de muestras del conjunto.
	
	int min_value;
	int max_value;
	int min_id;
	vector<int> sens_x_group;	//	Cantidad de sensores por grupo.
	vlc_vector<coder::fibonacci> ids_sens;		//	Lista de los IDs de los sensores.
	
	vector<vlc_vector<coder::fibonacci>> refs;		//	Referencias correspondientes a promedio de semanas por grupo
	vector<vlc_vector<coder::fibonacci>> series;	//	Series codificadas según la semana correspondiente
	vlc_vector<coder::fibonacci> factores;			//	Factores que se usan en cada serie
	
	TempSeriesSensoresMadridFactor(vector<vector<vector<int>>>&, vector<int>&, vector<int>&, int, bool);
	TempSeriesSensoresMadridFactor(string);

	vector<int> getSerieByID(int);

	bool save(string);
	int size_bytes();
	int size_kbytes();
	int size_mbytes();

	void print();
	void stats(bool=false);

private:
	void buildRanksSelects();
	unsigned int zigzag_encode(int);
	int zigzag_decode(int);
	bool esFija(vector<int>);
	void encuentraLimites(vector<vector<vector<int>>>&);
	string statsEV(vlc_vector<>);
};

#endif