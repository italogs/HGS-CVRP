import argparse
import os
# import numpy as np
from utils.data_utils import check_extension, save_dataset

def read_uchoa_vrp_data(filename):
    nbClients = 0
    vehicleCapacity = 0
    instanceData = None
    depotCoord = None
    clientsCoord = []
    clientsDemand = []
    with open(filename) as inputFile:
        for line in inputFile:
            words = line.split()
            if words[0] == "DIMENSION":
                nbClients = int(words[-1]) - 1
            elif words[0] == "CAPACITY":
                vehicleCapacity = int(words[-1])    
            elif words[0] == "NODE_COORD_SECTION":
                break

        instanceData = [ [0.00,0.00], nbClients*[0.00,0.00], nbClients*[0.00],[vehicleCapacity],[0],[0],[1000] ]
        #Depot coord
        for line in inputFile:
            words = line.split()
            depotCoord = [float(words[1]),float(words[2])]
            break
        #Clients coord
        clientID = 0
        for line in inputFile:
            words = line.split()
            if words[0] == "DEMAND_SECTION":
                break
            clientsCoord.append([float(words[1]),float(words[2])])
            clientID = clientID + 1

        #Depot demand (dummy reading)
        clientID = 0
        for line in inputFile:
            words = line.split()
            break
        
        # #Clients demands
        for line in inputFile:
            words = line.split()
            if words[0] == "DEPOT_SECTION":
                break   
            clientsDemand.append(int(words[1]))
   
    return (list(zip([depotCoord],[clientsCoord],[clientsDemand],[vehicleCapacity],[1],[0],[1000])))

    


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--filename", help="Filename of the dataset to create (ignores datadir)")
    parser.add_argument("--data_dir", default='data', help="Create datasets in data_dir/problem (default 'data')")
    
    parser.add_argument("--name", type=str, required=True, help="Name to identify dataset")
    parser.add_argument("--problem", type=str, default='all',
                        help="Problem, 'tsp', 'vrp' or 'all' to generate all")
    parser.add_argument('--data_distribution', type=str, default='all',
                        help="Distributions to generate for problem, default 'all'.")
    parser.add_argument("--dataset_size", type=int, default=10000, help="Size of the dataset")
    parser.add_argument('--graph_sizes', type=int, nargs='+', default=[100],
                        help="Sizes of problem instances (default 100)")
    parser.add_argument("-f", action='store_true', help="Set true to overwrite")
    parser.add_argument('--seed', type=int, default=1234, help="Random seed")

    parser.add_argument("--instance", help="Instance to read")

    opts = parser.parse_args()
    opts.instance_basename = opts.instance.split('/')[-1]

    assert opts.filename is None or (opts.problem != 'all' and len(opts.graph_sizes) == 1), \
        "Can only specify filename when generating a single dataset"

    distributions_per_problem = {
        'vrp': ['uchoa'],
    }
    if opts.problem == 'all':
        problems = distributions_per_problem
    else:
        problems = {
            opts.problem:
                distributions_per_problem[opts.problem]
                if opts.data_distribution == 'all'
                else [opts.data_distribution]
        }
    assert opts.filename is None or len(problems[opts.problem]) == 1, "Can only specify single distribution when generating single dataset"

    for problem, distributions in problems.items():
        datadir = os.path.join(opts.data_dir, problem)
        os.makedirs(datadir, exist_ok=True)

        if opts.filename is None:
            filename = os.path.join(datadir, "{}_{}_seed{}.pkl".format(opts.instance_basename, opts.name, opts.seed))
        else:
            filename = check_extension(opts.filename)

        assert opts.f or not os.path.isfile(check_extension(filename)), \
            "File already exists! Try running with -f option to overwrite."

        dataset = read_uchoa_vrp_data(opts.instance)
        
        save_dataset(dataset, filename)


