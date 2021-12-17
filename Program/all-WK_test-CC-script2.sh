#!/bin/bash
#SBATCH --gres=gpu:1
#SBATCH --cpus-per-task=5
#SBATCH --mem=20GB       
#SBATCH --time=24:00:00
#SBATCH --nodes=1


module load python
source ~/env/bin/activate

sleep 20

for (( it = ${instance_id} ; it < ${instance_id} + 1; it++))
do
    instance="WK_test_${it}"
    cd DPDP/
    python generate_instancePKL_each_100_customers.py ${instance}

    cd dpdp
    python export_heatmap.py --problem vrp --checkpoint logs/vrp_uchoa100/best_val_loss_checkpoint_cedar.tar --instances data/vrp/${instance}.pkl -f -o results/vrp/${instance}/heatmaps/${instance}.pkl --batch_size 1 -c logs/vrp_uchoa100/config_cedar.json

    cd ..
    python convert_heatmapsPKL_to_readable.py ${instance}
    cd ..
done 



for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    instance="WK_test_${it}"
    cd DPDP/
    python generate_instancePKL_each_100_customers.py ${instance}

    cd dpdp
    python export_heatmap.py --problem vrp --checkpoint logs/vrp_uchoa100/best_val_loss_checkpoint_cedar.tar --instances data/vrp/${instance}.pkl -f -o results/vrp/${instance}/heatmaps/${instance}.pkl --batch_size 1 -c logs/vrp_uchoa100/config_cedar.json

    cd ..
    python convert_heatmapsPKL_to_readable.py ${instance}
    cd ..
done 



seed=1
nbGranular=20
echo ${nbGranular}

for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    instance="WK_test_${it}"
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done

nbGranular=30
echo ${nbGranular}
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    instance="WK_test_${it}"
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done

nbGranular=50
echo ${nbGranular}
for (( it = ${instance_id} ; it < ${instance_id} + 100; it++))
do
    useDPDP=0
    crossover=1
    instance="WK_test_${it}"
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=1
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=0
    crossover=9
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    useDPDP=1
    crossover=9
    ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/useDPDP${useDPDP}_crossover${crossover}/${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossover ${crossover} -useDPDP ${useDPDP} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/useDPDP${useDPDP}_crossover${crossover}/output_${instance}_useDPDP${useDPDP}_crossover${crossover}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

    wait
done

