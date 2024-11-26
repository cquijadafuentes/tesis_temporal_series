#ifndef TRQuadComp
#define TRQuadComp

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
	unsigned int d_quad;		// Dimensión del cuadrante
	unsigned int cel_per_quad;	// Celdas por cuadrante
	unsigned int nQuadRows;	// Cantidad de cuadrantes en filas
	unsigned int nQuadCols;	// Cantidad de cuadrantes en columnas
	int min_value;
	int max_value;

	vlc_vector<> fixedValue;		//	Primer valor de cada serie fija
	vlc_vector<> refFirstValue;		//	Primer valor de las series de referencia
	vector<vlc_vector<>> refs;		//	Series de Referencias
	vector<vlc_vector<>> series;	//	Series que no son referencia ni fijas
	sd_vector<> bvQuadNoFijos;		//	Cuadrantes no fijos
	sd_vector<> bvReferencias;		//	Referencias de cuadrantes no fijos
	sd_vector<> bvSeriesFijas;		//	Series Fijas
	sd_vector<>::rank_1_type rankQuadNoFijos;
	sd_vector<>::rank_1_type rankReferencias;
	sd_vector<>::rank_1_type rankSeriesFijas;
	sd_vector<>::select_1_type selectReferencias;

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
	unsigned int getQuad(int, int);
	unsigned int getQuadLinealPosition(int, int);
	vector<int> getQuadReferenceSerie(int);
	unsigned int getSeriePositionFromQLP(int, int, int);
};

#endif