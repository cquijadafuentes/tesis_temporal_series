
/*
g++ -I ~/include -L ~/lib -o pruebas_sdsl_int_vector pruebas_sdsl_int_vector.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/

#include <iostream>
#include <sdsl/vectors.hpp>

using namespace std;
using namespace sdsl;

int main(int argc, char const *argv[]){
	int_vector<> v(10*(1<<20));
	for (size_t i=0; i<10; ++i)
		for (size_t j=0; j<1U<<20; ++j)
			v[i*(1<<20)+j] = j;
	cout << size_in_mega_bytes(v) << endl;
	util::bit_compress(v);
	cout << size_in_mega_bytes(v) << endl;
	enc_vector<> ev(v);
	cout << size_in_mega_bytes(ev) << endl;
	return 0;
}