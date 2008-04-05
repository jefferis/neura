#!/bin/tcsh

echo "Mosaic Data Processing"

source configure.tcsh

echo Start Processing one huge mosaic image

./rawtoenhanced  ../$1.rawData.bmp ../$1.enhancedData.bmp 
./enhancedtoseg  ../$1.enhancedData.bmp  ../$1.segmentedData.bmp 
./segtoskel	     ../$1.segmentedData.bmp  ../$1.skelData.bmp 
./skeltoprocessed ../$1.skelData.bmp ../$1.processedData.bmp

echo Finised Processing


