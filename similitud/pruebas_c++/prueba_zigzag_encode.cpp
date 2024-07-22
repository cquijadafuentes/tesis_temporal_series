#include <iostream>

using namespace std;

int zigzag_encode(int i){
   return ((i >> 31) ^ (i << 1));
}

int zigzag_decode(int i){
    return ((i >> 1) ^ -(i & 1));
}

int main(int argc, char const *argv[]){
	cout << "zigzag_encode 7: " << zigzag_encode(7) << endl;
	cout << "zigzag_encode -7: " << zigzag_encode(-7) << endl;
	cout << "zigzag_decode 30: "<< zigzag_decode(30) << endl;
	cout << "zigzag_decode 41: "<< zigzag_decode(41) << endl;
	return 0;
}