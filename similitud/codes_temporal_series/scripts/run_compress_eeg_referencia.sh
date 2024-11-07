#!/bin/bash

if [ $# -ne 1 ]; then
	echo "$(basename $0) <output_file>"
	exit 1
fi

g++ -std=c++11 -g -O0 -DNDEBUG -I ~/include -L ~/lib -o ts_comp_eeg_to_bitcompress ts_comp_eeg_to_bitcompress.cpp -lsdsl -ldivsufsort -ldivsufsort64

OUTFILE=$1

echo "Fecha INICIO BUILD: $(date +'%Y/%m/%d %H:%M:%S')" > ${OUTFILE}
echo "Medir compressión en EEG con serie de referencia" >> ${OUTFILE}

./ts_comp_eeg_to_bitcompress 507_Depression_REST.bin 50 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 508_Depression_REST.bin 49 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 509_Depression_REST.bin 0 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 510_Depression_REST.bin 1 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 511_Depression_REST.bin 60 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 512_Depression_REST.bin 40 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 513_Depression_REST.bin 26 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 514_Depression_REST.bin 9 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 515_Depression_REST.bin 48 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 516_Depression_REST.bin 8 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 517_Depression_REST.bin 49 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 518_Depression_REST.bin 12 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 519_Depression_REST.bin 34 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 520_Depression_REST.bin 15 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 521_Depression_REST.bin 60 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 522_Depression_REST.bin 8 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 523_Depression_REST.bin 33 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 524_Depression_REST.bin 35 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 525_Depression_REST.bin 34 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 526_Depression_REST.bin 53 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 527_Depression_REST.bin 47 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 528_Depression_REST.bin 64 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 529_Depression_REST.bin 44 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 530_Depression_REST.bin 29 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 531_Depression_REST.bin 32 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 532_Depression_REST.bin 29 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 533_Depression_REST.bin 15 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 534_Depression_REST.bin 39 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 535_Depression_REST.bin 46 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 536_Depression_REST.bin 51 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 537_Depression_REST.bin 36 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 538_Depression_REST.bin 8 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 539_Depression_REST.bin 37 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 540_Depression_REST.bin 48 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 541_Depression_REST.bin 58 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 542_Depression_REST.bin 7 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 543_Depression_REST.bin 20 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 545_Depression_REST.bin 5 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 546_Depression_REST.bin 54 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 547_Depression_REST.bin 38 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 548_Depression_REST.bin 55 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 549_Depression_REST.bin 58 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 550_Depression_REST.bin 53 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 551_Depression_REST.bin 58 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 552_Depression_REST.bin 16 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 553_Depression_REST.bin 24 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 554_Depression_REST.bin 26 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 555_Depression_REST.bin 6 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 556_Depression_REST.bin 48 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 557_Depression_REST.bin 50 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 558_Depression_REST.bin 56 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 559_Depression_REST.bin 62 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 560_Depression_REST.bin 42 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 561_Depression_REST.bin 49 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 562_Depression_REST.bin 54 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 563_Depression_REST.bin 24 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 564_Depression_REST.bin 9 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 565_Depression_REST.bin 41 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 566_Depression_REST.bin 11 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 567_Depression_REST.bin 6 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 568_Depression_REST.bin 43 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 569_Depression_REST.bin 46 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 570_Depression_REST.bin 30 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 573_Depression_REST.bin 14 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 574_Depression_REST.bin 44 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 575_Depression_REST.bin 12 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 576_Depression_REST.bin 25 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 577_Depression_REST.bin 62 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 578_Depression_REST.bin 60 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 579_Depression_REST.bin 35 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 580_Depression_REST.bin 20 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 581_Depression_REST.bin 54 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 582_Depression_REST.bin 46 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 583_Depression_REST.bin 30 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 584_Depression_REST.bin 13 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 585_Depression_REST.bin 36 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 586_Depression_REST.bin 9 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 587_Depression_REST.bin 9 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 588_Depression_REST.bin 35 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 589_Depression_REST.bin 9 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 590_Depression_REST.bin 52 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 591_Depression_REST.bin 9 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 592_Depression_REST.bin 13 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 593_Depression_REST.bin 47 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 594_Depression_REST.bin 9 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 595_Depression_REST.bin 52 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 596_Depression_REST.bin 30 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 597_Depression_REST.bin 29 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 598_Depression_REST.bin 15 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 599_Depression_REST.bin 23 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 600_Depression_REST.bin 18 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 601_Depression_REST.bin 49 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 602_Depression_REST.bin 48 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 603_Depression_REST.bin 30 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 604_Depression_REST.bin 52 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 605_Depression_REST.bin 39 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 606_Depression_REST.bin 52 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 607_Depression_REST.bin 25 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 608_Depression_REST.bin 43 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 609_Depression_REST.bin 15 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 610_Depression_REST.bin 40 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 611_Depression_REST.bin 41 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 612_Depression_REST.bin 58 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 613_Depression_REST.bin 57 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 614_Depression_REST.bin 32 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 615_Depression_REST.bin 12 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 616_Depression_REST.bin 52 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 617_Depression_REST.bin 52 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 618_Depression_REST.bin 25 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 619_Depression_REST.bin 15 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 620_Depression_REST.bin 56 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 621_Depression_REST.bin 38 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 622_Depression_REST.bin 56 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 623_Depression_REST.bin 62 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 624_Depression_REST.bin 41 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 625_Depression_REST.bin 27 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 626_Depression_REST.bin 51 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 627_Depression_REST.bin 32 >> ${OUTFILE}
./ts_comp_eeg_to_bitcompress 628_Depression_REST.bin 29 >> ${OUTFILE}

echo "Fecha FIN BUILD: $(date +'%Y/%m/%d %H:%M:%S')" >> ${OUTFILE}

