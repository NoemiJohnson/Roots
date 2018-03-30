#!/bin/bash

Nodes=$1

for ((i=1; i<=$Nodes; i++)); do

	cp -r base_dambreak dambreak_$i									#Copy base file for number of nodes (1 node = 16 processors)
	cd dambreak_$i											#Change to dambreak directory for current number of nodes
	cd system/											#Change to directory with file that controls mesh
	
	NumProc=$(($i*16))										#Calculate total number of processors	
	
        Sqrt=$(echo "scale=0;sqrt($NumProc)" | bc)
        Roundup=$(($Sqrt+1))
	
        for ((j=1; j<=$Roundup; j++)); do
                if (( $NumProc % $j == 0)); then
                        y=$(($NumProc/$j))
                        x=$j
                fi
        done
	
	sed -r -i "s/numberOfSubdomains 4/numberOfSubdomains $NumProc/" decomposeParDict		#replace subdomains with number of subdommains for total processors(Nodes*ppn)
	sed -r -i "s/( 2 16 1 )/ $x $y 1 /" decomposeParDict						#replacing domain configuration, should match total proc. 2*16*1 =32
	
	cd ..												#Changing back to dambreak directory for current number of nodes
	
	module purge
	module load gcc/4.6.3 mkl/13.1.0 impi/4.1.0
	source /share/apps/OpenFOAM/2.1.0-gcc/OpenFOAM-2.1.0/etc/bashrc
	
	sed -r -i "s/for _ processors/for $NumProc processors/" runtime
	decomposePar & pids[$i]=$!									#decompose the domain using decomposePar command
	wait ${pids[$i]}										#wait until decomposePar is done running
	
#Noemi script to change nodes ($Nodes previously calculated)						#Current directory is dambreak_$i
#Noemi script to change total processors ($NumProc previously calculated)				#Changing values in foam.pbs in the dambreak_$i directory
#Noemi script to submit .pbs job (qsub foam.pbs)							#Submit job to the que
	
	cd ..												#Change back to the clusterproject directory with foam.sh and dambreak_bas
	
done
