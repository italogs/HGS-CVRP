
from dpdp.utils.data_utils import load_heatmaps
import numpy as np

def write_heatmaps():
   
    heatmap = "dpdp/results/vrp/vrp_uchoa100_test_seed1234/heatmaps/heatmaps_vrp_uchoa100.pkl"
    heatmaps = np.exp(load_heatmaps(heatmap))
    for index, heatmap in enumerate(heatmaps):
        new_filename = "WK_test_" + str(index) 
        with open("../Heatmaps/" + new_filename + ".hm","w") as f:
            for customer_heatmap in heatmap:

                f.write(" ".join([ str(value) for value in customer_heatmap]))
                f.write("\n")

if __name__ == "__main__":
    write_heatmaps()