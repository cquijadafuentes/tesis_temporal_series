#include <iostream>
#include <sdsl/vectors.hpp>

using namespace std;
using namespace sdsl;

int main(){
    int_vector<> v = {3,2,1,0,2,1,3,4,1,1,1,3,2,3};
    v[1]=0;
    util::bit_compress(v);
    cout << v << endl;
    cout << size_in_bytes(v) << endl; 

    int_vector<> w(v.size());
    for(int i =0; i<v.size(); i++){
    	w[i] = v[v.size()-1-i];
    }
    cout << w << endl;
    cout << "Size in bytes of w: " << size_in_bytes(w) << endl;
    cout << "Compressing...";
    util::bit_compress(w);
    cout << " ok." << endl;
    cout << "Size in bytes of w: " << size_in_bytes(w) << endl;
}

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o pruebas_sdsl pruebas_sdsl.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/