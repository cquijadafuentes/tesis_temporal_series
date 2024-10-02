#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_regionquery_file> <output_windowquery_file> <output_getseriequery_file>" << endl;
		cout << "NOTA: este código toma como entrada los archivos generados para region_query del temporal k2-raster." << endl;
		return 0;
	}
	ifstream iqf(argv[1]);
	int queries;
	iqf >> queries;
	int xi, xf, yi, yf, ti, tf, vi, vf;
	ofstream windowq(argv[2]);
	ofstream getserieq(argv[3]);
	windowq << queries << endl;
	getserieq << queries << endl;
	for(int i=0; i<queries; i++){
		iqf >> xi >> xf >> yi >> yf >> ti >> tf >> vi >> vf;
		// Se valida el tf <= 2663
		if(ti > 2663) ti = 2663;
		if(tf > 2663) tf = 2663;
		windowq << xi << " " << xf;
		windowq << " ";
		windowq << yi << " " << yf;
		windowq << " ";
		windowq << ti << " " << tf;
		windowq << " ";
		windowq << vi << " " << vf;
		windowq << endl;
		// Se establece como ti y tf los valores límite del dataset
		ti = 0;
		tf = 2663;
		getserieq << xi << " " << xf;
		getserieq << " ";
		getserieq << yi << " " << yf;
		getserieq << " ";
		getserieq << ti << " " << tf;
		getserieq << " ";
		getserieq << vi << " " << vf;
		getserieq << endl;
	}
	return 0;
}
