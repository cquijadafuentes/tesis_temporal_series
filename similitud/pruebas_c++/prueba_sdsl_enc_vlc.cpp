#include <iostream>
#include <sdsl/vectors.hpp>

using namespace std;
using namespace sdsl;

int main(){
	int_vector<> v = {3,2,1,0,2,1,3,4,1,1,1,3,2,3};
	cout << size_in_bytes(v) << endl;
	cout << v << endl;
	cout << "int_vector: " << size_in_bytes(v) << endl;
	util::bit_compress(v);
	cout << "int_vector (bit_compress): " << size_in_bytes(v) << endl;
	vlc_vector<> x(v);
	cout << "vlc_vector: " << size_in_bytes(x) << endl;
	x = vlc_vector<coder::elias_delta>(v);
	cout << "vlc_vector<elias_delta>: " << size_in_bytes(x) << endl;
	x = vlc_vector<coder::elias_gamma>(v);
	cout << "vlc_vector<elias_gamma>: " << size_in_bytes(x) << endl;
	x = vlc_vector<coder::fibonacci>(v);
	cout << "vlc_vector<fibonacci>: " << size_in_bytes(x) << endl;
	cout << "int_vector (post vlc): " << size_in_bytes(v) << endl;
	enc_vector<> y(v);
	cout << "enc_vector: " << size_in_bytes(y) << endl;
	cout << "int_vector (post enc): " << size_in_bytes(v) << endl;
	v[0] = 4;
	cout << v << endl;
	cout << x << endl;
	cout << y << endl;
}

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o prueba_sdsl_enc_vlc prueba_sdsl_enc_vlc.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/