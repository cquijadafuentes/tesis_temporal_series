
#include "NASA_QuadComp.hpp"


long long int bytes_int_vector(vector<int> x){
	int_vector<> iv_x(x.size());
	for(int i=0; i<x.size(); i++){
		iv_x[i] = x[i];
	}
	util::bit_compress(iv_x);
	return size_in_bytes(iv_x);
}

int bytes_bit_vector(bit_vector b){
	sd_vector<> sdb(b);
	return size_in_bytes(b);
}


NASAQuadComp::NASAQuadComp(vector<vector<vector<int>>> &tseries, int dims){
	int n_rows = tseries.size();			// filas
	int n_cols = tseries[0].size();		// columnas
	int n_inst = tseries[0][0].size();	// length of temporal series
	d_quad = dims;
	// Cantidad de filas y columnas de cuadrantes según las dimensiones dims.
	int nQuadRows = n_rows / d_quad;
	if(nQuadRows*d_quad < n_rows){
		nQuadRows++;
	}
	int nQuadCols = n_cols / d_quad;
	if(nQuadCols*d_quad < n_cols){
		nQuadCols++;
	}
	
	cout << "nQuadRows: " << nQuadRows << " - nQuadCols: " << nQuadCols << endl;

	bit_vector bvQSR = bit_vector(nQuadRows * nQuadCols);	// Marcar cuadrantes con Serie de referencia
	int cantCeldasQuad = (d_quad * d_quad) * (nQuadRows * nQuadCols);
	bit_vector bvSF = bit_vector(cantCeldasQuad);			// Marcar las series fijas
	bit_vector bvSR = bit_vector(cantCeldasQuad);			// Marcar la serie de referencia

	vector<int> valoresFV;		// Guarda el primer valor de cada serie de referencia
	vector<int> valoresSF;		// Guarda el valor de cada serie fija
	int iFV = 0;
	long long int bytesTodas = 0;
	for(int fc=0; fc<nQuadRows; fc++){		// fc = fila-cuadrante
		for(int cc=0; cc<nQuadCols; cc++){	// cc = columna-cuadrante
			// Procesando primera series del cuadrante
			int f0c = fc*d_quad;		// Fila cero (0) del cuadrante
			int c0c = cc*d_quad;		// Columna cero (0) del cuadrante
			int iQuad = (fc * nQuadCols) + cc;	// Índice del cuadrante en row-major
			// Procesando el resto de las series del cuadrante
			vector<int> serieReferencia;
			for(int df=0; df<d_quad && f0c+df < n_rows; df++){		// delta-filas en celdas
				for(int dc=0; dc<d_quad && c0c+dc < n_cols; dc++){		// delta-columnas en celdas
					int posF = f0c+df;		// Fila de la celda en evaluación
					int posC = c0c+dc;		// Columna de la celda en evaluación
					int iCelda = getQuadLinealPosition(posF, posC);
					int val;
					if(esFija(tseries[posF][posC])){
						// La serie de tiempo es fija y no se representa
						bvSF[iCelda] = 1;
						valoresSF.push_back(tseries[posF][posC][0]);
					}else if(bvQSR[iQuad] == 0){
						// NO hay serie de referencia
						serieReferencia = tseries[posF][posC];
						valoresFV.push_back(tseries[posF][posC][0]);
						int_vector<> ivaux(n_inst-1);
						for(int k=1; k<n_inst; k++){
							val = tseries[posF][posC][k] - tseries[posF][posC][k-1];
							ivaux[k-1] = zigzag_encode(val);
						}
						util::bit_compress(ivaux);
						bytesTodas += size_in_bytes(ivaux);
						bvQSR[iQuad] = 1;
						bvSR[iCelda] = 1;
					}else{
						// SI hay serie de referencia
						int_vector<> ivaux2(n_inst);
						for(int k=0; k<n_inst; k++){
							val = serieReferencia[k] - tseries[posF][posC][k];
							ivaux2[k] = zigzag_encode(val);
						}
						util::bit_compress(ivaux2);
						bytesTodas += size_in_bytes(ivaux2);
					}
				}
			}
		}
	}
	bytesTodas += bytes_int_vector(valoresFV);
	bytesTodas += bytes_int_vector(valoresSF);
	bytesTodas += bytes_bit_vector(bvQSR);
	bytesTodas += bytes_bit_vector(bvSF);
	bytesTodas += bytes_bit_vector(bvSR);

	int megaBytes = ((bytesTodas + 0.0) / 1024) / 1024;

	cout << megaBytes << "\t" << d_quad << "\t" << nQuadRows << "\t" << nQuadCols << endl;
}

unsigned int NASAQuadComp::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

unsigned int NASAQuadComp::zigzag_decode(int i){
	 return ((i >> 1) ^ -(i & 1));
}

bool NASAQuadComp::esFija(vector<int> serie){
	for(int j=1; j<serie.size(); j++){
		if(serie[j] != serie[j-1]){
			return false;
		}
	}
	return true;
}

unsigned int NASAQuadComp::getQuad(int r, int c){
	int rquad = r / d_quad;
	int cquad = c / d_quad;
	int iquad = (rquad * nQuadCols) + cquad;
	return iquad;
}

unsigned int NASAQuadComp::getQuadLinealPosition(int r, int c){
	int iQuad = getQuad(r,c);
	int dr = r % d_quad;
	int dc = c % d_quad;
	int cxQuad = d_quad * d_quad;
	int quadLinealPos = (iQuad * cxQuad) + (dr * d_quad) + dc;
	return quadLinealPos;
}
