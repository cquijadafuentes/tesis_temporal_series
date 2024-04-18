#!/bin/bash

for i in A B C D E; 
do 
	../c_test_moran_i_tableros grilla${i}.txt
done

echo "FIN"
