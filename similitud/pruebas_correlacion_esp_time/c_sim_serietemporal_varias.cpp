#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

string get_header(int x){
    string header = "";
    for(int i=0; i<x; i++){
        header += "\t[" + to_string(i) + "]";
    }
    return header;
}

string print_serie(vector<int> v){
    string s = to_string(v[0]);
    for(int i=1; i<v.size(); i++){
        s += " " + to_string(v[i]);
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

double calc_sumadiffST(vector<int> x, vector<int> y){
    double acum = 0;
    for(int i=0; i<x.size(); i++){
        acum += (x[i] - y[i]);
    }
    return acum;
}

double calc_mape(vector<int> actual, vector<int> pred){
    double acum = 0.0;
    for(int i=0; i<actual.size(); i++){
        double temp = (actual[i] - pred[i]) / (actual[i] + 0.000001);
        acum += temp;
    }
    return (acum / actual.size());
}

double calc_euclidean(vector<int> actual, vector<int> pred){
    unsigned long long acum = 0;
    for(int i=0; i<actual.size(); i++){
        long diff = actual[i] - pred[i];
        diff *= diff;
        acum += diff;
    }
    return sqrt(acum);
}

double ZETA(vector<int> X, vector<int> Y){
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
    vector<vector<int>> series = {{1,2,3,4,5,6,7,8,9,10}            // st_00
                                ,{2,3,4,5,6,7,8,9,10,11}            // st_01
                                ,{11,12,13,14,15,16,17,18,19,20}    // st_02
                                ,{10,9,8,7,6,5,4,3,2,1}             // st_03
                                ,{2,4,6,8,10,12,14,16,18,20}        // st_04
                                ,{1,3,5,7,9,11,13,15,17,19}         // st_05
                                ,{1,1,1,1,1,1,1,1,1,1}              // st_06
                                ,{5,5,5,5,5,5,5,5,5,5}              // st_07
                                ,{10,10,10,10,10,10,10,10,10,10}    // st_08
                                ,{15,15,15,15,15,15,15,15,15,15}    // st_09
                                ,{1,1,2,2,3,3,4,4,5,5}              // st_10
                                ,{10,10,9,9,8,8,7,7,6,6}            // st_11
                                ,{1,2,3,4,5,6,7,8,7,7}              // st_12
                                ,{2,2,3,4,5,6,7,8,9,10}             // st_13
                                ,{20,19,18,17,16,15,14,13,12,11}    // st_14
                                ,{20,18,16,14,12,10,8,6,4,2}};      // st_15

    for(int i=0; i<series.size(); i++){
        cout << print_serie(series[i]) << endl;
    }

    string header = get_header(series.size());

    // Calculando matriz de comparación por el promedio de cada serie de tiempo
    vector<double> promedios(series.size());
    for(int i=0; i<series.size(); i++){
        promedios[i] = promedioST(series[i]);
    }
    cout << "Comparativa por promedio de la serie de tiempo:" << endl;
    cout << header << endl;
    for(int i=0; i<series.size(); i++){
        cout << " [" << i << "]\t";
        for(int j=0; j<series.size(); j++){
            cout << (promedios[i]-promedios[j]) << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // Calculando matriz de comparación por diferencia entre cada serie de tiempo
    cout << "Comparativa por diferencia de las series de tiempo:" << endl;
    cout << header << endl;
    for(int i=0; i<series.size(); i++){
        cout << " [" << i << "]\t";
        for(int j=0; j<series.size(); j++){
            cout << calc_sumadiffST(series[i], series[j]) << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // Calculando matriz de comparación por distancia euclidea entre series de tiempo
    cout << "Comparativa por distancia euclidea entre las series de tiempo:" << endl;
    cout << header << endl;
    for(int i=0; i<series.size(); i++){
        cout << " [" << i << "]\t";
        for(int j=0; j<series.size(); j++){
            cout << calc_euclidean(series[i], series[j]) << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // Calculando matriz de comparación por MAPE entre series de tiempo
    cout << "Comparativa por MAPE entre las series de tiempo:" << endl;
    cout << header << endl;
    for(int i=0; i<series.size(); i++){
        cout << " [" << i << "]\t";
        for(int j=0; j<series.size(); j++){
            cout << calc_mape(series[i], series[j]) << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // Calculando matriz de comparación por ZETA entre series de tiempo
    cout << "Comparativa por ZETA entre las series de tiempo:" << endl;
    cout << header << endl;
    for(int i=0; i<series.size(); i++){
        cout << " [" << i << "]\t";
        for(int j=0; j<series.size(); j++){
            cout << ZETA(series[i], series[j]) << "\t";
        }
        cout << endl;
    }
    cout << endl;

    return 0;
}
