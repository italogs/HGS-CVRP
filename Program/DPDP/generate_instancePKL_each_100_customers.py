from operator import itemgetter, attrgetter
import pickle
import sys
import os

uchoa_instance = ""
for i, arg in enumerate(sys.argv):
    if i == 1:
        uchoa_instance = str(arg)

for instance_name in [uchoa_instance]:
    print("instance_name:",instance_name)
    with open("../../Instances/CVRP/" + instance_name + ".vrp") as instance:
        instance_features = {}
        lines = instance.readlines()
        num_nodes = -1 
        capacity = -1

        index_NODE_COORD_SECTION = 0
        for line in lines:
            line = line.strip()
            if line == "NODE_COORD_SECTION":
                index_NODE_COORD_SECTION +=1
                break
            else:
                info = line.split(":")
                instance_features[info[0].strip()] = info[1].strip()
                index_NODE_COORD_SECTION +=1


        instance_features['CAPACITY'] = int(instance_features['CAPACITY'])
        instance_features['DIMENSION'] = int(instance_features['DIMENSION'])
        distance = [ [0 for j in range(0,instance_features['DIMENSION']+1)] for i in range(0,instance_features['DIMENSION']+1)   ]
        coordenadas = [ [0,0] for i in range(0,instance_features['DIMENSION']+1)   ]
        demandas = [0 for i in range(0,instance_features['DIMENSION']+1)]

        index_DEMAND_SECTION = index_NODE_COORD_SECTION

        for line in lines[index_NODE_COORD_SECTION:]:
            if line.strip() == "DEMAND_SECTION":
                index_DEMAND_SECTION +=1
                break
            x = line.split('\t')
            x[0] = int(x[0].strip())
            x[1] = int(x[1].strip())
            x[2] = int(x[2].strip())

            coordenadas[x[0]] = [x[1],x[2]]
            index_DEMAND_SECTION +=1

        for line in lines[index_DEMAND_SECTION:]:
            if line.strip() == "DEPOT_SECTION":
                break

            x = line.split('\t')
            x[0] = int(x[0].strip())
            x[1] = int(x[1].strip())
            demandas[x[0]] = x[1]

        closest_customers  = [[] for i in range(0,instance_features['DIMENSION']+1)]
        for customer_i in range(2,instance_features['DIMENSION']+1):

            aux_adjacent_customers = []
            for customer_j in range(2,instance_features['DIMENSION']+1):
                if customer_j != customer_i:

                    distance_i_j = ((coordenadas[customer_i][0] - coordenadas[customer_j][0])**2 + (coordenadas[customer_i][1] - coordenadas[customer_j][1])**2)**(0.5)
                    aux_adjacent_customers.append(( customer_j, distance_i_j))
            aux_adjacent_customers = sorted(aux_adjacent_customers, key=itemgetter(1))

            closest_customers[customer_i] = aux_adjacent_customers[:100]
          
        
        
        new_dimension = 101
        datasets = []
        for customer_i in range(2,instance_features['DIMENSION']+1):
        #     closest_customers[customer_i]
        #     filename =  instance_name + "/" + instance_name + "_customer"+ str(customer_i) +".vrp"
        #     with open(filename, 'w') as f:
        #         f.write("\n".join([
        #             "{} : {}".format(k, v)
        #             for k, v in (
        #                 ("NAME", filename),
        #                 ("TYPE", "CVRP"),
        #                 ("DIMENSION", new_dimension),
        #                 ("EDGE_WEIGHT_TYPE", "EUC_2D"),
        #                 ("CAPACITY", instance_features['CAPACITY'])
        #             )
        #         ]))
        #         f.write("\n")
        #         f.write("NODE_COORD_SECTION\n")
        #         x, y = coordenadas[1]
        #         f.write("{}\t{}\t{}\n".format(1,x,y))
        #         f.write("\n".join(["{}\t{}\t{}".format(key+2, coordenadas[tupla[0]][0],coordenadas[tupla[0]][1]) for key,tupla in enumerate(closest_customers[customer_i])]))
        #         f.write("\n")
        #         f.write("DEMAND_SECTION\n")
        #         f.write("{}\t{}\n".format(1,0))
        #         f.write("\n".join(["{}\t{}".format(key+2, demandas[tupla[0]]) for key,tupla in enumerate(closest_customers[customer_i])]))
        #         f.write("\n")
        #         f.write("DEPOT_SECTION\n")
        #         f.write("1\n")
        #         f.write("-1\n")
        #         f.write("EOF\n")

            dataset = (coordenadas[1],
            [[coordenadas[tupla[0]][0], coordenadas[tupla[0]][1]] for key, tupla in enumerate(closest_customers[customer_i])],
            [demandas[tupla[0]] for key,tupla in enumerate(closest_customers[customer_i])],
            instance_features['CAPACITY'],
             2,
             0,
             1000)
            datasets.append(dataset)
        if not(os.path.exists("dpdp/data/vrp/")):
            os.makedirs("dpdp/data/vrp/")
        
        with open("dpdp/data/vrp/" + instance_name + ".pkl", 'wb') as f:
            pickle.dump(datasets, f, pickle.HIGHEST_PROTOCOL)
        