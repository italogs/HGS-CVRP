from operator import itemgetter, attrgetter
import pickle

all_Uchoa_instances = ["X-n101-k25", "X-n106-k14", "X-n110-k13", "X-n115-k10", "X-n120-k6", "X-n125-k30", "X-n129-k18", "X-n134-k13", "X-n139-k10", "X-n143-k7", "X-n148-k46", "X-n153-k22", "X-n157-k13", "X-n162-k11", "X-n167-k10", "X-n172-k51", "X-n176-k26", "X-n181-k23", "X-n186-k15", "X-n190-k8", "X-n195-k51", "X-n200-k36", "X-n204-k19", "X-n209-k16", "X-n214-k11", "X-n219-k73", "X-n223-k34", "X-n228-k23", "X-n233-k16", "X-n237-k14", "X-n242-k48", "X-n247-k50", "X-n251-k28", "X-n256-k16", "X-n261-k13", "X-n266-k58", "X-n270-k35", "X-n275-k28", "X-n280-k17", "X-n284-k15", "X-n289-k60", "X-n294-k50", "X-n298-k31", "X-n303-k21", "X-n308-k13", "X-n313-k71", "X-n317-k53", "X-n322-k28", "X-n327-k20", "X-n331-k15", "X-n336-k84", "X-n344-k43", "X-n351-k40", "X-n359-k29", "X-n367-k17", "X-n376-k94", "X-n384-k52", "X-n393-k38", "X-n401-k29", "X-n411-k19", "X-n420-k130", "X-n429-k61", "X-n439-k37", "X-n449-k29", "X-n459-k26", "X-n469-k138", "X-n480-k70", "X-n491-k59", "X-n502-k39", "X-n513-k21", "X-n524-k153", "X-n536-k96", "X-n548-k50", "X-n561-k42", "X-n573-k30", "X-n586-k159", "X-n599-k92", "X-n613-k62", "X-n627-k43", "X-n641-k35", "X-n655-k131", "X-n670-k130", "X-n685-k75", "X-n701-k44", "X-n716-k35", "X-n733-k159", "X-n749-k98", "X-n766-k71", "X-n783-k48", "X-n801-k40", "X-n819-k171", "X-n837-k142", "X-n856-k95", "X-n876-k59", "X-n895-k37", "X-n916-k207", "X-n936-k151", "X-n957-k87", "X-n979-k58", "X-n1001-k43"]


for instance_name in all_Uchoa_instances:
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
        with open("dpdp/data/vrp/" + instance_name + ".pkl", 'wb') as f:
            pickle.dump(datasets, f, pickle.HIGHEST_PROTOCOL)
        