/*
	Entrada: archivo con los resultados de similitud de similitud en la columna 2 desde la segunda fila en adelante.
	Salida: distribución de las similitudes agrupados por valores enteros.

*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <filename>" << endl;
		return 0;
	}
	ifstream archivo(argv[1], ios::in);
	string x;
	// Consumir encabezado del archivo: "# distancia similitud"
	for(int i=0; i<3; i++){
		archivo >> x;
	}
	int cont = 0;
	int contNan = 0;
	float min = 1000000;
	float max = -min;
	int truncado;
	float aux;
	map<int,int> m;
	while(archivo >> x){
		archivo >> x;
		cont ++;
		if(x == "-nan"){
			contNan++;
		}else{
			aux = stod(x);
			truncado = (int)aux;
			m[truncado]++;
			if(aux < min){
				min = aux;
			}
			if(aux > max){
				max = aux;
			}
		}
	}

	map<int,int>::iterator mit;
	for(mit = m.begin(); mit != m.end(); mit++){
		cout << mit->first << "\t" << mit->second << endl;
	}

	cout << "Total datos: " << cont << endl;
	cout << "Total -nan: " << contNan << endl;
	cout << "Mínimo: " << min << endl;
	cout << "Máximo: " << max << endl;

	return 0;
}