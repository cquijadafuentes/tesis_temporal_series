#include "TempRaster_QuadComp.hpp"

using namespace std;
using namespace sdsl;

int sizeBytesVLC(vector<int> x, int min_value){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = x[i] - min_value;
	}
	vlc_vector<> vlcv(iv);

	return size_in_bytes(vlcv);
}

int sizeBytesVLC(vector<vector<int>> x, int min_value){
	int bytes = 0;
	for(int i=0; i<x.size(); i++){
		bytes += sizeBytesVLC(x[i], min_value);
	}
	return bytes;
}

int sizeBytesBitCompressing(vector<int> x, int min_value){
	int_vector<> iv(x.size());
	for(int i=0; i<x.size(); i++){
		iv[i] = x[i] - min_value;
	}
	util::bit_compress(iv);
	return size_in_bytes(iv);
}

int sizeBytesBitCompressing(vector<vector<int>> x, int min_value){
	int bytes = 0;
	for(int i=0; i<x.size(); i++){
		bytes += sizeBytesBitCompressing(x[i], min_value);
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
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <mapping_sensor_matrix>" << endl;
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
	bit_vector bvSF(81);		// Marcar las series fijas
	vector<vector<int>> series;
	vector<vector<int>> refs;
	vector<int> valoresPVSR;		// Guarda el primer valor de cada serie de referencia
	vector<int> valoresSF;		// Guarda el valor de cada serie fija
	int iFV = 0;
	for(int f=0; f<n_rows; f++){		// fc = fila-cuadrante
		vector<int> serieReferencia = tseries[f][0];
		for(int t=0; t<n_inst; t++){
			serieReferencia[t] = tseries[f][0][t];
		}
		valoresPVSR.push_back(tseries[f][0][0]);
		bvSF[9*f] = 0;
		int val;
		vector<int> ivaux(n_inst-1);
		for(int k=1; k<n_inst; k++){
			val = tseries[f][0][k] - tseries[f][0][k-1];
			ivaux[k-1] = zigzag_encode(val);
		}
		refs.push_back(ivaux);
		for(int c=1; c<n_cols; c++){
			int iCelda = 9*f+c;
			if(esFija(tseries[f][c])){
				// La serie de tiempo es fija y no se representa
				valoresSF.push_back(tseries[f][c][0]);
				bvSF[iCelda] = 1;
			}else{
				vector<int> ivaux2(n_inst);
				for(int k=0; k<n_inst; k++){
					val = serieReferencia[k] - tseries[f][c][k];
					ivaux2[k] = zigzag_encode(val);
				}
				series.push_back(ivaux2);
				bvSF[iCelda] = 0;
			}
		}
	}

	// CALCULANDO TAMAÑO
	int bytesVLCV = 0;
	int bytesBC = 0;
	// Valores enteros
	bytesVLCV += (sizeof(int) * 5);
	bytesBC += (sizeof(int) * 5);
	// Valores Fijos
	bytesVLCV += sizeBytesVLC(valoresSF, min_value);
	bytesBC += sizeBytesBitCompressing(valoresSF, min_value);
	// Primer Valor de Referencias
	bytesVLCV += sizeBytesVLC(valoresPVSR, min_value);
	bytesBC += sizeBytesBitCompressing(valoresPVSR, min_value);
	// Serie de Referencia
	bytesVLCV += sizeBytesVLC(refs, min_value);
	bytesBC += sizeBytesBitCompressing(refs, min_value);
	// Series Referenciadas
	bytesVLCV += sizeBytesVLC(series, min_value);
	bytesBC += sizeBytesBitCompressing(series, min_value);
	// Bitmap de series fijas
	sd_vector<> bvSeriesFijas(bvSF);
	bytesVLCV += size_in_bytes(bvSeriesFijas);
	bytesBC += size_in_bytes(bvSeriesFijas);

	int kbytesVLC = bytesVLCV / 1024;
	int kbytesBC = bytesBC / 1024;

	cout << "name\tvlc\tbc" << endl;
	cout << argv[1] << "\t" << kbytesVLC << "\t" << kbytesBC << " [KB]" << endl;

	return 0;
}