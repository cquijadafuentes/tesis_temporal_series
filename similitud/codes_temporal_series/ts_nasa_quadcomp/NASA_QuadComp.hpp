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
	int n_rows;     // Filas
	int n_cols;     // Columnas
	int n_inst;     // Instantes
	int dcuadl;     // Dimensión del cuadrante
	int nCuadRows;	// Cantidad de cuadrantes en filas
	int nCuadCols;	// Cantidad de cuadrantes en columnas
	int min_value;
	int max_value;

	int_vector<> firstValue;       //

	NASAQuadComp(vector<vector<vector<int>>>, int);

private:
	unsigned int zigzag_encode(int);
	unsigned int zigzag_decode(int);
};

#endif