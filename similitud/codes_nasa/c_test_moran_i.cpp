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

double dist_euclidea(int a1, int b1, int a2, int b2){
    int a = a1 - a2;
    int b = b1 - b2;
    long long c = (a*a) + (b*b);
    return sqrt(c);
}

string print_celda(int x, int y, int v){
    string s = "";
    s += "[" + to_string(x) + "][" + to_string(y) + "] (" + to_string(v) + ")";
    return s;
}

int main(int argc, char const *argv[]){
    vector<int> grilla = {1,2,3,4,5,6,7,8,9};
	int rows = 3;
    int cols = 3;
    int totalCeldas = rows*cols;
    long long acum = 0;
    for(int i=0; i<grilla.size(); i++){
        acum += grilla[i];
    }
    double promedio = (acum+0.0) / totalCeldas;
    cout << "acum: " << acum << endl;
    cout << "promedio: " << promedio << endl;
    // Generando matriz de pesos
    int i,j;
    int lenTempSerie = 0;
	// Lectura de los enteros de 32 bits del archivo
//            cout << "OK" << endl;
    // Cálculo de la Estadística de Moran
    double sumaW = 0.0;
    double numerador = 0.0;
    double denominador = 0.0;
//            cout << "Calculando I de Moran.. " << endl;
    for(int f1 = 0; f1 < rows; f1++){
        for(int c1 = 0; c1 < cols; c1++) {
            double diff_i_promedio = grilla[f1*rows+c1] - promedio;
            cout << "Desde celda: " << print_celda(f1, c1, grilla[f1*rows+c1]) << endl;
            for(int f2=(f1-1); f2<rows && f2<=(f1+1); f2++){
                for (int c2=(c1-1); c2<cols && c2<=(c1+1); c2++){
                    double w = 0;
                    double aux;
                    cout << "\tRevisando celda: " << print_celda(f2, c2, grilla[f2+rows+c2]);
                    if((f1 != f2 || c1 != c2) && f2 >= 0 && c2 >= 0){
                        // Para la contigüidad tipo reina las celdas vecinas que tocan
                        // toman un valor de 1
                        w = 1;
                        sumaW += w;
                        aux = w * (diff_i_promedio) * (grilla[f2*rows+c2] - promedio);
                        numerador += aux;
                        cout << " con valor: " << aux << endl;
                    }else{
                        cout << endl;
                    }
                }
            }
            denominador += (diff_i_promedio * diff_i_promedio);
        }
    }
    cout << "totalCeldas: " << totalCeldas << endl;
    cout << "sumaW: " << sumaW << endl;
    cout << "numerador: " << numerador << endl;
    cout << "denominador: " << denominador << endl;
    double moran_I = (totalCeldas / sumaW) * (numerador / denominador);
    cout << lenTempSerie << "\t" << moran_I << endl;
    return 0;
}
