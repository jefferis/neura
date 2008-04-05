#!/bin/tcsh

source configure.tcsh
echo Skel to processed to Segmented for Experiment Name: $ExperimentName


echo Create Target Directory for seg data
mkdir -p ../processedData

echo Copy Directory To Target Directory 
cp -r ../skelData/$ExperimentName  ../processedData

echo Start Iterating over $numberOfTiles Tiles to process raw to enhanced 
setenv counter 0
while ( $counter < $numberOfTiles )
@ counter ++
echo Process Nr. $counter
./skeltoprocessed ../skelData/$ExperimentName ../processedData/$ExperimentName  -tilenr:$counter $processparamter 
end

