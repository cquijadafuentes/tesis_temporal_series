#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

/*
    La estadística de Moran se calculará en base a la fórmula que indica:
    I = (Factor_1) * (Factor_2)
    donde
    Factor_1 = (Cantidad de elementos / Sumatoria de los pesos)
    Factor_2 = Numerador_1 / Denominador_1
    Numerador_1 = Sumatoria_i Sumatoria_j de: (peso_ij) * (x_i - X_prom) * (x_j - X_prom)
    Denominador_1 = Sumatoria_i de: (x_i - X_prom)^2
*/

using namespace std;

double dist_euclidea(int a1, int b1, int a2, int b2){
    int a = a1 - a2;
    int b = b1 - b2;
    long long c = (a*a) + (b*b);
    return sqrt(c);
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <filename>" << endl;
		return 0;
	}
//    cout << "DATASET: " << argv[1] << endl;
	int rows, cols, a1, a2, a3, a4;
    // Leyendo datos desde el archivo de entrada
	ifstream txtInput(argv[1], ifstream::in);
	txtInput >> rows >> cols >> a1 >> a2 >> a3 >> a4;
	int totalCeldas = rows*cols;
    double distMax = dist_euclidea(0, 0, rows, cols);
    vector<int> grilla(totalCeldas);
    // Generando matriz de pesos
    int i,j;
    string fileName;
    int lenTempSerie = 0;
	while(txtInput >> fileName){
        // Abrir el archivo binario en modo binario utilizando ifstream
        ifstream archivo(fileName, ios::binary);
        cout << "Leyendo el archivo " << fileName << endl;
        i = 0;
        if(archivo.is_open()) {
            // Lectura de los enteros de 32 bits del archivo
            int entero;
            long long acum = 0;
            cout << "Cargando grilla.. ";
            for(int f = 0; f < rows; f++){
                for(int c = 0; c < cols; c++) {
                    archivo.read(reinterpret_cast<char*>(&entero), sizeof(entero));
                    grilla[i] = entero;
                    acum += 0;
                    i++;
                }
            }
            cout << "OK" << endl;
            double promedio = acum / totalCeldas;
            // Cálculo de la Estadística de Moran
            i = 0;
            j = 0;
            double sumaW = 0.0;
            double denominador = 0.0;
            double numerador = 0.0;
            cout << "Calculando I de Moran.. " << endl;
            for(int f1 = 0; f1 < rows; f1++){
                for(int c1 = 0; c1 < cols; c1++) {
                    double diff_i_promedio = grilla[i] - promedio;
                    for(int f2=(f1-1); f2<rows && f2<=(f1+1); f2++){
                        for (int c2=(c1-1); c2<cols && c2<=(c1+1); c2++){
                            double w = 0;
                            if(f1 != f2 && c1 != c2){
                                double distAux = dist_euclidea(f1, c1, f2, c2);
                                // Para la contigüidad tipo reina las celdas vecinas que tocan
                                // toman un valor de 1
                                w = 1;
                                sumaW += w;
                                denominador += (w * (diff_i_promedio) * (grilla[j] - promedio));
                            }
                            j++;
                        }
                    }
                    numerador += (diff_i_promedio * diff_i_promedio);
                    i++;
                    j = 0;
                }
            }
            double moran_I = (totalCeldas / sumaW) * (numerador / denominador);
            cout << "Moran's I = " << moran_I << endl;
            archivo.close();
        } else {
            cerr << "Error al abrir el archivo: " << fileName << endl;
        }
        lenTempSerie++;
	}
    cout << "lenTempSerie: " << lenTempSerie << endl;

    return 0;
}
