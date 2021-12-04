from dpdp.utils.data_utils import load_heatmaps
import numpy as np
import os
import sys

def write_heatmaps():

    uchoa_instance = ""
    for i, arg in enumerate(sys.argv):
        if i == 1:
            uchoa_instance = str(arg)
    for instance in [uchoa_instance]:

        heatmap = "dpdp/results/vrp/"+ instance + "/heatmaps/" + instance + ".pkl"
        heatmaps = np.exp(load_heatmaps(heatmap))

        file_dir = "Heatmaps_for_HGS/" + instance + "/"
        if not(os.path.exists(file_dir)):
            os.makedirs(file_dir)

        for index, heatmap in enumerate(heatmaps):
            new_filename = instance + "_" + str(index+1)

            with open(file_dir + new_filename + ".hm","w") as f:
                for customer_heatmap in heatmap:
                    f.write(" ".join([ str(value) for value in customer_heatmap]))
                    f.write("\n")

if __name__ == "__main__":
    write_heatmaps()