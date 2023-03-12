#!/bin/bash

make clean && make
mkdir -p Solutions/
mkdir -p Solutions-DPDP/
mkdir -p outputs/

seed=1
instances_time=(24 25 26 27 29 30 31 32 33 34 35 37 37 39 40 41 42 43 44 45 47 48 49 50 51 52 53 55 56 57 58 59 60 61 62 64 65 66 67 68 69 70 71 73 74 75 76 77 78 79 80 82 84 86 88 90 92 94 96 98 101 103 105 108 110 112 115 118 120 123 126 128 131 134 137 140 144 147 150 154 157 161 164 168 172 176 180 184 188 192 196 201 205 210 215 220 224 229 235 240 )
instances=(X-n101-k25 X-n106-k14 X-n110-k13 X-n115-k10 X-n120-k6 X-n125-k30 X-n129-k18 X-n134-k13 X-n139-k10 X-n143-k7 X-n148-k46 X-n153-k22 X-n157-k13 X-n162-k11 X-n167-k10 X-n172-k51 X-n176-k26 X-n181-k23 X-n186-k15 X-n190-k8 X-n195-k51 X-n200-k36 X-n204-k19 X-n209-k16 X-n214-k11 X-n219-k73 X-n223-k34 X-n228-k23 X-n233-k16 X-n237-k14 X-n242-k48 X-n247-k50 X-n251-k28 X-n256-k16 X-n261-k13 X-n266-k58 X-n270-k35 X-n275-k28 X-n280-k17 X-n284-k15 X-n289-k60 X-n294-k50 X-n298-k31 X-n303-k21 X-n308-k13 X-n313-k71 X-n317-k53 X-n322-k28 X-n327-k20 X-n331-k15 X-n336-k84 X-n344-k43 X-n351-k40 X-n359-k29 X-n367-k17 X-n376-k94 X-n384-k52 X-n393-k38 X-n401-k29 X-n411-k19 X-n420-k130 X-n429-k61 X-n439-k37 X-n449-k29 X-n459-k26 X-n469-k138 X-n480-k70 X-n491-k59 X-n502-k39 X-n513-k21 X-n524-k153 X-n536-k96 X-n548-k50 X-n561-k42 X-n573-k30 X-n586-k159 X-n599-k92 X-n613-k62 X-n627-k43 X-n641-k35 X-n655-k131 X-n670-k130 X-n685-k75 X-n701-k44 X-n716-k35 X-n733-k159 X-n749-k98 X-n766-k71 X-n783-k48 X-n801-k40 X-n819-k171 X-n837-k142 X-n856-k95 X-n876-k59 X-n895-k37 X-n916-k207 X-n936-k151 X-n957-k87 X-n979-k58 X-n1001-k43)

instances=(X-n270-k35)
trained_models=(10dim-5layers-1500epochs) # 10dim-5layers-500epochs Kool-300dim-30layers-1500epochs) 
for model in "${trained_models[@]}"
do
    # Print the model name
    echo $model
    instance_index=0
    for instance in "${instances[@]}"
    do
        time=${instances_time[$instance_index]}



        # # Non-heatmap-based combination (no need in having dpdp environment done)
        # nbGranular=15
        # useHeatmapLS=0
        # crossoverType=0
        # ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}.sol -useHeatmapLS ${useHeatmapLS} -crossoverType ${crossoverType} -seed ${seed} -t ${time} -nbGranular ${nbGranular}  &> outputs/output_${instance}_useHeatmap${useHeatmapLS}_crossover${crossoverType}_seed${seed}_time${time}_nbGranular${nbGranular}.txt &




        # Example
        # Heatmap-based combination (must have dpdp environment)

        # Transform CVRP instance into the input instance for the GNN
        cd DPDP/
        python generate_instancePKL_each_100_customers.py ${instance}

        # Export the heatmap
        cd dpdp
        python export_heatmap.py --problem vrp --checkpoint logs/vrp_uchoa100/best_val_loss_checkpoint_${model}.tar --instances data/vrp/${instance}.pkl -f -o results/vrp/${instance}/heatmaps/${instance}.pkl --batch_size 1 -c logs/vrp_uchoa100/config_${model}.json

        # Convert PKL (heatmaps from the GNN) into TXT (readable in HGS)
        cd ..
        python convert_heatmapsPKL_to_readable.py ${instance}
        cd ..
    
        nbGranular=15
        useHeatmapLS=1
        crossoverType=0
        ./genvrp ../Instances/CVRP/${instance}.vrp Solutions/${instance}_useHeatmapLS${useHeatmapLS}_crossoverType${crossoverType}.sol -useHeatmapLS ${useHeatmapLS} -crossoverType ${crossoverType} -seed ${seed} -t ${time} -nbGranular ${nbGranular}  # &> outputs/output_${instance}_useHeatmap${useHeatmapLS}_crossover${crossoverType}_seed${seed}_time${time}_nbGranular${nbGranular}.txt &

        wait;
        ((instance_index=instance_index+1))
        break
    done;

done







