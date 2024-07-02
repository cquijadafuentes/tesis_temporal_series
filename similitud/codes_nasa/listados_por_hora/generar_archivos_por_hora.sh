#!/bin/bash

for i in APCP CAPE CONV DLWRF DSWFR PEVAP PRES SPFH TMP UGRD VGRD
do
	for h in '00' '01' '02' '03' '04' '05' '06' '07' '08' '09' '10' '11' '12' '13' '14' '15' '16' '17' '18' '19' '20' '21' '22' '23'
	do
		FILE="${i}_NLDAS_hour_${h}.txt"
		touch ${FILE}
		echo "464 224 4 2 4 2" > ${FILE}
		for d in {1..9}
		do
			echo "${i}_NLDAS_FORA0125_H.A2018010${d}.${h}00.002.grb.SUB.nc4.bin" >> ${FILE}
		done
		for d in {10..31}
		do
			echo "${i}_NLDAS_FORA0125_H.A201801${d}.${h}00.002.grb.SUB.nc4.bin" >> ${FILE}
		done
		for d in {1..9}
		do
			echo "${i}_NLDAS_FORA0125_H.A2018020${d}.${h}00.002.grb.SUB.nc4.bin" >> ${FILE}
		done
		for d in {10..28}
		do
			echo "${i}_NLDAS_FORA0125_H.A201802${d}.${h}00.002.grb.SUB.nc4.bin" >> ${FILE}
		done
		for d in {1..9}
		do
			echo "${i}_NLDAS_FORA0125_H.A2018030${d}.${h}00.002.grb.SUB.nc4.bin" >> ${FILE}
		done
		for d in {10..31}
		do
			echo "${i}_NLDAS_FORA0125_H.A201803${d}.${h}00.002.grb.SUB.nc4.bin" >> ${FILE}
		done
		for d in {1..9}
		do
			echo "${i}_NLDAS_FORA0125_H.A2018040${d}.${h}00.002.grb.SUB.nc4.bin" >> ${FILE}
		done
		for d in {10..21}
		do
			echo "${i}_NLDAS_FORA0125_H.A201804${d}.${h}00.002.grb.SUB.nc4.bin" >> ${FILE}
		done
	done
done

echo "FIN"