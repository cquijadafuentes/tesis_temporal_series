#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

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

int main(int argc, char const *argv[]){
    if(argc < 2){
        cout << "Error! Faltan argumentos." << endl;
        cout << argv[0] << " <filename>" << endl;
        return 0;
    }
    int rows, cols, a1, a2, a3, a4;
    // Leyendo datos desde el archivo de entrada
    ifstream txtInput(argv[1], ifstream::in);
    txtInput >> rows >> cols >> a1 >> a2 >> a3 >> a4;
    int totalCeldas = rows*cols;
    string fileName;
    vector<vector<int>> grilla(rows, vector<int>(cols));
    int lenTempSerie = 0;
    while(txtInput >> fileName){
        // Abrir el archivo binario en modo binario utilizando ifstream
        ifstream archivo(fileName, ios::binary);
        if(archivo.is_open()) {
            // Leer enteros de 32 bits del archivo
            int entero;
            long long acum = 0;
            for(int c = 0; c < cols; c++) {
                for(int f = 0; f < rows; f++){
                    archivo.read(reinterpret_cast<char*>(&entero), sizeof(entero));
                    grilla[f][c] = entero;
                    acum += entero;
                }
            }
            archivo.close();
//            cout << "OK" << endl;
            double promedio = (acum+0.0) / totalCeldas;
//            cout << "acum: " << acum << endl;
//            cout << "promedio: " << promedio << endl;
            // Cálculo de la Estadística de Moran
            double sumaW = 0.0;
            double numerador = 0.0;
            double denominador = 0.0;
//            cout << "Calculando I de Moran.. " << endl;
            for(int f1 = 0; f1 < rows; f1++){
                for(int c1 = 0; c1 < cols; c1++) {
                    double diff_i_promedio = grilla[f1][c1] - promedio;
                    for(int f2=(f1-1); f2<rows && f2<=(f1+1); f2++){
                        for (int c2=(c1-1); c2<cols && c2<=(c1+1); c2++){
                            double w = 0;
                            double aux;
                            if((f1 != f2 || c1 != c2) && f2 >= 0 && c2 >= 0){
                                // Para la contigüidad tipo reina las celdas vecinas que tocan
                                // toman un valor de 1
                                w = 1;
                                sumaW += w;
                                aux = w * (diff_i_promedio) * (grilla[f2][c2] - promedio);
                                numerador += aux;
                            }
                        }
                    }
                    denominador += (diff_i_promedio * diff_i_promedio);
                }
            }
//            cout << "totalCeldas: " << totalCeldas << endl;
//            cout << "sumaW: " << sumaW << endl;
//            cout << "numerador: " << numerador << endl;
//            cout << "denominador: " << denominador << endl;
            double moran_I = (totalCeldas / sumaW) * (numerador / denominador);
            cout << lenTempSerie << "\t" << moran_I << endl;
        } else {
            cerr << "Error al abrir el archivo: " << fileName << endl;
        }
        lenTempSerie++;
	}
    return 0;
}
