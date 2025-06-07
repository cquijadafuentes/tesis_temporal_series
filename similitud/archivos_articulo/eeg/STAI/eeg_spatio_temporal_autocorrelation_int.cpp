#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

/*
    Implementación de cálculo de correlación espacio temporal propuesto en:
    Measuring spatio-temporal autocorrelation in time series data of collective human mobility 
    [Gao, Cheng, Meng and Liu - Geo-spatial information science, 2019]
    Aplicado a datos de Electro-encéfalogramas
    Usando como entrada archivo .bin
*/

using namespace std;

string print_celda(int x, int y, double v){
    string s = "";
    s += "[" + to_string(x) + "][" + to_string(y) + "] (" + to_string(v) + ")";
    return s;
}

string print_serie(vector<double> v){
    string s = "";
    for(int i=0; i<v.size(); i++){
        s += to_string(v[i]) + " ";
    }
    return s;
}

string print_serie(vector<int> v){
    string s = "";
    for(int i=0; i<v.size(); i++){
        s += to_string(v[i]) + " ";
    }
    return s;
}

double promedioST(vector<int> x){
    unsigned long long acum = 0;
    for(int i=0; i<x.size(); i++){
        acum += x[i];
    }
    acum /= x.size();
    return acum;
}

double ZETA(vector<int> X, vector<double> Y){
    /*
        - First CORT is calculated: 
            Adaptative temporal dissimilarity:
            covers temporal correlation and data distance
            Result in range -1 to 1:
                -1 X and Y share similar growth in rate but opposite in direction
                1 X and Y have a similiar growth in rate and direction
                0 different temporal behaviors
        - Then PHI is applied:
            tunes the first order correlation coefficient from [-1,1] to [0,2] where
            phi(0) = 1, phi(1) = 0, and phi(-1) = 2.
            Then, less value means more similar time series, 
            and greater values means less similar time series.
        - Finally is multiplied by the difference of accumulative volumes
    */
    double numeradorCORT = 0.0;
    double denCORTFactorX = 0.0;
    double denCORTFactorY = 0.0;
    double auxDiffX, auxDiffY;
    double accVolX = 0.0 + X[0];
    double accVolY = 0.0 + X[0];
    for(int i=1; i <X.size(); i++){
        auxDiffX = X[i]-X[i-1];
        auxDiffY = Y[i]-Y[i-1];
        numeradorCORT += (auxDiffX*auxDiffY);
        denCORTFactorX += (auxDiffX*auxDiffX);
        denCORTFactorY += (auxDiffY*auxDiffY);
        accVolX += X[i];
        accVolY += Y[i];
    }
    double denominadorCORT = sqrt(denCORTFactorX) * sqrt(denCORTFactorY);
    double CORT = numeradorCORT / denominadorCORT;
    double PHI = 2 / (1 + exp(2*CORT));
    double ZETA = PHI * (accVolX - accVolY);
//    if(denCORTFactorX * denCORTFactorY == 0){
//        cout << "Denominador en 0: " << auxDiffX << " " << auxDiffY << endl;
//        cout << print_serie(X) << endl;
//        cout << print_serie(Y) << endl;
//    }
    return ZETA;
}

int main(int argc, char const *argv[]){
    if(argc < 3){
        cout << "Error! Faltan argumentos." << endl;
        cout << argv[0] << " <filename.txt> <w_matrix_file.txt>" << endl;
        return 0;
    }
    cout << "DATASET: " << argv[1] << endl;
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
    cout << electrodos << " - " << muestras << endl;
    vector<vector<int>> eeg(electrodos, vector<int>(muestras));
    for(int i=0; i<electrodos; i++){
        for(int j=0; j<muestras; j++){
            infile.read((char *)&eeg[i][j], sizeof(int));
        }
    }
    infile.close();

    cout << "Matrix W: " << argv[2] << endl;
    ifstream eegMatrixIn(argv[2], ifstream::in);
    if(eegMatrixIn.fail()){
        cout << "Error! Lectura de " << argv[2] << " fallida." << endl;
        return -1;
    }
    int matrixIds;
    eegMatrixIn >> matrixIds;
    cout << matrixIds << endl;
    vector<vector<short>> matrixW(matrixIds, vector<short>(matrixIds));
    for(int i=0; i<matrixIds; i++){
        for(int j=0; j<matrixIds; j++){
            eegMatrixIn >> matrixW[i][j];
        }
    }
    eegMatrixIn.close();

    //  Calculando serie temporal promedio
    vector<double> stPromedio(muestras);
    for(int m=0; m<muestras; m++){
        double acum = 0.0;
        for(int e=0; e<electrodos; e++){
            acum += eeg[e][m];
        }
        stPromedio[m] = (acum / electrodos);
    }

    //  Marcando series temporales fijas: todos sus elementos iguales
    vector<bool> serieFija(electrodos, true);
    for(int e=0; e<electrodos; e++){
        for(int m=1; m<muestras; m++){
            serieFija[e] = serieFija[e] && (eeg[e][m] == eeg[e][m-1]);
        }
    }
    
    vector<double> zethaValues(electrodos);
    for(int e=0; e<electrodos; e++){
        zethaValues[e] = ZETA(eeg[e], stPromedio);
    }

    // Cálculo de Medida de utocorrelación espacio-temporal
    int sumaW = 0;
    int cantFijas = 0;
    double numerador = 0.0;
    double denominador = 0.0;
    for(int e=0; e<electrodos; e++){
        if(!serieFija[e]){
            for(int j=0; j<electrodos; j++){
                double w = 0;
                double factor;
                if(matrixW[e][j] == 1 && !serieFija[j]){
                    w = 1;
                    sumaW += w;
                    factor = w * zethaValues[e] * zethaValues[j];
                    numerador += factor;
                }
            }
            denominador += (zethaValues[e] * zethaValues[e]);
        }else{
            cantFijas++;
        }
    }
//    cout << "totalCeldas: " << (rows*cols) << endl;
//    cout << "cantFijas: " << cantFijas << endl;
//    cout << "sumaW: " << sumaW << endl;
//    cout << "numerador: " << numerador << endl;
//    cout << "denominador: " << denominador << endl;

    // El valor N depende en este caso de los sensores considerados en la matriz de pesos
    double N = 58.0 - cantFijas;    
    double IAET = (N / sumaW) * (numerador / denominador);
//    cout << argv[1] << " STA = " << IAET << endl;
    cout << "dataset\tIAET\ttotalCeldas\tceldasFijas\ttotalW" << endl;
    cout << argv[1] << "\t" << IAET << "\t" << (electrodos) << "\t" << cantFijas << "\t" << sumaW << endl;
    return 0;
}
