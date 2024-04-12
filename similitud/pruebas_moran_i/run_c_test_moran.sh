#!/bin/bash

g++ -o c_test_moran_i_tableros c_test_moran_i_tableros.cpp
for i in {0..5}; 
do 
	./c_test_moran_i_tableros tablero_${i}.txt
done

echo "FIN"