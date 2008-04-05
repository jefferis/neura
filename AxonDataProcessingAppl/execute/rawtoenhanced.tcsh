#!/bin/tcsh

source configure.tcsh
echo Now Process Step Raw to Enhanced for Experiment Name: $ExperimentName

echo Create Target Directory for enhanced data
mkdir -p ../enhancedData

echo Copy Directory To Target Directory 
cp -r ../rawData/$ExperimentName  ../enhancedData

echo Start Iterating over $numberOfTiles Tiles to process raw to enhanced 
setenv counter 0
while ( $counter < $numberOfTiles )
@ counter ++
echo Process Nr. $counter
./rawtoenhanced ../rawData/$ExperimentName ../enhancedData/$ExperimentName  -tilenr:$counter $EnhanceParameter 
end

