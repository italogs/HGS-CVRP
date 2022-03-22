#!/bin/bash
#SBATCH --cpus-per-task=8
#SBATCH --mem=12GB
#SBATCH --time=48:00:00
#SBATCH --nodes=1
#SBATCH --gres=gpu:1


module load python
source ~/env/bin/activate

sleep 5


echo "batch_size: ${batch_size}"
trained_models=(10dim-5layers-1500epochs 10dim-5layers-500epochs)
seed=1

for model in "${trained_models[@]}"
do
    cd DPDP
    python generate_instancePKL_each_100_customers.py ${instance}

    cd dpdp
    python export_heatmap.py --problem vrp --checkpoint logs/vrp_uchoa100/best_val_loss_checkpoint_${model}.tar --instances data/vrp/${instance}.pkl -f -o results/vrp/${instance}/heatmaps/${instance}.pkl --batch_size ${batch_size} -c logs/vrp_uchoa100/config_${model}.json

    echo "Final export"

    cd ..
    python convert_heatmapsPKL_to_readable.py ${instance}
    cd ..
    echo "Convert heatmaps"
    time=${timeClassic}
    echo ${time}
    arr_nbGranular=(15 20 30)
    # Spawn 12 processes at once
    for nbGranular in "${arr_nbGranular[@]}"
    do
        echo ${nbGranular}
        useHeatmapLS=0
        useHeatmapOX=0
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -useHeatmapOX ${useHeatmapOX} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

        useHeatmapLS=0
        useHeatmapOX=1
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -useHeatmapOX ${useHeatmapOX} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
        # wait
        useHeatmapLS=1
        useHeatmapOX=0
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -useHeatmapOX ${useHeatmapOX} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &            

        useHeatmapLS=1
        useHeatmapOX=1
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -useHeatmapOX ${useHeatmapOX} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
        wait
    done

    for (( seed = 2 ; seed <= 5; seed++))
    do
        time=${timeReduced}
        echo ${time}
        arr_nbGranular=(15 20 30)
        # Spawn 12 processes at once
        for nbGranular in "${arr_nbGranular[@]}"
        do
            echo ${nbGranular}
            useHeatmapLS=0
            useHeatmapOX=0
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -useHeatmapOX ${useHeatmapOX} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            useHeatmapLS=0
            useHeatmapOX=1
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -useHeatmapOX ${useHeatmapOX} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            useHeatmapLS=1
            useHeatmapOX=0
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -useHeatmapOX ${useHeatmapOX} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &            

            useHeatmapLS=1
            useHeatmapOX=1
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -useHeatmapOX ${useHeatmapOX} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/${model}/useHeatmapLS${useHeatmapLS}_useHeatmapOX${useHeatmapOX}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &
            wait
        done
    done
done 

rm -rf DPDP/Heatmaps_for_HGS/${instance}/
rm -rf DPDP/dpdp/results/vrp/${instance}/

echo "FIM"