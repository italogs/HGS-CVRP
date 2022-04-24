#!/bin/bash

# Creating folders
mkdir -p Solutions/5.1/
mkdir -p outputs/5.1/


# It will solve k instances at each sbatch
step_instances=250
time=5

for (( offset=0; offset <= 9999; ));
do
    sbatch --export=offset=${offset},time=${time},step_instances=${step_instances} all-XML-CC-script2-subsec5.1.sh
    offset=$((${offset} + ${step_instances}))
done
