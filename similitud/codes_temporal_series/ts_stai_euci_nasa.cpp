#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

/*
	Determina el Índice de Autocorrelación Espacio-Temporal
*/

bool esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}

double ZETA(vector<int> X, vector<double> Y){
    /*
        Igual a la función anterior pero para valores enteros
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
    return ZETA;
}

double EUCLIDEAN(vector<int> actual, vector<double> pred){
    unsigned long long acum = 0;
    for(int i=0; i<actual.size(); i++){
        long diff = actual[i] - pred[i];
        diff *= diff;
        acum += diff;
    }
    return sqrt(acum);
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile>" << endl;
		return 0;
	}
    // Leyendo datos desde el archivo de entrada
    ifstream infile(argv[1], ofstream::binary);
    if(infile.fail()){
        cout << "Error! Lectura de " << argv[1] << " fallida." << endl;
        return -1;
    }
    int rows, cols, lenTempSerie;
    // Cargando datos
    infile.read((char *)&rows, sizeof(int));
    infile.read((char *)&cols, sizeof(int));
    infile.read((char *)&lenTempSerie, sizeof(int));

    cout << "File: " << argv[1] << endl;
    cout << "Filas: " << rows << " - Cols: " << cols << " - Muestras: " << lenTempSerie << endl;

    vector<vector<vector<int>>> temporalSeries(rows, vector<vector<int>>(cols, vector<int>(lenTempSerie)));
    for(int i=0; i<rows; i++){
    	for(int j=0; j<cols; j++){
    		for(int k=0; k<lenTempSerie; k++){
                infile.read((char *)&temporalSeries[i][j][k], sizeof(int));
            }
    	}        
    }

    int totalCeldas = rows * cols;

	//  Marcando series temporales fijas: todos sus elementos iguales
	//	y calculando serie promedio con series no fijas.
	vector<bit_vector> serieFija(rows, bit_vector(cols));
	int seriesFijas = 0;
	int seriesNoFijas = 0;
	// Se marcan las series fijas
	for(int f=0; f<rows; f++){
		for(int c=0; c<cols; c++){
			if(esFija(temporalSeries[f][c])){
				serieFija[f][c] = 1;
				seriesFijas++;
			}else{
				seriesNoFijas++;
			}
		}
	}
	// Se verifican las series no fijas que si poseen vecinos
	// y se acumulan los valores para la serie promedio
	vector<bit_vector> conVecinos(rows, bit_vector(cols));
	vector<double> stPromedio(lenTempSerie, 0.0);
	int noFijasSinVecinos = 0;
	int noFijasConVecinos = 0;
	for(int f=0; f<rows; f++){
		for(int c=0; c<cols; c++){
			if(serieFija[f][c] == 0){
				int vecinos = 0;
				for(int df=-1; df<2 && f+df < rows; df++){
					for(int dc=-1; dc<2 && dc+c < cols; dc++){
						// El ciclo permite recorrer las 8 celdas que lo rodean
						// Limita por valores límite superior (rows y cols)
						if( ((f+df) >= 0) && ((c+dc) >=0) ){
							// Validación por el límite inferior (0)
							if((df != 0) && (dc != 0) && serieFija[f+df][c+dc] == 0){
								// Se verifica que no sea la misma celda, y que no sea fija
								vecinos++;
							}
						}
					}
				}
				if(vecinos > 0){
					noFijasConVecinos++;
					conVecinos[f][c] = 1;
					for(int k=0; k<lenTempSerie; k++){
						stPromedio[k] += temporalSeries[f][c][k];
					}
				}else{
					noFijasSinVecinos++;
				}
			}
		}
	}
	// Se prorratea el acumulado de la serie promedio
	for(int k=0; k<lenTempSerie; k++){
		stPromedio[k] /= noFijasConVecinos;
	}
	// Se calculan los valores para los índices
	vector<vector<double>> zethaValues(rows, vector<double>(cols, 0.0));
	vector<vector<double>> eucDistance(rows, vector<double>(cols, 0.0));
	for(int f=0; f<rows; f++){
		for(int c=0; c<cols; c++){
			// Sólo para las celdas que si poseen vecinos y se usaron para
			// calcular el promedio
			if(conVecinos[f][c] == 1){
				zethaValues[f][c] = ZETA(temporalSeries[f][c], stPromedio);
				eucDistance[f][c] = EUCLIDEAN(temporalSeries[f][c], stPromedio);
			}
		}
	}


	// Cálculo de Medida de autocorrelación espacio-temporal
	int sumaW = 0;
	int N = 0;
	double numeradorIAET = 0.0;
	double denominadorIAET = 0.0;
	double numeradorEUC = 0.0;
	double denominadorEUC = 0.0;
	for(int f=0; f<rows; f++){
		for(int c=0; c<cols; c++){
			if(conVecinos[f][c] == 1){
				for(int df=-1; df<2 && f+df < rows; df++){
					for(int dc=-1; dc<2 && dc+c < cols; dc++){
						// El ciclo permite recorrer las 8 celdas que lo rodean
						// Limita por valores límite superior (rows y cols)
						double factor;
						if( ((f+df) >= 0) && ((c+dc) >=0) ){
							// Validación por el límite inferior (0)
							if((df != 0) && (dc != 0) && serieFija[f+df][c+dc] == 0){
								// Se verifica que no sea la misma celda, y que no sea fija
								sumaW++;
								numeradorIAET += (zethaValues[f][c] * zethaValues[f+df][c+dc]);
								numeradorEUC += (eucDistance[f][c] * eucDistance[f+df][c+dc]);
							}
						}
					}
				}
				denominadorIAET += (zethaValues[f][c] * zethaValues[f][c]);
				denominadorEUC += (eucDistance[f][c] * eucDistance[f][c]);
				N++;
			}
		}
	}

	// El valor N depende en este caso de los sensores considerados en la matriz de pesos
	double IAET = ((0.0+N) / sumaW) * (numeradorIAET / denominadorIAET);
	double IEUC = ((0.0+N) / sumaW) * (numeradorEUC / denominadorEUC);
	cout << setprecision(4) << fixed;
	cout << "dataset\t\tIAET\t\tIEUC\tN\ttotal\tfijas\tnoFijas\tNFconV\tNFsinV" << endl;
	cout << argv[1] << "\t" << IAET << "\t" << IEUC << "\t" << N << "\t" << totalCeldas << "\t";
	cout << seriesFijas << "\t" << seriesNoFijas << "\t" << noFijasConVecinos << "\t" << noFijasSinVecinos << endl;

}
