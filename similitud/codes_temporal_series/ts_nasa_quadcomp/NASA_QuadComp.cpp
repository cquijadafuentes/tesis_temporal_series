
#include "NASA_QuadComp.hpp"


NASAQuadComp::NASAQuadComp(vector<vector<vector<int>>> &tseries, int dims){
	n_rows = tseries.size();			// filas
	n_cols = tseries[0].size();		// columnas
	n_inst = tseries[0][0].size();	// length of temporal series
	d_quad = dims;
	// Cantidad de filas y columnas de cuadrantes según las dimensiones dims.
	nQuadRows = n_rows / d_quad;
	if(nQuadRows*d_quad < n_rows){
		nQuadRows++;
	}
	nQuadCols = n_cols / d_quad;
	if(nQuadCols*d_quad < n_cols){
		nQuadCols++;
	}

	min_value = tseries[0][0][0];
	max_value = tseries[0][0][0];
	for(int i=0; i<n_rows; i++){
		for(int j=0; j<n_cols; j++){
			for(int t=0; t<n_inst; t++){
				if(tseries[i][j][t] < min_value){
					min_value = tseries[i][j][t];
				}
				if(tseries[i][j][t] > max_value){
					max_value = tseries[i][j][t];
				}
			}
		}
	}
	cout << "min_value: " << min_value << " - max_value: " << max_value << endl;

	cout << "Construyendo ..." << endl;

	bit_vector bvQSR = bit_vector(nQuadRows * nQuadCols);	// Marcar cuadrantes con Serie de referencia
	int cantCeldasQuad = (d_quad * d_quad) * (nQuadRows * nQuadCols);
	bit_vector bvSR = bit_vector(cantCeldasQuad);			// Marcar la serie de referencia
	bit_vector bvSF = bit_vector(cantCeldasQuad);			// Marcar las series fijas

	cout << "nQuadRows: " << nQuadRows << " - nQuadCols: " << nQuadCols << endl;
	cout << "d_quad: " << d_quad << " - cantQuads: " << (nQuadRows*nQuadCols) << endl;
	cout << "Total celdas en cuadrantes: " << cantCeldasQuad << endl;

	vector<int> valoresPVSR;		// Guarda el primer valor de cada serie de referencia
	vector<int> valoresSF;		// Guarda el valor de cada serie fija
	int iFV = 0;
	for(int fc=0; fc<nQuadRows; fc++){		// fc = fila-cuadrante
		for(int cc=0; cc<nQuadCols; cc++){	// cc = columna-cuadrante
			// Procesando primera series del cuadrante
			int f0c = fc*d_quad;		// Fila cero (0) del cuadrante
			int c0c = cc*d_quad;		// Columna cero (0) del cuadrante
			int iQuad = (fc * nQuadCols) + cc;	// Índice del cuadrante en row-major
			// Procesando el resto de las series del cuadrante
			vector<int> serieReferencia(n_inst);
			for(int df=0; df<d_quad && f0c+df < n_rows; df++){		// delta-filas en celdas
				for(int dc=0; dc<d_quad && c0c+dc < n_cols; dc++){		// delta-columnas en celdas
					int posF = f0c+df;		// Fila de la celda en evaluación
					int posC = c0c+dc;		// Columna de la celda en evaluación
					int iCelda = getQuadLinealPosition(posF, posC);
					int val;
					if(esFija(tseries[posF][posC])){
						// La serie de tiempo es fija y no se representa
						valoresSF.push_back(tseries[posF][posC][0]);
						bvSF[iCelda] = 1;
						bvSR[iCelda] = 0;
					}else if(bvQSR[iQuad] == 0){
						// NO hay serie de referencia
						for(int t=0; t<n_inst; t++){
							serieReferencia[t] = tseries[posF][posC][t];
						}
						valoresPVSR.push_back(tseries[posF][posC][0]);
						int_vector<> ivaux(n_inst-1);
						for(int k=1; k<n_inst; k++){
							val = tseries[posF][posC][k] - tseries[posF][posC][k-1];
							ivaux[k-1] = zigzag_encode(val);
						}
						util::bit_compress(ivaux);
						refs.push_back(ivaux);
						bvQSR[iQuad] = 1;
						bvSR[iCelda] = 1;
						bvSF[iCelda] = 0;
					}else{
						// SI hay serie de referencia
						int_vector<> ivaux2(n_inst);
						for(int k=0; k<n_inst; k++){
							val = serieReferencia[k] - tseries[posF][posC][k];
							ivaux2[k] = zigzag_encode(val);
						}
						util::bit_compress(ivaux2);
						series.push_back(ivaux2);
						bvSF[iCelda] = 0;
					}
				}
			}
		}
	}

	fixedValue = int_vector<>(valoresSF.size());
	for(int i=0; i<valoresSF.size(); i++){
		fixedValue[i] = valoresSF[i] - min_value;
	}
	util::bit_compress(fixedValue);

	refFirstValue = int_vector<>(valoresPVSR.size());
	for(int i=0; i<valoresPVSR.size(); i++){
		refFirstValue[i] = valoresPVSR[i] - min_value;
	}
	util::bit_compress(refFirstValue);

	bvQuadNoFijos = sd_vector<>(bvQSR);
	bvReferencias = sd_vector<>(bvSR);
	bvSeriesFijas = sd_vector<>(bvSF);

	buildRanksSelects();

	cout << "Size in kBytes: " << size_kbytes() << endl;
}

NASAQuadComp::NASAQuadComp(string inputFilename){
	ifstream infile(inputFilename, ofstream::binary);
	if(!infile){
		cout << "Error en la carga!" << endl;
		return;
	}    
	int aux1, aux2;
	// --------- Cargando valores enteros
	infile.read((char *)&n_rows, sizeof(int));
	infile.read((char *)&n_cols, sizeof(int));
	infile.read((char *)&n_inst, sizeof(int));
	infile.read((char *)&d_quad, sizeof(int));
	infile.read((char *)&nQuadRows, sizeof(int));
	infile.read((char *)&nQuadCols, sizeof(int));
	infile.read((char *)&min_value, sizeof(int));
	infile.read((char *)&max_value, sizeof(int));
	// --------- Cargando int_vectors
	fixedValue.load(infile);
	refFirstValue.load(infile);
	// --------- Cargando vectores de int_vectors
	int_vector<> tempIV;
	infile.read((char *)&aux1,sizeof(int));
	refs = vector<int_vector<>>(aux1);
	for(int i=0; i<aux1; i++){
		tempIV.load(infile);
		util::bit_compress(tempIV);
		refs[i] = tempIV;
	}
	infile.read((char *)&aux1,sizeof(int));
	series = vector<int_vector<>>(aux1);
	for(int i=0; i<aux1; i++){
		tempIV.load(infile);
		util::bit_compress(tempIV);
		series[i] = tempIV;
	}
	// --------- Cargando bitvectors
	bvQuadNoFijos.load(infile);
	bvReferencias.load(infile);
	bvSeriesFijas.load(infile);
	// Cerrando archivo
	infile.close();
	// Construyendo rank y select de MFS
	buildRanksSelects();

	cout << "Leído..." << endl;
}

bool NASAQuadComp::save(string outputFilename){
	ofstream outfile(outputFilename, ofstream::binary);
	// Guardando valores enteros
	outfile.write((char const*)&n_rows, sizeof(int));
	outfile.write((char const*)&n_cols, sizeof(int));
	outfile.write((char const*)&n_inst, sizeof(int));
	outfile.write((char const*)&d_quad, sizeof(int));
	outfile.write((char const*)&nQuadRows, sizeof(int));
	outfile.write((char const*)&nQuadCols, sizeof(int));
	outfile.write((char const*)&min_value, sizeof(int));
	outfile.write((char const*)&max_value, sizeof(int));
	// Guardando int_vectors
	fixedValue.serialize(outfile);
	refFirstValue.serialize(outfile);
	// Guardando vectores de int_vectors
	int aux;
	aux = refs.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<aux; i++){
		refs[i].serialize(outfile);
	}
	aux = series.size();
	outfile.write((char const*)&aux, sizeof(int));
	for(int i=0; i<aux; i++){
		series[i].serialize(outfile);
	}
	// Guardando bitvectors
	bvQuadNoFijos.serialize(outfile);
	bvReferencias.serialize(outfile);
	bvSeriesFijas.serialize(outfile);
	// Cerrando archivo
	outfile.close();
	return true;
}

int NASAQuadComp::size_bytes(){
	int bytes = 0;
	// Atributos enteros
	bytes += (sizeof(int) * 8);
	// int_vectors
	bytes += size_in_bytes(fixedValue);
	bytes += size_in_bytes(refFirstValue);
	for(int i=0; i<refs.size(); i++){
		bytes += size_in_bytes(refs[i]);
	}
	for(int i=0; i<series.size(); i++){
		bytes += size_in_bytes(series[i]);
	}
	// bit_vectors
	bytes += size_in_bytes(bvQuadNoFijos);
	bytes += size_in_bytes(bvReferencias);
	bytes += size_in_bytes(bvSeriesFijas);
	return bytes;
}

int NASAQuadComp::size_kbytes(){
	int kbytes = size_bytes() / 1024;
	return kbytes;
}

int NASAQuadComp::size_mbytes(){
	int mbytes = size_bytes() / 1024 / 1024;
	return mbytes;
}

vector<int> NASAQuadComp::getSerie(int row, int col){
	vector<int> r;
	// 1 - Verificar que no corresponde a una serie fija
	int posQLP = getQuadLinealPosition(row, col);
	if(bvSeriesFijas[posQLP] == 1){
		cout << "f ";
		int p = rankSeriesFijas(posQLP);
		int v = fixedValue[p] + min_value;
		r = vector<int>(n_inst, v);
		return r;
	}
	// 2 - Recuperar la serie de referencia del cuadrante
	vector<int> ref = getReferenciaQuad(getQuad(row, col));
	if(bvReferencias[posQLP] == 1){
		return ref;
	}
	// 3 - Si no es la referencia, recuperar la serie
	cout << "n ";
	vector<int> ref = getReferenciaQuad(getQuad(row, col));
	cout << "referencia: ";
	for(int i=0; i<n_inst; i++){
		cout << ref[i] << " ";
	}
	cout << endl;
	int posSerie = getSeriePositionFromQLP(posQLP);
	cout << "posQLP: " << posQLP << endl;
	cout << "posSerie: " << posSerie << endl;

	return r;
}

void NASAQuadComp::pruebas(){

}

/***********************************************************
							PRIVATE
***********************************************************/

void NASAQuadComp::buildRanksSelects(){
	rankQuadNoFijos = sd_vector<>::rank_1_type(&bvQuadNoFijos);
	rankReferencias = sd_vector<>::rank_1_type(&bvReferencias);
	rankSeriesFijas = sd_vector<>::rank_1_type(&bvSeriesFijas);
	selectReferencias = sd_vector<>::select_1_type(&bvReferencias);
}

unsigned int NASAQuadComp::zigzag_encode(int i){
	return ((i >> 31) ^ (i << 1));
}

int NASAQuadComp::zigzag_decode(int i){
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
	unsigned int rquad = r / d_quad;
	unsigned int cquad = c / d_quad;
	unsigned int iquad = (rquad * nQuadCols) + cquad;
	return iquad;
}

unsigned int NASAQuadComp::getQuadLinealPosition(int r, int c){
	unsigned int iQuad = getQuad(r,c);
	unsigned int dr = r % d_quad;
	unsigned int dc = c % d_quad;
	unsigned int cxQuad = d_quad * d_quad;
	unsigned int quadLinealPos = (iQuad * cxQuad) + (dr * d_quad) + dc;
	return quadLinealPos;
}

unsigned int NASAQuadComp::getSeriePositionFromQLP(int qlp){
	unsigned int iQuad = qlp / (d_quad * d_quad);
	unsigned int cantQuadRowsPrevious = iQuad / nQuadRows;
	unsigned int rowsToDismiss = cantQuadRowsPrevious * d_quad;
	unsigned int dCeldas = n_rows % d_quad;
	unsigned seriePos = qlp - (rowsToDismiss * dCeldas);
	int rFijas = rankSeriesFijas(qlp);
	int rRefs = rankReferencias(qlp);
	seriePos = seriePos - rFijas - rRefs;
	return seriePos;
}

unsigned int NASAQuadComp::getRefPositionFromQLP(int qlp){
	unsigned int iQuad = qlp / (d_quad * d_quad);
	unsigned int cantQuadRowsPrevious = iQuad / nQuadRows;
	unsigned int rowsToDismiss = cantQuadRowsPrevious * d_quad;
	unsigned int dCeldas = n_rows % d_quad;
	unsigned seriePos = qlp - (rowsToDismiss * dCeldas);
	return seriePos;
}

vector<int> NASAQuadComp::getReferenciaQuad(int iQuad){
	int cantQCR = rankQuadNoFijos(iQuad);
	int posQLP = selectReferencias(cantQCR+1);
	cout << "posQLP" << endl;
	vector<int> r(n_inst);
	int rFijas = rankSeriesFijas(posQLP);
	int rRefs = rankReferencias(posQLP);
	r[0] = refFirstValue[rRefs] + min_value;
	int p =  getRefPositionFromQLP(posQLP) - rFijas - rRefs;
	for(int i=0; i<n_inst-1; i++){
		r[i+1] = r[i] + zigzag_decode(refs[p][i]);
	}
	return r;
}