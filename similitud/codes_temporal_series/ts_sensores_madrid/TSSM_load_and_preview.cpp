#include "TS_SM.hpp"

using namespace std;
using namespace sdsl;

string show_array(vector<int> x);
string show_first_elements(vlc_vector<> x, int n);

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <inputFile>" << endl;
		return 0;
	}

	string filename = argv[1];
	TempSeriesSensoresMadrid tssm(filename);
	cout << "num_sensores: " << tssm.num_sensores << endl;
	cout << "num_groups: " << tssm.num_groups << endl;
	cout << "num_muestras: " << tssm.num_muestras << endl;
	cout << "k: " << tssm.k << endl;
	cout << "min_value: " << tssm.min_value << endl;
	cout << "max_value: " << tssm.max_value << endl;
	cout << "Sens x group:" << show_array(tssm.sens_x_group) << endl;
	cout << "-------------------------------------------" << endl;
	cout << "pgReference.size(): " << tssm.pgReference.size() << endl;
	cout << "pgSeries.size(): " << tssm.pgSeries.size() << endl;
	cout << "lgSeries.size(): " << tssm.lgSeries.size() << endl;
	cout << "-------------------------------------------" << endl;
	cout << "pgFirstValue:" << endl;
	cout << "\t" << show_first_elements(tssm.pgFirstValue, 5) << endl;
	
	cout << "pgReference:" << endl;
	if(tssm.pgReference.size() > 0){
		int tope = 8;
		if(tssm.pgReference.size() < 8){
			tope = tssm.pgReference.size();
		}
		for(int i=0; i<tope; i++){
			cout << "[" << i << "]\t" << show_first_elements(tssm.pgReference[i], 5) << endl;
		}
	}else{
		cout << "\t no elements" << endl;
	}

	cout << "pgSeries:" << endl;
	if(tssm.pgSeries.size() > 0){
		int tope = 8;
		if(tssm.pgSeries.size() < 8){
			tope = tssm.pgSeries.size();
		}
		for(int i=0; i<tope; i++){
			cout << "[" << i << "]\t" << show_first_elements(tssm.pgSeries[i], 5) << endl;
		}
	}else{
		cout << "\t no elements" << endl;
	}

	cout << "lgFirstValue:" << endl;
	cout << "\t" << show_first_elements(tssm.lgFirstValue, 5) << endl;

	cout << "lgSeries:" << endl;
	if(tssm.lgSeries.size() > 0){
		int tope = 8;
		if(tssm.lgSeries.size() < 8){
			tope = tssm.lgSeries.size();
		}
		for(int i=0; i<tope; i++){
			cout << "[" << i << "]\t" << show_first_elements(tssm.lgSeries[i], 5) << endl;
		}
	}else{
		cout << "\t no elements" << endl;
	}

	cout << " -------------------------------------------------------- " << endl;

	// Calculando promedio en datos de estructura
	long long int acumDE = 0;
	int cantDE = 0;
	for(int i=0; i<tssm.pgFirstValue.size(); i++){
		acumDE += tssm.pgFirstValue[i];
		cantDE++;
	}
	
	for(int i=0; i<tssm.lgFirstValue.size(); i++){
		acumDE += tssm.lgFirstValue[i];
		cantDE++;
	}
	
	for(int i=0; i<tssm.pgReference.size(); i++){
		for(int j=0; j<tssm.pgReference[i].size(); j++){
			acumDE += tssm.pgReference[i][j];
			cantDE++;
		}
	}

	for(int i=0; i<tssm.pgSeries.size(); i++){
		for(int j=0; j<tssm.pgSeries[i].size(); j++){
			acumDE += tssm.pgSeries[i][j];
			cantDE++;
		}
	}

	for(int i=0; i<tssm.lgSeries.size(); i++){
		for(int j=0; j<tssm.lgSeries[i].size(); j++){
			acumDE += tssm.lgSeries[i][j];
			cantDE++;
		}
	}
	
	double promDE = (0.0 + acumDE) / cantDE;
	cout << "Promedio en datos de estructura: " << promDE << endl;
	cout << "Cantidad de elementos considerados: " << cantDE << endl;

	return 0;

}

string show_array(vector<int> x){
	string s = "";
	for(int i=0; i<x.size(); i++){
		s += "\t" + to_string(x[i]);
	}
	return s;
}

string show_first_elements(vlc_vector<> x, int n){
	string s = "";
	if(x.size() < n){
		if(x.size() == 0){
			return s;		
		}
		n= x.size();
	}
	for(int i=0; i<n; i++){
		s += "\t" + to_string(x[i]);
	}
	return s;
}

