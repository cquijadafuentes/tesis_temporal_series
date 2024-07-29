#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

/*
	Se codifican las series de tiempo según el valor anterior.
*/

int zigzag_encode(int i){
   return ((i >> 31) ^ (i << 1));
}

int zigzag_decode(int i){
    return ((i >> 1) ^ -(i & 1));
}

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <d_cuadrante>" << endl;
		return 0;
	}
    // Leyendo datos desde el archivo de entrada
    ifstream infile(argv[1], ofstream::binary);
    if(infile.fail()){
        cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
        return -1;
    }
    int dcuad = stoi(argv[2]);
    int rows, cols, lenTempSerie;
    // Cargando datos
    infile.read((char *)&rows, sizeof(int));
    infile.read((char *)&cols, sizeof(int));
    infile.read((char *)&lenTempSerie, sizeof(int));

    cout << "File: " << argv[1] << endl;
    cout << "Filas: " << rows << " - Cols: " << cols << " - Muestras: " << lenTempSerie << endl;

    vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols, vector<int>(lenTempSerie)));
    for(int i=0; i<rows; i++){
    	for(int j=0; j<cols; j++){
    		for(int k=0; k<lenTempSerie; k++){
                infile.read((char *)&temporalSeries[i][j][k], sizeof(int));
            }
    	}        
    }
	
	// Cantidad de filas y columnas de cuadrantes según las dimensiones dcuad.
	int cuadRows = rows / dcuad;
	if(cuadRows*dcuad < rows){
		cuadRows++;
	}
	int cuadCols = cols / dcuad;
	if(cuadCols*dcuad < cols){
		cuadCols++;
	}
	

	int_vector<> firstValue(cuadRows * cuadCols);
	int iFV = 0;
	long long int bytesTodas = 0;
	for(int fc=0; fc<cuadRows; fc++){		// fc = fila-cuadrante
		for(int cc=0; cc<cuadCols; cc++){	// cc = columna-cuadrante
			// Procesando primera series del cuadrante
			int fref = fc*dcuad;
			int cref = cc*dcuad;
			int val = temporalSeries[fref][cref][0];
			firstValue[iFV++] = zigzag_encode(val);
			int_vector<> ivaux(lenTempSerie-1);
			for(int k=1; k<lenTempSerie; k++){
				val = temporalSeries[fref][cref][k] - temporalSeries[fref][cref][k-1];
				ivaux[k-1] = zigzag_encode(val);
			}
			util::bit_compress(ivaux);
			bytesTodas += size_in_bytes(ivaux);
			// Procesando el resto de las series del cuadrante
			for(int i=0; i<dcuad && fref+i < rows; i++){
				int_vector<> ivaux2(lenTempSerie);
				for(int j=0; j<dcuad && cref+j < cols; j++){
					if(i!=0 || j!=0){
						// NO es la primera celda (que ya esta considerada)
						for(int k=0; k<lenTempSerie; k++){
							val = temporalSeries[fref][cref][k] - temporalSeries[fref+i][cref+j][k];
							ivaux2[k] = zigzag_encode(val);
						}
						util::bit_compress(ivaux2);
						bytesTodas += size_in_bytes(ivaux2);
					}
				}
			}
		}
	}
	util::bit_compress(firstValue);
	bytesTodas += size_in_bytes(firstValue);

	cout << argv[1] << "\t" << bytesTodas << "\t" << dcuad << "\t" << cuadRows << "\t" << cuadCols << endl;
}
