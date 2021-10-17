#!/bin/bash

unzip LKH-3.0.6.zip
mv execute-lkh-CC-2step.sh LKH-3.0.6/

cd LKH-3.0.6

echo "Compiling LKH"
make clean 
make

echo "The job spawning part"
for (( instance_id=0; instance_id <= 9999; instance_id = instance_id + 35 ));
do
	sbatch --export=instance_id=${instance_id} execute-lkh-CC-2step.sh
done
