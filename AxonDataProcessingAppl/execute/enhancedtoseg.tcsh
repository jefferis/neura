#!/bin/tcsh

source configure.tcsh
echo Enhanced to Segmented for Experiment Name: $ExperimentName

echo Create Target Directory for seg data
mkdir -p ../segData

echo Copy Directory To Target Directory 
cp -r ../enhancedData/$ExperimentName  ../segData

echo Start Iterating over $numberOfTiles Tiles to process raw to enhanced 
setenv counter 0
while ( $counter < $numberOfTiles )
@ counter ++
echo Process Nr. $counter
./enhancedtoseg ../enhancedData/$ExperimentName ../segData/$ExperimentName  -tilenr:$counter $SegParameter
end

