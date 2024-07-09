#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sdsl/vectors.hpp>

using namespace std;
using namespace sdsl;

int main(){
    int_vector<> v(10000);
    generate(v.begin(), v.end(), rand);
    cout << "v:";
    for(int i=0; i<10; i++){
        cout << "\t" << v[i] ;
    }
    cout << "\t..." << endl;

    int_vector<> w(v.size());    
    for(int i=0; i<w.size(); i++){
        w[i] = v[i] % 10;
    }
    cout << "w:";
    for(int i=0; i<10; i++){
        cout << "\t" << w[i] ;
    }
    cout << "\t..." << endl;

    cout << "v.size(): " << v.size() << " - memory: " << size_in_bytes(v) << " bytes." << endl;
    cout << "w.size(): " << w.size() << " - memory: " << size_in_bytes(w) << " bytes." << endl;
    cout << "bitcompressing..." << endl;
    util::bit_compress(v);
    util::bit_compress(w);
    cout << "v memory: " << size_in_bytes(v) << " bytes." << endl;
    cout << "w memory: " << size_in_bytes(w) << " bytes." << endl;
    cout << "encoding enc_vector..." << endl;
    enc_vector<> evv(v);
    enc_vector<> evw(w);
    cout << "evv memory: " << size_in_bytes(evv) << " bytes." << endl;
    cout << "evw memory: " << size_in_bytes(evw) << " bytes." << endl;
    cout << "encoding vlc_vector..." << endl;
    vlc_vector<> vlcv(v);
    vlc_vector<> vlcw(w);
    cout << "evv memory: " << size_in_bytes(evv) << " bytes." << endl;
    cout << "evw memory: " << size_in_bytes(evw) << " bytes." << endl;
    cout << endl;
    
}

/*
g++ -std=c++11 -g -O0 -DNDEBUG -fopenmp -I ~/include -L ~/lib -o prueba_vector_enc prueba_vector_enc.cpp -lsdsl -ldivsufsort -ldivsufsort64
*/