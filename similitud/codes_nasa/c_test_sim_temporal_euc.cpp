#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

double calc_euclidean(vector<int> actual, vector<int> pred){
    long acum = 0;
    for(int i=0; i<actual.size(); i++){
        long diff = actual[i] - pred[i];
        diff *= diff;
        acum += diff;
    }
    return sqrt(acum);
}

double dist_euclidea(int a1, int b1, int a2, int b2){
    int a = a1 - a2;
    int b = b1 - b2;
    int c = a*a + b*b;
    return sqrt(c);
}

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <filas> <columnas> <lenSerieT>" << endl;
		return 0;
	}
    cout << "Generando datos de serie temporal" << endl;
	int rows = stoi(argv[1]);
    int cols = stoi(argv[2]);
    int lenSerieT = stoi(argv[3]);
	vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols, vector<int>(lenSerieT)));
    for(int f = 0; f < rows; f++){
        for(int c = 0; c < cols; c++) {
            for(int ts = 0; ts < lenSerieT; ts++){
                temporalSeries[f][c][ts] = f+c+ts;
            }
        }
    }

    cout << "Calculando similitud euclidea" << endl;

    // Procesando datos desde el archivo de entrada
    int cantComb = (rows*cols)*(rows*cols);
    double distancia, simEuclidea;
    cout << "#\tdistancia\tsimilitud" << endl;
    for(int f1=0; f1<rows; f1++){
        for(int c1=0; c1<cols; c1++){
            distancia = dist_euclidea(0, 0, f1, c1);
            simEuclidea = calc_euclidean(temporalSeries[0][0], temporalSeries[f1][c1]);
            cout << distancia << "\t" << simEuclidea << endl;
        }
    }

    return 0;
}
