#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sdsl/vectors.hpp>

using namespace std;
using namespace sdsl;

/*
	Calcula espacio necesario para arreglos de datos con
	1k, 2k, 4k, 8k, y 16k valores, entre rangos de
	0.1k, 1, 10k, 100k, 1M, y 10M
*/

int main(int argc, char const *argv[]){

	vector<int> rango = {100, 1000, 10000, 100000, 1000000, 10000000};
	vector<int> tamaño = {1000, 2000, 4000, 8000, 16000};
	vector<vector<int>> memoria(rango.size(), vector<int>(tamaño.size(), 0));
	for(int i=0; i<rango.size(); i++){
		for(int j=0; j<tamaño.size(); j++){
			int_vector<> v(tamaño[j]);
			generate(v.begin(), v.end(), rand);
			for(int k=0; k<tamaño[j]; k++){
				v[k] = v[k] % rango[i];
			}
			util::bit_compress(v);
			vlc_vector<> vlcv(v);
			memoria[i][j] = (int)(size_in_bytes(vlcv)/1024);
		}
	}

	cout << "Resultados" << endl;
	for(int i=0; i<tamaño.size(); i++){
		cout << "\t" << tamaño[i];
	}
	cout << endl;
	for(int i=0; i<rango.size(); i++){
		cout << rango[i];
		for(int j=0; j<tamaño.size(); j++){
			cout << "\t" << memoria[i][j];
		}
		cout << endl;
	}
	cout << endl;


	cout << "******************************" << endl;
	int m = 1000000;
	int_vector<> ivT1(m);
	generate(ivT1.begin(), ivT1.end(), rand);
	int_vector<> ivT2(m);
	for(int i=0; i<m; i++){
		ivT2[i] = ivT1[i]/1000;
	}
	cout << "m: " << m << endl;
	cout << "******************************" << endl;
	cout << "ivT1:" << endl;
	for(int i=0; i<20; i++){
		cout << "\t" << ivT1[i];
	}
	cout << endl;
	cout << "******************************" << endl;
	cout << "ivT2:" << endl;
	for(int i=0; i<20; i++){
		cout << "\t" << ivT2[i];
	}
	cout << endl;
	cout << "******************************" << endl;
	util::bit_compress(ivT1);
	util::bit_compress(ivT2);
	vlc_vector<> vlcT;
	int bytes_c1 = size_in_bytes(ivT1);
	int bytes_c2 = size_in_bytes(ivT2);
	int bytes_d1 = size_in_bytes(vlc_vector<coder::elias_delta>(ivT1));
	int bytes_d2 = size_in_bytes(vlc_vector<coder::elias_delta>(ivT2));
	vlc_vector<coder::elias_delta> vlcTd(ivT2);
	int bytes_d3 = size_in_bytes(vlcTd);
	int bytes_g1 = size_in_bytes(vlc_vector<coder::elias_gamma>(ivT1));
	int bytes_g2 = size_in_bytes(vlc_vector<coder::elias_gamma>(ivT2));
	vlc_vector<coder::elias_gamma> vlcTg(ivT2);
	int bytes_g3 = size_in_bytes(vlcTg);
	int bytes_f1 = size_in_bytes(vlc_vector<coder::fibonacci>(ivT1));
	int bytes_f2 = size_in_bytes(vlc_vector<coder::fibonacci>(ivT2));
	vlc_vector<coder::fibonacci> vlcTf(ivT2);
	int bytes_f3 = size_in_bytes(vlcTf);
	cout << "bit_compress:\t" << bytes_c1 << "\t" << bytes_c2 << "\t\t[B]" << endl;
	cout << "elias_delta:\t" << bytes_d1 << "\t" << bytes_d2 << "\t" << bytes_d3 << "\t[B]" << endl;
	cout << "elias_gamma:\t" << bytes_g1 << "\t" << bytes_g2 << "\t" << bytes_g3 << "\t[B]" << endl;
	cout << "fibonacci:\t" << bytes_f1 << "\t" << bytes_f2 << "\t" << bytes_f3 << "\t[B]" << endl;

	int kbytes_c1 = bytes_c1 / 1024;
	int kbytes_c2 = bytes_c2 / 1024;
	int kbytes_d1 = bytes_d1 / 1024;
	int kbytes_d2 = bytes_d2 / 1024;
	int kbytes_d3 = bytes_d3 / 1024;
	int kbytes_g1 = bytes_g1 / 1024;
	int kbytes_g2 = bytes_g2 / 1024;
	int kbytes_g3 = bytes_g3 / 1024;
	int kbytes_f1 = bytes_f1 / 1024;
	int kbytes_f2 = bytes_f2 / 1024;
	int kbytes_f3 = bytes_f3 / 1024;

	cout << "bit_compress:\t" << kbytes_c1 << "\t" << kbytes_c2 << "\t\t[KB]" << endl;
	cout << "elias_delta:\t" << kbytes_d1 << "\t" << kbytes_d2 << "\t" << kbytes_d3 << "\t[KB]" << endl;
	cout << "elias_gamma:\t" << kbytes_g1 << "\t" << kbytes_g2 << "\t" << kbytes_g3 << "\t[KB]" << endl;
	cout << "fibonacci:\t" << kbytes_f1 << "\t" << kbytes_f2 << "\t" << kbytes_f3 << "\t[KB]" << endl;

	return 0;
}

/*
g++ -I ~/include -L ~/lib -o pruebas_sdsl_vector_vlcv pruebas_sdsl_vector_vlcv.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

/*


*/