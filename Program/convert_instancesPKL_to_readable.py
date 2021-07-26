from dpdp.utils.data_utils import load_dataset
import os


def write_instances():

    filename = "dpdp/data/vrp/vrp_uchoa100_test_seed1234.pkl"
    print(os.path.isfile(filename))

    instances = load_dataset(filename)

    for index, instance in enumerate(instances):
        new_filename = "WK_test_" + str(index) 
        with open("../Instances/CVRP/" + new_filename + ".vrp","w") as f:


            depot = instances[index][0]
            loc = instances[index][1]
            demand = instances[index][2]

            f.write("\n".join([
                "{} : {}".format(k, v)
                for k, v in (
                    ("NAME", new_filename),
                    ("TYPE", "CVRP"),
                    ("DIMENSION", len(instances[index][1]) + 1),
                    ("EDGE_WEIGHT_TYPE", "EUC_2D"),
                    ("CAPACITY", instances[index][3])
                )
            ]))

        
            f.write("\n")
            f.write("NODE_COORD_SECTION\n")
            f.write("\n".join([
                "{}\t{}\t{}".format(i + 1, x, y)
                for i, (x, y) in enumerate([depot] + loc)
            ]))
            f.write("\n")
            f.write("DEMAND_SECTION\n")
            f.write("\n".join([
                "{}\t{}".format(i + 1, d)
                for i, d in enumerate([0] + demand)
            ]))
            f.write("\n")
            f.write("DEPOT_SECTION\n")
            f.write("1\n")
            f.write("-1\n")
            f.write("EOF\n")

if __name__ == "__main__":
    write_instances()