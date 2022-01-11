#!/bin/bash

# Creating folders
mkdir -p DPDP/Heatmaps_for_HGS/

trained_models=(10dim-5layers-1500epochs 10dim-5layers-500epochs Kool-300dim-30layers-1500epochs) 
for model in "${trained_models[@]}"
do
    mkdir -p Solutions/useHeatmap0_crossover1/${model}/
    mkdir -p outputs/useHeatmap0_crossover1/${model}/

    mkdir -p Solutions/useHeatmap0_crossover9/${model}/
    mkdir -p outputs/useHeatmap0_crossover9/${model}/

    mkdir -p Solutions/useHeatmap1_crossover1/${model}/
    mkdir -p outputs/useHeatmap1_crossover1/${model}/

    mkdir -p Solutions/useHeatmap1_crossover9/${model}/
    mkdir -p outputs/useHeatmap1_crossover9/${model}/
done


instances=(X-n101-k25 X-n106-k14 X-n110-k13 X-n115-k10 X-n120-k6 X-n125-k30 X-n129-k18 X-n134-k13 X-n139-k10 X-n143-k7 X-n148-k46 X-n153-k22 X-n157-k13 X-n162-k11 X-n167-k10 X-n172-k51 X-n176-k26 X-n181-k23 X-n186-k15 X-n190-k8 X-n195-k51 X-n200-k36 X-n204-k19 X-n209-k16 X-n214-k11 X-n219-k73 X-n223-k34 X-n228-k23 X-n233-k16 X-n237-k14 X-n242-k48 X-n247-k50 X-n251-k28 X-n256-k16 X-n261-k13 X-n266-k58 X-n270-k35 X-n275-k28 X-n280-k17 X-n284-k15 X-n289-k60 X-n294-k50 X-n298-k31 X-n303-k21 X-n308-k13 X-n313-k71 X-n317-k53 X-n322-k28 X-n327-k20 X-n331-k15 X-n336-k84 X-n344-k43 X-n351-k40 X-n359-k29 X-n367-k17 X-n376-k94 X-n384-k52 X-n393-k38 X-n401-k29 X-n411-k19 X-n420-k130 X-n429-k61 X-n439-k37 X-n449-k29 X-n459-k26 X-n469-k138 X-n480-k70 X-n491-k59 X-n502-k39 X-n513-k21 X-n524-k153 X-n536-k96 X-n548-k50 X-n561-k42 X-n573-k30 X-n586-k159 X-n599-k92 X-n613-k62 X-n627-k43 X-n641-k35 X-n655-k131 X-n670-k130 X-n685-k75 X-n701-k44 X-n716-k35 X-n733-k159 X-n749-k98 X-n766-k71 X-n783-k48 X-n801-k40 X-n819-k171 X-n837-k142 X-n856-k95 X-n876-k59 X-n895-k37 X-n916-k207 X-n936-k151 X-n957-k87 X-n979-k58 X-n1001-k43)
instances_time=(240 252 262 274 286 298 307 319 331 341 353 365 374 386 398 410 420 432 444 454 466 478 487 499 511 523 533 545 557 566 578 590 600 612 624 636 646 658 670 679 691 703 713 725 737 749 758 770 782 792 804 823 840 859 878 900 919 941 960 984 1006 1027 1051 1075 1099 1123 1150 1176 1202 1229 1255 1284 1313 1344 1373 1404 1435 1469 1502 1536 1570 1606 1642 1680 1716 1757 1795 1836 1877 1920 1963 2006 2052 2100 2146 2196 2244 2294 2347 2400)
instances_time_reduced=(24 25 26 27 29 30 31 32 33 34 35 37 37 39 40 41 42 43 44 45 47 48 49 50 51 52 53 55 56 57 58 59 60 61 62 64 65 66 67 68 69 70 71 73 74 75 76 77 78 79 80 82 84 86 88 90 92 94 96 98 101 103 105 108 110 112 115 118 120 123 126 128 131 134 137 140 144 147 150 154 157 161 164 168 172 176 180 184 188 192 196 201 205 210 215 220 224 229 235 240)
batches_size=(1 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50)

# We will execute 100 sbatchs commands. Each sbatch executes 20 processes in parallel (4 different crossover types on 5 different seeds)
for ((index=${#instances[@]} - 1; index>=0; index--))
do
    sbatch --export=instance=${instances[$index]},batch_size=${batches_size[$index]},timeClassic=${instances_time[$index]},timeReduced=${instances_time_reduced[$index]} all-X-CC-script2.sh
done