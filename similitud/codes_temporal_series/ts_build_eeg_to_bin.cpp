#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 3){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> <outputFile>" << endl;
		return 0;
	}

	ifstream eegDataIn(argv[1], ifstream::in);
	if(eegDataIn.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}
	int electrodos, muestras;
	eegDataIn >> electrodos >> muestras;
	ofstream outfile(argv[2], ofstream::binary);
	// Guardando valores enteros
	outfile.write((const char*)&electrodos, sizeof(int));
	outfile.write((const char*)&muestras, sizeof(int));
	double auxD, maxi = 0;
	int auxF;
	for(int i=0; i<electrodos; i++){
		for(int j=0; j<muestras; j++){
			eegDataIn >> auxD;
			if(auxD > maxi){
				maxi = auxD;
			}
			auxF = (int) auxD*100;
			outfile.write((const char*)&auxF, sizeof(int));
		}
	}
	eegDataIn.close();
	cout << "maximo valor: " << maxi << endl;
	outfile.close();
}