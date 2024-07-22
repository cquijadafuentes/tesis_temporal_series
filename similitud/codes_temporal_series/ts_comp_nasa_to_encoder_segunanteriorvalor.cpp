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
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile>" << endl;
		return 0;
	}
    // Leyendo datos desde el archivo de entrada
    ifstream infile(argv[1], ofstream::binary);
    if(infile.fail()){
        cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
        return -1;
    }
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

	int_vector<> firstValue(rows * cols);
	int iFV = 0;
	int cantFijas = 0;
	int maximo = temporalSeries[0][0][0];
	int minimo = temporalSeries[0][0][0];
	long long int bytesTodas = 0;
	long long int bytesNoFijas = 0;
	for(int f=0; f<rows; f++){
		for(int c=0; c<cols; c++){
			int val = temporalSeries[f][c][0];
			firstValue[iFV++] = zigzag_encode(val);
			int_vector<> ivaux(lenTempSerie-1);
			bool fija = true;
			for(int j=1; j<lenTempSerie; j++){
				if(temporalSeries[f][c][j] > maximo){
					maximo = temporalSeries[f][c][j];
				}
				if(temporalSeries[f][c][j] < minimo){
					minimo = temporalSeries[f][c][j];
				}
				val = temporalSeries[f][c][j] - temporalSeries[f][c][j-1];
				ivaux[j-1] = zigzag_encode(val);
				fija = fija && (val == 0);
			}
			util::bit_compress(ivaux);
			bytesTodas += size_in_bytes(ivaux);
			if(!fija){
				bytesNoFijas += size_in_bytes(ivaux);
			}else{
				cantFijas++;
			}
		}
	}
	util::bit_compress(firstValue);
	bytesTodas += size_in_bytes(firstValue);
	bytesNoFijas += size_in_bytes(firstValue);


	cout << "Rango de valores: [" << minimo << " , " << maximo << "]" << endl;
	cout << argv[1] << "\t" << bytesTodas << "\t" << bytesNoFijas << "\t" << cantFijas << endl;
}
