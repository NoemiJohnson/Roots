#!/bin/bash

rm runtimes

Nodes=$1

for ((i=1; i<=$Nodes; i++)); do

	cd dambreak_$i
	cat runtime>>/home/jpsimpso/OpenFOAM/clusterproject/runtimes
	cd ..

done
