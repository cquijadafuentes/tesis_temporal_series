#include "TempRaster_QuadComp.hpp"

using namespace std;
using namespace sdsl;

map<int,int> histoINT;
map<int,int> histoBC;
map<int,int> histoBC2;
map<int,int> histoBCdiff;
map<int,int> histoVLCdiff;
map<int,int> histoVLCzz;

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

int maximo(vector<int> x){
	int max = x[0];
	for(int i=1; i<x.size(); i++){
		if(x[i] > max){
			max = x[i];
		}
	}
	return max;
}

int maximo(vector<vector<int>> x){
	int max = maximo(x[0]);
	int aux;
	for(int i=1; i<x.size(); i++){
		aux = maximo(x[i]);
		if(aux > max){
			max = aux;
		}
	}
	return max;
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

int build_int_compress(vector<vector<vector<int>>> s, int minS){
	// Todas las series de tiempo se desplazan por el valor mínimo del conjunto
	int rows = s.size();
	int cols = s[0].size();
	int inst = s[0][0].size();
	int kb = 0;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			if(! esFija(s[i][j])){
				int_vector<> iv(inst);
				for(int k=0; k<inst; k++){
					iv[k] = s[i][j][k] - minS;
					histoBC[iv[k]] += 1;
				}
				util::bit_compress(iv);
				kb += size_in_bytes(iv);
			}
		}
	}
	kb = (int)(kb / 1024);
	return kb;
}

int build_int_compress(vector<vector<vector<int>>> s){
	// Para cada serie de tiempo se obtiene su valor mínimo y se usa para desplazarla
	int rows = s.size();
	int cols = s[0].size();
	int inst = s[0][0].size();
	int kb = 0;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			if(! esFija(s[i][j])){
				int minS = minimo(s[i][j]);
				int_vector<> iv(inst);
				for(int k=0; k<inst; k++){
					iv[k] = s[i][j][k] - minS;
					histoBC2[iv[k]] += 1;
				}
				util::bit_compress(iv);
				kb += size_in_bytes(iv);
			}
		}
	}
	kb = (int)(kb / 1024);
	return kb;
}

int build_int_compress_diff(vector<vector<vector<int>>> s){
	// Serie de Referencia: se codifica por usa R[i]-R[i-1]
	// Series generales: se codifica por S[i]-R[i]
	int rows = s.size();
	int cols = s[0].size();
	int inst = s[0][0].size();
	int bytes = 0;
	for(int i=0; i<rows; i++){
		// La primera fila es la referencia
		vector<int> vR(inst-1);
		for(int k=1; k<inst; k++){
			vR[k-1] = s[i][0][k]-s[i][0][k-1];
		}
		int minSR = minimo(vR);
		int_vector<> ivR(inst-1);
		for(int k=0; k<inst-1; k++){
			ivR[k] = vR[k]-minSR;
			histoBCdiff[ivR[k]] += 1;
		}
		util::bit_compress(ivR);
		bytes += size_in_bytes(ivR);
		for(int j=1; j<cols; j++){
			if(! esFija(s[i][j])){
				vector<int> vS(inst);
				for(int k=0; k<inst; k++){
					vS[k] = s[i][j][k] - s[i][0][k];
				}
				int minS = minimo(vS);
				int_vector<> ivS(inst);
				for(int k=0; k<inst; k++){
					ivS[k] = vS[k]-minS;
					histoBCdiff[ivS[k]] += 1;
				}
				util::bit_compress(ivS);
				bytes += size_in_bytes(ivS);
			}
		}
	}
	int kb = (int)(bytes / 1024);
	return kb;
}

int build_int_vlc_diff(vector<vector<vector<int>>> s){
	// Serie de Referencia: se codifica por usa R[i]-R[i-1]
	// Series generales: se codifica por S[i]-R[i]
	int rows = s.size();
	int cols = s[0].size();
	int inst = s[0][0].size();
	int bytes = 0;
	for(int i=0; i<rows; i++){
		// La primera fila es la referencia
		vector<int> vR(inst-1);
		for(int k=1; k<inst; k++){
			vR[k-1] = s[i][0][k]-s[i][0][k-1];
		}
		int minSR = minimo(vR);
		int_vector<> ivR(inst-1);
		for(int k=0; k<inst-1; k++){
			ivR[k] = vR[k]-minSR;
			histoVLCdiff[ivR[k]] += 1;
		}
		vlc_vector<> vlcR(ivR);
		bytes += size_in_bytes(vlcR);
		for(int j=1; j<cols; j++){
			if(! esFija(s[i][j])){
				vector<int> vS(inst);
				for(int k=0; k<inst; k++){
					vS[k] = s[i][j][k] - s[i][0][k];
				}
				int minS = minimo(vS);
				int_vector<> ivS(inst);
				for(int k=0; k<inst; k++){
					ivS[k] = vS[k]-minS;
					histoVLCdiff[ivS[k]] += 1;
				}
				vlc_vector<> vlcS(ivS);
				bytes += size_in_bytes(vlcS);
			}
		}
	}
	int kb = (int)(bytes / 1024);
	return kb;
}

int build_int_vlc_zz(vector<vector<vector<int>>> s){
	// Serie de Referencia: se codifica por usa R[i]-R[i-1]
	// Series generales: se codifica por S[i]-R[i]
	int rows = s.size();
	int cols = s[0].size();
	int inst = s[0][0].size();
	int bytes = 0;
	for(int i=0; i<rows; i++){
		// La primera fila es la referencia
		vector<int> vR(inst-1);
		for(int k=1; k<inst; k++){
			vR[k-1] = s[i][0][k]-s[i][0][k-1];
		}
		int minSR = minimo(vR);
		int_vector<> ivR(inst-1);
		for(int k=0; k<inst-1; k++){
			ivR[k] = zigzag_encode(vR[k]);
			histoVLCzz[ivR[k]] += 1;
		}
		vlc_vector<> vlcR(ivR);
		bytes += size_in_bytes(vlcR);
		for(int j=1; j<cols; j++){
			if(! esFija(s[i][j])){
				int minS = minimo(s[i][j]);
				vector<int> vS(inst);
				for(int k=0; k<inst; k++){
					vS[k] = s[i][j][k] - s[i][0][k];
				}
				int_vector<> ivS(inst);
				for(int k=0; k<inst; k++){
					ivS[k] = zigzag_encode(vS[k]);
					histoVLCzz[ivS[k]] += 1;
				}
				vlc_vector<> vlcS(ivS);
				bytes += size_in_bytes(vlcS);
			}
		}
	}
	int kb = (int)(bytes / 1024);
	return kb;
}

int minHistogramas(){
	cout << "Buscando minimo de histogramas..." << endl;
	vector<int> m(6);
	map<int,int>::iterator aux;
	aux = histoINT.begin();
	m[0] = aux->first;
	aux = histoBC.begin();
	m[1] = aux->first;
	aux = histoBC2.begin();
	m[2] = aux->first;
	aux = histoBCdiff.begin();
	m[3] = aux->first;
	aux = histoVLCdiff.begin();
	m[4] = aux->first;
	aux = histoVLCzz.begin();
	m[5] = aux->first;
	return minimo(m);
}

int maxHistogramas(){
	cout << "Buscando máximo de histogramas..." << endl;
	vector<int> m(6);
	map<int,int>::iterator aux;
	aux = histoINT.end();
	aux--;
	m[0] = aux->first;
	aux = histoBC.end();
	aux--;
	m[1] = aux->first;
	aux = histoBC2.end();
	aux--;
	m[2] = aux->first;
	aux = histoBCdiff.end();
	aux--;
	m[3] = aux->first;
	aux = histoVLCdiff.end();
	aux--;
	m[4] = aux->first;
	aux = histoVLCzz.end();
	aux--;
	m[5] = aux->first;
	return maximo(m);
}

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <mapping_sensor_matrix> [<output_histogram>]" << endl;
		cout << "File\tkb_bc\tkb_bc2\tkb_bc_diff\tkb_vlc_diff\tkb_vlc_zz\t[KB]" << endl;
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
	cout << "Electrodos: " << electrodos << " - Muestras: " << muestras << endl;

	vector<vector<int>> eegData(electrodos, vector<int>(muestras));
	for(int i=0; i<electrodos; i++){
		for(int k=0; k<muestras; k++){
			infile.read((char *)&eegData[i][k], sizeof(int));
			histoINT[eegData[i][k]] += 1;
		}
	}
	int min_value = minimo(eegData);
	int max_value = maximo(eegData);
	cout << "Rango valores: [" << min_value << " , " << max_value << "]" << endl;

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
	
	int kb_bc = build_int_compress(tseries, min_value);
	int kb_bc2 = build_int_compress(tseries);
	int kb_bc_diff = build_int_compress_diff(tseries);
	int kb_vlc_diff = build_int_vlc_diff(tseries);
	int kb_vlc_zz = build_int_vlc_zz(tseries);

	cout << argv[1] << "\t" << kb_bc << "\t" << kb_bc2 << "\t" << kb_bc_diff << "\t" << kb_vlc_diff << "\t" << kb_vlc_zz << "\t[KB]" << endl;

	int hMin = minHistogramas();
	int hMax = maxHistogramas();

	cout << "Rango Histogramas: [" << hMin << " , " << hMax << "]" << endl;
	
	if(argc >= 4){
		ofstream salida(argv[3]);
		if(salida.fail()){
			cout << "Error! Al crear el archivo " << argv[3] << " de salida." << endl;
			return -1;
		}
		salida << "#num\tint\tbc\tbc2\tbcdiff\tvlcdiff\tvlczz" << endl;
		for(int i=hMin; i<=hMax; i++){
			salida << i << "\t" << histoINT[i] << "\t" << histoBC[i] << "\t" << histoBC2[i] << "\t" << histoBCdiff[i] << "\t" << histoVLCdiff[i] << "\t" << histoVLCzz[i] << endl;
		}
	}

	return 0;
}
