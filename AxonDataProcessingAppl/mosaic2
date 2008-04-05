#!/bin/tcsh

echo "Mosaic Data Processing"

source configure.tcsh

echo Start Processing mosaic image

./rawtoenhanced.tcsh 
./enhancedtoseg.tcsh
./segtoskel.tcsh
./skeltoprocessed.tcsh

echo Finised Processing


