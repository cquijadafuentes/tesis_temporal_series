#include "TempRaster_RowComp.hpp"

using namespace std;
using namespace sdsl;

string mostrarserie(vector<int> v){
	string concat = "";
	if(v.size() > 0){
		concat += to_string(v[0]);
		for(int i=1; i<v.size(); i++){
			concat += " " + to_string(v[i]);
		}
	}
	return concat;
}

void informacion_datos(TempRasterRowComp& nqc, string fn){
	cout << "\tFilename: " << fn << endl;
	cout << "\tFilas: " << nqc.n_rows << endl;
	cout << "\tColumnas: " << nqc.n_cols << endl;
	cout << "\tTotal celdas (FxC): " << (nqc.n_rows * nqc.n_cols) << endl;
	cout << "\tInstantes: " << nqc.n_inst << endl;
	cout << "\tTotal datos (FxCxI): " << (nqc.n_rows * nqc.n_cols * nqc.n_inst) << endl;
	cout << "\tMin: " << nqc.min_value << " - Max: " << nqc.max_value << endl;
	cout << "\tDimension cuadrantes: " << nqc.d_quad << endl;
	cout << "\tFilas de cuadrantes: " << nqc.nQuadRows << endl;
	cout << "\tColumnas de cuadrantes: " << nqc.nQuadCols << endl;
	cout << "\tCeldas por cuadrantes: " << nqc.cel_per_quad << endl;
	cout << "\tTotal cuadrantes (FQxCQ): " << (nqc.nQuadRows * nqc.nQuadCols) << endl;
	cout << "\tTamaño del archivo: " << nqc.size_kbytes() << " [KB]" << endl;
	cout << "\tTamaño del archivo: " << nqc.size_mbytes() << " [MB]" << endl;	
}

void informacion_estructura(TempRasterRowComp& nqc){
	cout << "\tfixedValue: " << nqc.fixedValue.size() << endl;
	cout << "\trefFirstValue: " << nqc.refFirstValue.size() << endl;
	cout << "\trefs: " << nqc.refs.size() << endl;
	cout << "\tseries: " << nqc.series.size() << endl;
	cout << "\tbvQuadNoFijos: " << nqc.bvQuadNoFijos.size();
	cout << " - rank(): " << nqc.rankQuadNoFijos(nqc.bvQuadNoFijos.size()) << endl;
	cout << "\tbvReferencias: " << nqc.bvReferencias.size();
	cout << " - rank(): " << nqc.rankReferencias(nqc.bvReferencias.size()) << endl;
	cout << "\tbvSeriesFijas: " << nqc.bvSeriesFijas.size();
	cout << " - rank(): " << nqc.rankSeriesFijas(nqc.bvSeriesFijas.size()) << endl;
}

void getAccess(TempRasterRowComp& nqc){
	cout << "Ingrese fila columna e instante separados por espacio." << endl;
	int f,c,i;
	cin >> f >> c >> i ;
	if(f >= nqc.n_rows || f < 0){
		cout << "Error en la fila" << endl;
		return;
	}
	if(c >= nqc.n_cols || c < 0){
		cout << "Error en la columna" << endl;
		return;
	}
	if(i >= nqc.n_inst || i < 0){
		cout << "Error en el instante" << endl;
		return;
	}
	int r = nqc.queryAccess(f,c,i);
	cout << "access(f:" << f << " , c:" << c << " , i:" << i << "): " << r << endl;
}

void getTimeSerie(TempRasterRowComp& nqc){
	cout << "Ingrese fila y columna separados por espacio." << endl;
	int f,c;
	cin >> f >> c ;
	if(f >= nqc.n_rows || f < 0){
		cout << "Error en la fila" << endl;
		return;
	}
	if(c >= nqc.n_cols || c < 0){
		cout << "Error en la columna" << endl;
		return;
	}
	vector<int> r = nqc.getSerie(f,c);
	cout << "Serie(f:" << f << " , c:" << c << "): " << endl << "\t";
	for(int i=0; i<r.size(); i++){
		cout << r[i] << " ";
	}
	cout << endl;
}

int show_menu(){
	int opc;
	do{
		cout << "************************ MENU ************************" << endl;
		cout << "1- Información." << endl;
		cout << "2- Información estructura." << endl;
		cout << "3- Access." << endl;
		cout << "4- Time Serie." << endl;
		cout << "5- Window Query." << endl;
		cout << "6- Range Query." << endl;
		cout << endl;
		cout << "0- SALIR." << endl;
		cout << "******************************************************" << endl;
		cin >> opc;
	}while(opc < 0 || opc > 4);
	return opc;
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <NQC_file>" << endl;
		return 0;
	}
	string nqcFilename(argv[1]);
	TempRasterRowComp nqc(nqcFilename);

	cout << "NQC File: " << argv[1] << endl;

	int opc = 0;
	do{
		opc = show_menu();

		switch(opc){
			case (0):
				cout << "Saliendo..." << endl;
				return 0;
			case (1):
				informacion_datos(nqc, string(argv[1]));
				break;
			case (2):
				informacion_estructura(nqc);
				break;
			case (3):
				getAccess(nqc);
				break;
			case (4):
				getTimeSerie(nqc);
				break;			
		default:
				cout << "Función aún sin implementar..." << endl;
		}
	}while(opc != 0);
	
	return 0;
}