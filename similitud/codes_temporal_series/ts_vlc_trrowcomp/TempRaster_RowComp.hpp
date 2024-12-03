#ifndef TRRowComp
#define TRRowComp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

// *********************** CLASE TOPORELGST ***********************
//  --------------------Inicio clase TempRasterQuadComp-------------------
class TempRasterQuadComp{
public:
	unsigned int n_rows;		// Filas
	unsigned int n_cols;		// Columnas
	unsigned int n_inst;		// Instantes
	int min_value;
	int max_value;

	vlc_vector<> fixedValue;		//	Primer valor de cada serie fija
	vlc_vector<> refFirstValue;		//	Primer valor de las series de referencia
	vector<vlc_vector<>> refs;		//	Series de Referencias
	vector<vlc_vector<>> series;	//	Series que no son referencia ni fijas
	sd_vector<> bvSeriesFijas;		//	Series Fijas
	sd_vector<>::rank_1_type rankSeriesFijas;

	TempRasterQuadComp(vector<vector<vector<int>>>&, int);
	TempRasterQuadComp(string);

	bool save(string);
	int size_bytes();
	int size_kbytes();
	int size_mbytes();

	vector<int> getSerie(int, int);

	void print();

	int queryAccess(int, int, int, bool=false);
	vector<vector<vector<int>>> queryWindow(int, int, int, int, int, int, bool=false);

private:
	void buildRanksSelects();
	unsigned int zigzag_encode(int);
	int zigzag_decode(int);
	bool esFija(vector<int>);
};

#endif