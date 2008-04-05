#!/bin/tcsh

source configure.tcsh
echo Segmented to Skel to Segmented for Experiment Name: $ExperimentName

echo Create Target Directory for seg data
mkdir -p ../skelData

echo Copy Directory To Target Directory 
cp -r ../segData/$ExperimentName  ../skelData

echo Start Iterating over $numberOfTiles Tiles to process raw to enhanced 
setenv counter 0
while ( $counter < $numberOfTiles )
@ counter ++
echo Process Nr. $counter
./segtoskel ../segData/$ExperimentName ../skelData/$ExperimentName  -tilenr:$counter $SkelParameter 
end

