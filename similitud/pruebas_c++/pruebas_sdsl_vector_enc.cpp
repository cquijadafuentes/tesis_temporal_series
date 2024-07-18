#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sdsl/vectors.hpp>

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	int num = 10000;
	if(argc > 1){
		int x = stoi(argv[1]);
		if(x > num){
			num = x;
		}
	}
//	-------------------------------------------------
	int_vector<> v(num);
	generate(v.begin(), v.end(), rand);
	cout << "v:";
	for(int i=0; i<10; i++){
		cout << "\t" << v[i] ;
	}
	cout << "\t..." << endl;
//	-------------------------------------------------
	int_vector<> w(v.size());
	for(int i=0; i<w.size(); i++){
		w[i] = v[i] % 10;
	}
	cout << "w:";
	for(int i=0; i<10; i++){
		cout << "\t" << w[i] ;
	}
	cout << "\t..." << endl;
//	-------------------------------------------------
	int deci = v.size() / 10;
	int_vector<> x(v.size());
	for(int i=0; i<x.size(); i++){
		x[i] = i%deci;
	}
	cout << "x:";
	for(int i=0; i<10; i++){
		cout << "\t" << x[i] ;
	}
	cout << "\t..." << endl;
//	-------------------------------------------------
	int_vector<> y(v.size());
	for(int i=0; i<y.size(); i++){
		y[i] = (v[i] % 10) - 5;
	}
	cout << "y:";
	for(int i=0; i<10; i++){
		cout << "\t" << y[i] ;
	}
	cout << "\t..." << endl;
//	-------------------------------------------------
	int_vector<> z(v.size());
	for(int i=0; i<y.size(); i++){
		z[i] = v[i] % 3000;
	}
	cout << "z:";
	for(int i=0; i<10; i++){
		cout << "\t" << z[i] ;
	}
	cout << "\t..." << endl;
//	-------------------------------------------------
	cout << "\tv.size(): " << v.size() << endl;
	cout << "\tw.size(): " << w.size() << endl;
	cout << "\tx.size(): " << x.size() << endl;
	cout << "\ty.size(): " << y.size() << endl;
	cout << "\ty.size(): " << z.size() << endl;

	cout << "-----+-----+-----+-----+-----+-----+-----+-----+-----+-----" << endl;

	cout << "compression\tv mem[KB]\tw mem[KB]\tx mem[KB]\ty mem[KB]\tz mem[KB]" << endl;
	cout << "no compress";
	cout << "\t" << (int)(size_in_bytes(v)/1024);
	cout << "\t" << (int)(size_in_bytes(w)/1024);
	cout << "\t" << (int)(size_in_bytes(x)/1024);
	cout << "\t" << (int)(size_in_bytes(y)/1024);
	cout << "\t" << (int)(size_in_bytes(z)/1024);
	cout << endl;
	
	util::bit_compress(v);
	util::bit_compress(w);
	util::bit_compress(x);
	util::bit_compress(y);
	util::bit_compress(z);
	cout << "bitcompress";
	cout << "\t" << (int)(size_in_bytes(v)/1024);
	cout << "\t" << (int)(size_in_bytes(w)/1024);
	cout << "\t" << (int)(size_in_bytes(x)/1024);
	cout << "\t" << (int)(size_in_bytes(y)/1024);
	cout << "\t" << (int)(size_in_bytes(z)/1024);
	cout << endl;
	
	enc_vector<> evv(v);
	enc_vector<> evw(w);
	enc_vector<> evx(x);
	enc_vector<> evy(y);
	enc_vector<> evz(z);
	cout << "enc_v (def)";
	cout << "\t" << (int)(size_in_bytes(evv)/1024);
	cout << "\t" << (int)(size_in_bytes(evw)/1024);
	cout << "\t" << (int)(size_in_bytes(evx)/1024);
	cout << "\t" << (int)(size_in_bytes(evy)/1024);
	cout << "\t" << (int)(size_in_bytes(evz)/1024);
	cout << endl;
	
	enc_vector<coder::elias_delta> evDv(v);
	enc_vector<coder::elias_delta> evDw(w);
	enc_vector<coder::elias_delta> evDx(x);
	enc_vector<coder::elias_delta> evDy(y);
	enc_vector<coder::elias_delta> evDz(z);
	cout << "enc_v eliasD";
	cout << "\t" << (int)(size_in_bytes(evDv)/1024);
	cout << "\t" << (int)(size_in_bytes(evDw)/1024);
	cout << "\t" << (int)(size_in_bytes(evDx)/1024);
	cout << "\t" << (int)(size_in_bytes(evDy)/1024);
	cout << "\t" << (int)(size_in_bytes(evDz)/1024);
	cout << endl;
	
	enc_vector<coder::elias_gamma> evGv(v);
	enc_vector<coder::elias_gamma> evGw(w);
	enc_vector<coder::elias_gamma> evGx(x);
	enc_vector<coder::elias_gamma> evGy(y);
	enc_vector<coder::elias_gamma> evGz(z);
	cout << "enc_v eliasG";
	cout << "\t" << (int)(size_in_bytes(evGv)/1024);
	cout << "\t" << (int)(size_in_bytes(evGw)/1024);
	cout << "\t" << (int)(size_in_bytes(evGx)/1024);
	cout << "\t" << (int)(size_in_bytes(evGy)/1024);
	cout << "\t" << (int)(size_in_bytes(evGz)/1024);
	cout << endl;
	
	enc_vector<coder::fibonacci> evFv(v);
	enc_vector<coder::fibonacci> evFw(w);
	enc_vector<coder::fibonacci> evFx(x);
	enc_vector<coder::fibonacci> evFy(y);
	enc_vector<coder::fibonacci> evFz(z);
	cout << "enc_v Fibo";
	cout << "\t" << (int)(size_in_bytes(evFv)/1024);
	cout << "\t" << (int)(size_in_bytes(evFw)/1024);
	cout << "\t" << (int)(size_in_bytes(evFx)/1024);
	cout << "\t" << (int)(size_in_bytes(evFy)/1024);
	cout << "\t" << (int)(size_in_bytes(evFz)/1024);
	cout << endl;
	
	vlc_vector<> vlcv(v);
	vlc_vector<> vlcw(w);
	vlc_vector<> vlcx(x);
	vlc_vector<> vlcz(z);
	cout << "vlc_v (def)";
	cout << "\t" << (int)(size_in_bytes(vlcv)/1024);
	cout << "\t" << (int)(size_in_bytes(vlcw)/1024);
	cout << "\t" << (int)(size_in_bytes(vlcx)/1024);
	cout << "\tNAN";
	cout << "\t" << (int)(size_in_bytes(vlcz)/1024);
	cout << endl;
	
	vlc_vector<coder::elias_delta> vlcDv(v);
	vlc_vector<coder::elias_delta> vlcDw(w);
	vlc_vector<coder::elias_delta> vlcDx(x);
	vlc_vector<coder::elias_delta> vlcDz(z);
	cout << "vlc_v eliasD";
	cout << "\t" << (int)(size_in_bytes(vlcDv)/1024);
	cout << "\t" << (int)(size_in_bytes(vlcDw)/1024);
	cout << "\t" << (int)(size_in_bytes(vlcDx)/1024);
	cout << "\tNAN";
	cout << "\t" << (int)(size_in_bytes(vlcDz)/1024);
	cout << endl;
	
	vlc_vector<coder::elias_gamma> vlcGv(v);
	vlc_vector<coder::elias_gamma> vlcGw(w);
	vlc_vector<coder::elias_gamma> vlcGx(x);
	vlc_vector<coder::elias_gamma> vlcGz(z);
	cout << "vlc_v eliasG";
	cout << "\t" << (int)(size_in_bytes(vlcGv)/1024);
	cout << "\t" << (int)(size_in_bytes(vlcGw)/1024);
	cout << "\t" << (int)(size_in_bytes(vlcGx)/1024);
	cout << "\tNAN";
	cout << "\t" << (int)(size_in_bytes(vlcGz)/1024);
	cout << endl;
	
	vlc_vector<coder::fibonacci> vlcFv(v);
	vlc_vector<coder::fibonacci> vlcFw(w);
	vlc_vector<coder::fibonacci> vlcFx(x);
	vlc_vector<coder::fibonacci> vlcFz(z);
	cout << "vlc_v Fibo";
	cout << "\t" << (int)(size_in_bytes(vlcFv)/1024);
	cout << "\t" << (int)(size_in_bytes(vlcFw)/1024);
	cout << "\t" << (int)(size_in_bytes(vlcFx)/1024);
	cout << "\tNAN";
	cout << "\t" << (int)(size_in_bytes(vlcFz)/1024);
	cout << endl;
	
	cout << "-----+-----+-----+-----+-----+-----+-----+-----+-----+-----" << endl;

	cout << endl;
	
}

/*
g++ -I ~/include -L ~/lib -o pruebas_sdsl_vector_enc pruebas_sdsl_vector_enc.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

