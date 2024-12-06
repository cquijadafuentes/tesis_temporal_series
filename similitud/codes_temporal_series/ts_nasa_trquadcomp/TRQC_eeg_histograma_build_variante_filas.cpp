

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

map<int,int> histoVLC;
map<int,int> histoBC;
map<int,int> histoZZ;
map<int,int> histoBIN;

int minimo(vector<int> x){
	int min = x[0];
	for(int i=1; i<x.size(); i++){
		if(x[i] < min){
			min = x[i];
		}
	}
	return min;
}

int minimo(vector<vector<int>> x){
	int min = minimo(x[0]);
	int aux;
	for(int i=1; i<x.size(); i++){
		aux = minimo(x[i]);
		if(aux < min){
			min = aux;
		}
	}
	return min;
}

int sizeBytesVLC(vector<int> x, int min_value){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = x[i] - min_value;
		histoVLC[iv[i]] += 1;
	}
	vlc_vector<> vlcv(iv);
	return size_in_bytes(vlcv);
}

int sizeBytesVLC(vector<vector<int>> x){
	int min = minimo(x);
	int bytes = 0;
	for(int i=0; i<x.size(); i++){
		bytes += sizeBytesVLC(x[i], min);
	}
	return bytes;
}

int sizeBytesBitCompressing(vector<int> x, int min_value){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = x[i] - min_value;
		histoBC[iv[i]] += 1;
	}
	util::bit_compress(iv);
	return size_in_bytes(iv);
}

int sizeBytesBitCompressing(vector<vector<int>> x){
	int min = minimo(x);
	int bytes = 0;
	for(int i=0; i<x.size(); i++){
		bytes += sizeBytesBitCompressing(x[i], min);
	}
	return bytes;
}

bool esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <mapping_sensor_matrix>  <outputFile>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	string ifilename(argv[1]);
	ifstream infile(ifilename, ofstream::binary);
	if(infile.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int electrodos, muestras;
	// Cargando datos
	infile.read((char *)&electrodos, sizeof(int));
	infile.read((char *)&muestras, sizeof(int));

	vector<vector<int>> eegData(electrodos, vector<int>(muestras));
	for(int i=0; i<electrodos; i++){
		for(int k=0; k<muestras; k++){
			infile.read((char *)&eegData[i][k], sizeof(int));
			histoBIN[eegData[i][k]] += 1;
		}
	}

	//	Lectura de matriz con el mapeo de los ids por FILAS
	ifstream eegMappingMatrix(argv[2], ifstream::in);
    if(eegMappingMatrix.fail()){
        cout << "Error! Lectura de " << argv[2] << " fallida." << endl;
        return -1;
    }
    int matrixInputs;
    eegMappingMatrix >> matrixInputs;
    int pos, f, c;
    vector<vector<vector<int>>> tseries(9, vector<vector<int>>(9, vector<int>(muestras, 0)));
    for(int i=0; i<matrixInputs; i++){
        eegMappingMatrix >> pos >> f >> c;
        tseries[f][c] = eegData[pos];
    }
    eegMappingMatrix.close();

	// Construcción del QuadComp para EEG POR FILAS
	int n_rows = tseries.size();			// filas
	int n_cols = tseries[0].size();		// columnas
	int n_inst = tseries[0][0].size();	// length of temporal series
	int min_value = tseries[0][0][0];
	int max_value = tseries[0][0][0];
	for(int i=0; i<n_rows; i++){
		for(int j=0; j<n_cols; j++){
			for(int t=0; t<n_inst; t++){
				if(tseries[i][j][t] < min_value){
					min_value = tseries[i][j][t];
				}
				if(tseries[i][j][t] > max_value){
					max_value = tseries[i][j][t];
				}
			}
		}
	}
	//cout << "min: " << min_value << " - max: " << max_value << endl;
	bit_vector bvSF(81);		// Marcar las series fijas
	vector<vector<int>> series;
	vector<vector<int>> refs;
	vector<int> valoresPVSR;		// Guarda el primer valor de cada serie de referencia
	vector<int> valoresSF;		// Guarda el valor de cada serie fija
	vector<int> valoresPVSRzz;
	vector<int> valoresSFzz;
	int zzaux;
	for(int f=0; f<n_rows; f++){		// fc = fila-cuadrante
		vector<int> serieReferencia = tseries[f][0];
		for(int t=0; t<n_inst; t++){
			serieReferencia[t] = tseries[f][0][t];
		}
		valoresPVSR.push_back(tseries[f][0][0]);
		histoZZ[zigzag_encode(tseries[f][0][0])] += 1;
		bvSF[9*f] = 0;
		int val;
		vector<int> ivaux(n_inst-1);
		vector<int> ivauxZZ(n_inst-1,0);
		for(int k=1; k<n_inst; k++){
			ivaux[k-1] = tseries[f][0][k] - tseries[f][0][k-1];
			histoZZ[zigzag_encode(ivaux[k-1])] += 1;
		}
		refs.push_back(ivaux);
		for(int c=1; c<n_cols; c++){
			int iCelda = 9*f+c;
			if(esFija(tseries[f][c])){
				// La serie de tiempo es fija y no se representa
				valoresSF.push_back(tseries[f][c][0]);
				histoZZ[zigzag_encode(tseries[f][c][0])] += 1;
				bvSF[iCelda] = 1;
			}else{
				ivaux = vector<int>(n_inst,0);
				ivauxZZ = vector<int>(n_inst,0);
				for(int k=0; k<n_inst; k++){
					ivaux[k] = serieReferencia[k] - tseries[f][c][k];
					histoZZ[zigzag_encode(ivaux[k])] += 1;
				}
				series.push_back(ivaux);
				bvSF[iCelda] = 0;
			}
		}
	}

	// Valores Fijos
	int auxmin = minimo(valoresSF);
	sizeBytesVLC(valoresSF, auxmin);
	sizeBytesBitCompressing(valoresSF, auxmin);
	// Primer Valor de Referencias
	auxmin = minimo(valoresPVSR);
	sizeBytesVLC(valoresPVSR, auxmin);
	sizeBytesBitCompressing(valoresPVSR, auxmin);
	// Serie de Referencia
	sizeBytesVLC(refs);
	sizeBytesBitCompressing(refs);
	// Series Referenciadas
	sizeBytesVLC(series);
	sizeBytesBitCompressing(series);



	int minHisto = histoBIN.begin()->first;
	if(histoBC.begin()->first < minHisto){
		minHisto = histoBC.begin()->first;
	}
	if(histoVLC.begin()->first < minHisto){
		minHisto = histoVLC.begin()->first;
	}
	if(histoZZ.begin()->first < minHisto){
		minHisto = histoZZ.begin()->first;
	}

	auto ult = histoBIN.end();
	ult--;
	int maxHisto = ult->first;
	ult = histoBC.end();
	ult--;
	if(ult->first > maxHisto){
		maxHisto = ult->first;
	}
	ult = histoVLC.end();
	ult--;
	if(ult->first > maxHisto){
		maxHisto = ult->first;
	}
	ult = histoZZ.end();
	ult--;
	if(ult->first > maxHisto){
		maxHisto = ult->first;
	}

	ofstream salida(argv[3]);
	if(salida.fail()){
		cout << "Error! Al crear el archivo " << argv[3] << " de salida." << endl;
		return -1;
	}
	salida << "#num\tint\tbc\tvlc\tzz" << endl;
	for(int i=minHisto; i<=maxHisto; i++){
		salida << i << "\t" << histoBIN[i] << "\t" << histoBC[i] << "\t" << histoVLC[i] << "\t" << histoZZ[i] << endl;
	}

	return 0;
}