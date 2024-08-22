
#include "NASA_QuadComp.hpp"


NASAQuadComp::NASAQuadComp(vector<vector<vector<int>>> tseries, int dims){
	int rows = tseries.size();			// filas
	int cols = tseries[0].size();		// columnas
	int lts = tseries[0][0].size();	// length of temporal series
	int cuadRows = rows / dims;
	if(cuadRows*dims < rows){
		cuadRows++;
	}
	int cuadCols = cols / dims;
	if(cuadCols*dims < cols){
		cuadCols++;
	}
	

	int_vector<> firstValue(cuadRows * cuadCols);
	int iFV = 0;
	long long int bytesTodas = 0;
	for(int fc=0; fc<cuadRows; fc++){		// fc = fila-cuadrante
		for(int cc=0; cc<cuadCols; cc++){	// cc = columna-cuadrante
			// Procesando primera series del cuadrante
			int fref = fc*dims;
			int cref = cc*dims;
			int val = tseries[fref][cref][0];
			firstValue[iFV++] = zigzag_encode(val);
			int_vector<> ivaux(lts-1);
			for(int k=1; k<lts; k++){
				val = tseries[fref][cref][k] - tseries[fref][cref][k-1];
				ivaux[k-1] = zigzag_encode(val);
			}
			util::bit_compress(ivaux);
			bytesTodas += size_in_bytes(ivaux);
			// Procesando el resto de las series del cuadrante
			for(int i=0; i<dims && fref+i < rows; i++){
				for(int j=0; j<dims && cref+j < cols; j++){
					if(i!=0 || j!=0){
						// NO es la primera celda (que ya esta considerada)
						int_vector<> ivaux2(lts);
						for(int k=0; k<lts; k++){
							val = tseries[fref][cref][k] - tseries[fref+i][cref+j][k];
							ivaux2[k] = zigzag_encode(val);
						}
						util::bit_compress(ivaux2);
						bytesTodas += size_in_bytes(ivaux2);
					}
				}
			}
		}
	}
	util::bit_compress(firstValue);
	bytesTodas += size_in_bytes(firstValue);
	int megaBytes = ((bytesTodas + 0.0) / 1024) / 1024;

	cout << "\t" << megaBytes << "\t" << dims << "\t" << cuadRows << "\t" << cuadCols << endl;
}

unsigned int NASAQuadComp::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

unsigned int NASAQuadComp::zigzag_decode(int i){
	 return ((i >> 1) ^ -(i & 1));
}