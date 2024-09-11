#include <iostream>

using namespace std;

void swap(int &a, int &b){
	int x = a;
	a = b;
	b = x;
}

int main(int argc, char const *argv[]){
	int x = 0;
	int y = 3;
	cout << "x: " << x << " - y: " << y << endl;
	swap(x, y);
	cout << "x: " << x << " - y: " << y << endl;
	return 0;
}