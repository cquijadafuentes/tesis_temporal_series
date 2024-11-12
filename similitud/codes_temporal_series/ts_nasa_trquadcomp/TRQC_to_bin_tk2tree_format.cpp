#include "TempRaster_QuadComp.hpp"

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	if(argc < 4){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <NQC_file> <Output_Folder> <prefixfilenames>" << endl;
		return 0;
	}
	// Leyendo datos desde el archivo de entrada
	string nqcFilename(argv[1]);
	TempRasterQuadComp nqc(nqcFilename);
	vector<vector<vector<int>>> data(nqc.n_rows, vector<vector<int>>(nqc.n_cols, vector<int>(nqc.n_inst, 0)));
	for(int i=0; i<nqc.n_rows; i++){
		for(int j=0; j<nqc.n_cols; j++){
			data[i][j] = nqc.getSerie(i,j);
		}
	}

	string folder(argv[2]);
	string prefixname(argv[3]);
	string oFilename(folder+prefixname+"_lista.txt");
	cout << oFilename << endl;
	ofstream listFile(oFilename);
	listFile << nqc.n_cols << " " << nqc.n_rows << " 4 2 4 2" << endl;
	for(int i=0; i<nqc.n_inst; i++){
	//for(int i=0; i<5; i++){
		string boutfilename(prefixname+"_"+to_string(i)+".bin");
		listFile << boutfilename << endl;
		ofstream binout(folder+boutfilename);
		for(int c = 0; c < nqc.n_cols; c++){
			for(int f = 0; f < nqc.n_rows; f++){
				binout.write((char const*)&data[f][c][i], sizeof(int));
			}
		}
	}
	listFile.close();
	return 0;
}