from utils.data_utils import load_dataset
import os


def write_solutions():
    filenames = ["results/vrp/vrp_uchoa100_test_seed1234/main_results/beam1000000.pkl", "results/vrp/vrp_uchoa100_test_seed1234/main_results/beam100000.pkl", "results/vrp/vrp_uchoa100_test_seed1234/main_results/beam10000.pkl"]

    
    for filename in filenames:
        baseline = ((filename.split("/")[-1]).split(".")[0])+ ".txt"
        with open(baseline,"w")  as f:
            instances = load_dataset(filename)
            for instance in instances[0]:
                f.write(str(instance[0]) + "\n")


if __name__ == "__main__":
    write_solutions()