#include "TempRaster_QuadComp.hpp"

using namespace std;
using namespace sdsl;

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
	vector<int_vector<>> series;
	vector<int_vector<>> refs;
	vector<int> valoresPVSR;		// Guarda el primer valor de cada serie de referencia
	vector<int> valoresSF;		// Guarda el valor de cada serie fija
	int iFV = 0;
	for(int c=0; c<n_cols; c++){
		vector<int> serieReferencia = tseries[0][c];
		for(int t=0; t<n_inst; t++){
			serieReferencia[t] = tseries[0][c][t];
		}
		valoresPVSR.push_back(tseries[0][c][0]);
		bvSF[c] = 0;
		int val;
		int_vector<> ivaux(n_inst-1);
		for(int k=1; k<n_inst; k++){
			val = tseries[0][c][k] - tseries[0][c][k-1];
			ivaux[k-1] = zigzag_encode(val);
		}
		util::bit_compress(ivaux);
		refs.push_back(ivaux);
		for(int f=1; f<n_rows; f++){		// fc = fila-cuadrante
			int iCelda = 9*f+c;
			if(esFija(tseries[f][c])){
				// La serie de tiempo es fija y no se representa
				valoresSF.push_back(tseries[f][c][0]);
				bvSF[iCelda] = 1;
			}else{
				int_vector<> ivaux2(n_inst);
				for(int k=0; k<n_inst; k++){
					val = serieReferencia[k] - tseries[f][c][k];
					ivaux2[k] = zigzag_encode(val);
				}
				util::bit_compress(ivaux2);
				series.push_back(ivaux2);
				bvSF[iCelda] = 0;
			}
		}
	}

	int_vector<> fixedValue(valoresSF.size());
	for(int i=0; i<valoresSF.size(); i++){
		fixedValue[i] = valoresSF[i] - min_value;
	}
	util::bit_compress(fixedValue);

	int_vector<> refFirstValue(valoresPVSR.size());
	for(int i=0; i<valoresPVSR.size(); i++){
		refFirstValue[i] = valoresPVSR[i] - min_value;
	}
	util::bit_compress(refFirstValue);

	sd_vector<> bvSeriesFijas(bvSF);

	// CALCULANDO TAMAÑO
	int bytes = 0;
	// Atributos enteros
	bytes += (sizeof(int) * 5);
	// int_vectors
	bytes += size_in_bytes(fixedValue);
	bytes += size_in_bytes(refFirstValue);
	for(int i=0; i<refs.size(); i++){
		bytes += size_in_bytes(refs[i]);
	}
	for(int i=0; i<series.size(); i++){
		bytes += size_in_bytes(series[i]);
	}
	// bit_vectors
	bytes += size_in_bytes(bvSeriesFijas);
	int mbytes = bytes / 1024 / 1024;
	cout << argv[1] << "\t" << mbytes << " [MB]" << endl;

	return 0;
}