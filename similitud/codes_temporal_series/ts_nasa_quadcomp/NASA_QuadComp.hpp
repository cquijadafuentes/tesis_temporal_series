#ifndef QuadCompNasa
#define QuadCompNasa

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

// *********************** CLASE TOPORELGST ***********************
//  --------------------Inicio clase NASAQuadComp-------------------
class NASAQuadComp{
public:
	int n_rows;		// Filas
	int n_cols;		// Columnas
	int n_inst;		// Instantes
	int d_quad;		// Dimensión del cuadrante
	int nQuadRows;	// Cantidad de cuadrantes en filas
	int nQuadCols;	// Cantidad de cuadrantes en columnas
	int min_value;
	int max_value;

	int_vector<> fixedValue;		//	Primer valor de cada serie fija
	int_vector<> refFirstValue;		//	Primer valor de las series de referencia
	vector<int_vector<>> refs;		//	Series de Referencias
	vector<int_vector<>> series;	//	Series que no son referencia ni fijas
	sd_vector<> bvQuadNoFijos;		//	Cuadrantes no fijos
	sd_vector<> bvReferencias;		//	Referencias de cuadrantes no fijos
	sd_vector<> bvSeriesFijas;		//	Series Fijas

	NASAQuadComp(vector<vector<vector<int>>>&, int);

private:
	unsigned int zigzag_encode(int);
	unsigned int zigzag_decode(int);
	bool esFija(vector<int>);
	unsigned int getQuad(int, int);
	unsigned int getQuadLinealPosition(int, int);
};

#endif