#include <iostream>
#include <fstream>

using namespace std;

int swapCalls = 0;

void swap(int &a, int &b){
	int x = a;
	a = b;
	b = x;
	swapCalls++;
}

int main(int argc, char const *argv[]){
	if(argc < 2){
		cout << "Error! Faltan argumentos." << endl;
		cout << argv[0] << " <input_regionquery_file>" << endl;
		cout << "NOTA: este código toma como entrada los archivos generados para region_query del temporal k2-raster." << endl;
		return 0;
	}
	ifstream iqf(argv[1]);
	int queries;
	iqf >> queries;
	int xi, xf, yi, yf, ti, tf, vi, vf;
	int minx, maxx, miny, maxy, mint, maxt, minv, maxv;
	iqf >> minx >> maxx >> miny >> maxy >> mint >> maxt >> minv >> maxv;
	for(int i=1; i<queries; i++){
		iqf >> xi >> xf >> yi >> yf >> ti >> tf >> vi >> vf;
		// Revisando X
		if(xi > xf) swap(xi, xf);
		if(xi < minx) minx = xi;
		if(xf > maxx) maxx = xf;
		// Revisando Y
		if(yi > yf) swap(yi, yf);
		if(yi < miny) miny = yi;
		if(yf > maxy) maxy = yf;
		// Revisando T
		if(ti > tf) swap(ti, tf);
		if(ti < mint) mint = ti;
		if(tf > maxt) maxt = tf;
		// Revisando V
		if(vi > vf) swap(vi, vf);
		if(vi < minv) minv = vi;
		if(vf > maxv) maxv = vf;
	}
	cout << "Rango x:[" << minx << "," << maxx << "]" << endl;
	cout << "Rango y:[" << miny << "," << maxy << "]" << endl;
	cout << "Rango t:[" << mint << "," << maxt << "]" << endl;
	cout << "Rango v:[" << minv << "," << maxv << "]" << endl;
	cout << "Cantidad de llamadas a swap: " << swapCalls << endl;
	return 0;
}
