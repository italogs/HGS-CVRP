# Neural Networks for Local Search and Crossover in Vehicle Routing: A Possible Overkill?

This is the repository for [Neural Networks for Local Search and Crossover in Vehicle Routing: A Possible Overkill?](https://arxiv.org/abs/2210.12075) by Italo Santana, Andrea Lodi, and Thibaut Vidal.

Paper link: [click here](https://arxiv.org/abs/2210.12075)

## Scope

This code has been designed to solve the "canonical" Capacitated Vehicle Routing Problem (CVRP).
It can also directly handle asymmetric distances as well as duration constraints.

## Running the algorithm


The following options are supported:
```
Usage:
  ./genvrp instancePath solPath [-it nbIter] [-t myCPUtime] [-bks bksPath] [-seed mySeed] [-veh nbVehicles]
Available options:
  -it                   Sets a maximum number of iterations without improvement. Defaults to 20,000
  -t                    Sets a time limit in seconds. If this parameter is set, the code will be restart iteratively until the time limit
  -bks                  Sets an optional path to a BKS in CVRPLib format. This file will be overwritten in case of improvement 
  -seed                 Sets a fixed seed. Defaults to 0     
  -veh                  Sets a prescribed fleet size. Otherwise a reasonable UB on the fleet size is calculated
  -crossoverType        Sets the crossover to be used: 0 for original (no relatedness), 1 for Heatmap-based, and 2 for Distance-based. Defaults to 0.
  -useHeatmapLS         Activate heatmaps to local search. Defaults to 0.
  -nbGranular           Sets the size of the neighborhood in the granular local search. Defaults to 20.
```

### Requirements

To run this algorithm, the user must have several python libraries (e.g., Pytorch and cudatoolkit) as we are reusing a trained graph neural network of https://github.com/wouterkool/dpdp 

and any C++ compiler that supports `-std=c++11` to HGS.

#### To prepare your Python environment, run the instructions below (from https://github.com/wouterkool/dpdp):
```bash
# Create environment named 'dpdp' and activate
conda create -n dpdp python=3.8 scipy anaconda -y
conda activate dpdp
# Install some basic packages
conda install tqdm -y
pip install gdown
# Install PyTorch, see https://pytorch.org/get-started/locally/
conda install pytorch torchvision torchaudio cudatoolkit=10.2 -c pytorch -y
# If you want to train models, you need these packages
pip install tensorboardx==1.5 fastprogress==0.1.18
# We use cupy for some efficient operations
# Somehow conda installation fails
# conda install -c conda-forge cupy cudatoolkit=10.2 -y
pip install cupy-cuda102
# Also some efficient sparse operations using https://github.com/rusty1s/pytorch_scatter
pip install torch-scatter -f https://pytorch-geometric.com/whl/torch-1.10.0+cu102.html
```
If this environment is not working, you are not able to run any HGS combination that relies on heatmaps.

However, you can still run HGS on the other combinations referred in the paper. See **Important** subsection below.


#### Next, install a C++ compiled that supports `-std=c++11`. 

For example, it was tested on clang 11.0.0 in MacOSX and C++9.0 in Ubuntu 20.

Important: This code was not tested on Windows.

### Executing 

* Make sure all requirements were accomplished
* Enter the Program directory: `cd Program`
* Activate your dpdp environment: `conda activate dpdp`
* Run `sh all-X-local.sh`. This will run all instances from set X [[Uchoa et al. 2017](https://doi.org/10.1016/j.ejor.2016.08.012)].
* or run `sh all-XML-local.sh`. This will run all instances from set XML [[Queiroga et al. 2021](https://openreview.net/pdf?id=yHiMXKN6nTl)].
* Logs will be generated on `Solutions/`, `Solutions-DPDP/`and `outputs/`.

### Important
In case you are not able to prepare **dpdp** environment, you can still run HGS (yay!). 

In this case, you must comment the following instructions to prevent your script in dealing with python.

```

cd DPDP/
python generate_instancePKL_each_100_customers.py ${instance}

cd dpdp
python export_heatmap.py --problem vrp --checkpoint logs/vrp_uchoa100/best_val_loss_checkpoint_${model}.tar --instances data/vrp/${instance}.pkl -f -o results/vrp/${instance}/heatmaps/${instance}.pkl --batch_size 1 -c logs/vrp_uchoa100/config_${model}.json

cd ..
python convert_heatmapsPKL_to_readable.py ${instance}
cd ..
```

## Code structure

The code structure is documented in [Vidal (2022)](https://doi.org/10.1016/j.cor.2021.105643) and organized in the following manner:
* **Individual**: Represents an individual solution in the genetic algorithm, also provide I/O functions to read and write individual solutions in CVRPLib format.
* **Population**: Stores the solutions of the genetic algorithm into two different groups according to their feasibility. Also includes the functions in charge of diversity management.
* **Genetic**: Contains the main procedures of the genetic algorithm as well as the crossover.
* **LocalSearch**: Includes the local search functions, including the SWAP* neighborhood.
* **LocalSearch**: A small code used to represent and manage arc sectors (to efficiently restrict the SWAP* neighborhood).
* **Params**: Stores the method parameters, instance data and I/O functions.
* **Commandline**: Reads the line of command.
* **Solver**: Contains all of the HGS algorithm's population mechanisms.
* **Heatmap**: Contains all of routines for reading the heatmaps for HGS mechanisms.
* **main**: Main code to start the algorithm.


## Side notes

* All trained graph neural networks (GNNs) are provided. Given a limitation of 100mb for each file size, the user must join manually a set of splitted files to obtain the largest GNN, which is the GNN provided in https://github.com/wouterkool/dpdp. To obtain this GNN, run the following command `cat best_val_loss_checkpoint_Kool-300dim-30layers-1500epochs.parta* > best_val_loss_checkpoint_Kool-300dim-30layers-1500epochs.tar` 
* For any concern regarding the GNN, we refer the interested reader to https://github.com/wouterkool/dpdp.
* Each generated heatmap contains all probabilities and the processing time. If the user wants to make experiments ignoring this processing time, access Heatmap.h and set `params->time_shift_export_heatmap=0.0` right before `printTimeShift()` within the constructor.

## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**
- Copyright(c) 2022 Italo Santana

