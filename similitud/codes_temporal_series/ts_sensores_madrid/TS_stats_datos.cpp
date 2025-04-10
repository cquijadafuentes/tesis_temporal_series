#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

int zigzag_decode(int i){
	return ((i >> 1) ^ -(i & 1));
}

unsigned int zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile> [<4>]" << endl;
		cout << "inputFile: archivo con los datos." << endl;
		cout << "<4>: Si se incluye se considera el archivo de entrada en formato STAI (los 4 atributos)." << endl;
		cout << "\tEn otro caso se considera que existen los datos para un atributo." << endl;
		return 0;
	}
	int numAtributos = 1;
	if(argc > 2 && stoi(argv[2]) == 4){
		numAtributos = 4;
	}
	
	// Leyendo datos desde el archivo de entrada
	ifstream dataSensores(argv[1], ifstream::in);
	if(dataSensores.fail()){
		cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
		return -1;
	}

	cout << "Archivo de entrada: " << argv[1] << endl;
	cout << "Num Atributos: " << numAtributos << endl;
	int sensores, muestras;
	dataSensores >> sensores >> muestras;
	cout << "Cantidad de sensores: " << sensores << endl;
	cout << "Cantidad de muestras: " << muestras << endl;

	int aux;
	for(int i=0; i<sensores; i++){
		// Descartando información de IDs
		dataSensores >> aux;
	}

	vector<long long int> acum(numAtributos, 0);
	vector<vector<vector<int>>> data(sensores, vector<vector<int>>(numAtributos, vector<int>(muestras, 0)));
	for(int i=0; i<sensores; i++){
		for(int j=0; j<numAtributos; j++){
			for(int k=0; k<muestras; k++){
				dataSensores >> aux;
				data[i][j][k] = aux;
				acum[j] += aux;
			}
		}
	}
	dataSensores.close();

	vector<double> promedio(numAtributos);
	vector<int> minimo(numAtributos);
	vector<int> maximo(numAtributos);
	int N = sensores * muestras;
	for(int j=0; j<numAtributos; j++){
		promedio[j] = (0.0 + acum[j]) / N;
		// Entregar valor inicial para luego recorrer todo.
		minimo[j] = data[0][j][0];
		maximo[j] = data[0][j][0];
	}

	vector<double> varianza(numAtributos);
	vector<double> desvEstandar(numAtributos);
	for(int i=0; i<sensores; i++){
		for(int j=0; j<numAtributos; j++){
			for(int k=0; k<muestras; k++){
				aux = promedio[j] - data[i][j][k];
				varianza[j] += (aux * aux);
				if(data[i][j][k] < minimo[j]){
					minimo[j] = data[i][j][k];
				}
				if(data[i][j][k] > maximo[j]){
					maximo[j] = data[i][j][k];
				}
			}
		}
	}
	for(int j=0; j<numAtributos; j++){
		varianza[j] /= N;
		desvEstandar[j] = sqrt(varianza[j]);
	}

	if(numAtributos > 1){
		cout << "\tInt\tOcup\tCarga\tVel" << endl;
	}

	cout << "Min:";
	for(int j=0; j<numAtributos; j++){
		cout << "\t" << minimo[j];
	}
	cout << endl;

	cout << "Max:";
	for(int j=0; j<numAtributos; j++){
		cout << "\t" << maximo[j];
	}
	cout << endl;

	cout << "Prom:";
	for(int j=0; j<numAtributos; j++){
		cout << "\t" << promedio[j];
	}
	cout << endl;

	cout << "Var:";
	for(int j=0; j<numAtributos; j++){
		cout << "\t" << varianza[j];
	}
	cout << endl;

	cout << "DEst.:";
	for(int j=0; j<numAtributos; j++){
		cout << "\t" << desvEstandar[j];
	}
	cout << endl;

	return 0;
}