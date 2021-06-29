#!/bin/bash


time=400

bash clean-heatmap-files.sh
# make clean && make

seed=1
instances=('X-n200-k36')
for (( index=${#instances[@]}-1; index >= 0; index--));
do
    cd heatmap 

    # Generate training instance (read VRP uchoa isntance and put it into PKL)
    python generate_data.py --name train --instance ../../Instances/CVRP/${instances[$index]}.vrp --seed 1 -f

    # Generate example solutions for model
    python -m problems.vrp.vrp_baseline lkh data/vrp/${instances[$index]}.vrp_train_seed1.pkl --no_cuda --disable_cache --lkh_seed 1 -f 
    python -m problems.vrp.vrp_baseline lkh data/vrp/${instances[$index]}.vrp_train_seed1.pkl --no_cuda --disable_cache --lkh_seed 2 -f 
    python -m problems.vrp.vrp_baseline lkh data/vrp/${instances[$index]}.vrp_train_seed1.pkl --no_cuda --disable_cache --lkh_seed 3 -f

    # Train model
    CUDA_VISIBLE_DEVICES=0,0 python main_vrp.py --config configs/vrp_configs.json --placeholder ${instances[$index]}.vrp

    # Export heatmap
    CUDA_VISIBLE_DEVICES=0,0 python export_heatmap.py --problem vrp --checkpoint logs/vrp_${instances[$index]}.vrp/best_val_loss_checkpoint.tar --instances data/vrp/${instances[$index]}.vrp_train_seed1.pkl -f --batch_size 1


    CUDA_VISIBLE_DEVICES=0,0 python eval.py data/vrp/${instances[$index]}.vrp_train_seed1.pkl --problem cvrp --decode_strategy dpdp --score_function heatmap_potential --beam_size 10000 --heatmap_threshold 1e-5 --heatmap results/vrp/${instances[$index]}.vrp_train_seed1/heatmaps/heatmaps_vrp_${instances[$index]}.vrp.pkl -f
    cd ..


    # for ((seed=1; seed <= 1; seed++));
    # do
    #     crossover=1


        
    #     ./genvrp ../Instances/CVRP/${instances[$index]}.vrp testes/${instances[$index]}_crossover${crossover}_heap_optimized_seed${seed}_time${time}.sol -crossover ${crossover} -seed $seed -t ${time} #> testes/output_${instances[$index]}_crossover${crossover}_heap_optimized_seed${seed}_time${time}.txt &
        
    # done;
    # wait
done;