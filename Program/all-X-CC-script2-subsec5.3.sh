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
trained_models=(10dim-5layers-500epochs)
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
    arr_nbGranular=(15 20 30 50)
    for (( seed = 1 ; seed <= 10; seed++))
    do
        time=${timeReduced}
        echo ${time}
        for nbGranular in "${arr_nbGranular[@]}"
        do
            useHeatmapLS=0
            crossoverType=0
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossoverType ${crossoverType} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            useHeatmapLS=0
            crossoverType=2
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossoverType ${crossoverType} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            useHeatmapLS=1
            crossoverType=2
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossoverType ${crossoverType} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            wait

            useHeatmapLS=0
            crossoverType=1
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossoverType ${crossoverType} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            useHeatmapLS=1
            crossoverType=0
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossoverType ${crossoverType} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            useHeatmapLS=1
            crossoverType=1
            ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.sol -crossoverType ${crossoverType} -useHeatmapLS ${useHeatmapLS} -t ${time} -seed ${seed} -nbGranular ${nbGranular} &> outputs/5.3/${model}/useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}/output_${instance}_time${time}_seed${seed}_nbGranular${nbGranular}.txt &

            wait
        done
    done
done 

rm -rf DPDP/Heatmaps_for_HGS/${instance}/
rm -rf DPDP/dpdp/results/vrp/${instance}/

echo "FIM"