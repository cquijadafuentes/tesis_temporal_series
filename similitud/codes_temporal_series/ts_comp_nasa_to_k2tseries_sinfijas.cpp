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
	No se codifican las series fijas.
*/

int zigzag_encode(int i){
   return ((i >> 31) ^ (i << 1));
}

int zigzag_decode(int i){
    return ((i >> 1) ^ -(i & 1));
}

long long int bytes_int_vector(vector<int> x){
	int_vector<> iv_x(x.size());
	for(int i=0; i<x.size(); i++){
		iv_x[i] = x[i];
	}
	util::bit_compress(iv_x);
	return size_in_bytes(iv_x);
}

bool esFija(vector<int> serie){
	bool fija = true;
	int val;
	for(int j=1; j<serie.size(); j++){
		val = serie[j] - serie[j-1];
		fija = fija && (val == 0);
	}
	return fija;
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
	

	bit_vector bvSF = bit_vector(rows * cols);			// Marcar las series fijas
	bit_vector bvSR = bit_vector(cuadRows * cuadCols);	// Marcar cuadrantes con valor inicial
	vector<int> valoresFV;		// Guarda el primer valor de cada serie de referencia
	vector<int> valoresSF;		// Guarda el valor de cada serie fija
	int iFV = 0;
	long long int bytesTodas = 0;
	for(int fc=0; fc<cuadRows; fc++){		// fc = fila-cuadrante
		for(int cc=0; cc<cuadCols; cc++){	// cc = columna-cuadrante
			// Procesando primera series del cuadrante
			int fc0 = fc*dcuad;
			int cc0 = cc*dcuad;
			int iCuad = fc0 * cuadRows + cc0;
			vector<int> serieReferencia;
			// Procesando el resto de las series del cuadrante
			for(int df=0; df<dcuad && fc0+df < rows; df++){		// delta-filas
				for(int dc=0; dc<dcuad && cc0+dc < cols; dc++){		// delta-columnas
					int posF = fc0+df;
					int posC = cc0+dc;
					int val;
					if(esFija(temporalSeries[posF][posC])){
						// La serie de tiempo es fija y no se representa
						int iRaster = ((posF) * rows) + (posC);
						bvSF[iRaster] = 1;
						valoresSF.push_back(temporalSeries[posF][posC][0]);
					}else if(bvSR[iCuad] == 0){
						// NO hay serie de referencia
						serieReferencia = temporalSeries[posF][posC];
						valoresFV.push_back(temporalSeries[posF][posC][0]);
						int_vector<> ivaux(lenTempSerie-1);
						for(int k=1; k<lenTempSerie; k++){
							val = temporalSeries[fc0][cc0][k] - temporalSeries[fc0][cc0][k-1];
							ivaux[k-1] = zigzag_encode(val);
						}
						util::bit_compress(ivaux);
						bytesTodas += size_in_bytes(ivaux);
					}else{
						// SI hay serie de referencia
						int_vector<> ivaux2(lenTempSerie);
						for(int k=0; k<lenTempSerie; k++){
							val = serieReferencia[k] - temporalSeries[posF][posC][k];
							ivaux2[k] = zigzag_encode(val);
						}
						util::bit_compress(ivaux2);
						bytesTodas += size_in_bytes(ivaux2);
					}
				}
			}
		}
	}
	bytesTodas += bytes_int_vector(valoresFV);
	bytesTodas += bytes_int_vector(valoresSF);
	int megaBytes = ((bytesTodas + 0.0) / 1024) / 1024;

	cout << argv[1] << "\t" << megaBytes << "\t" << dcuad << "\t" << cuadRows << "\t" << cuadCols << endl;
}
