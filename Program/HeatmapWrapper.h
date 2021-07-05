
#ifndef HEATMAP_WRAPPER_H
#define HEATMAP_WRAPPER_H

#include <iostream>
#include <string>
#include <climits>
#include <vector>
#include <fstream>
class HeatmapWrapper
{    
    public:
        std::vector<int> bigTourDPDP;
        std::string instanceName;
        bool validHeatmap;
 
        HeatmapWrapper(const std::string &instancePath)
        {
            instanceName = instancePath.substr(instancePath.find_last_of("/\\") + 1);
            validHeatmap = false;
        }

        void processBigTourDPDP()
        {
            std::string command;
            int res;

            command = "cd heatmap/ && python generate_data.py --name train --instance ../../Instances/CVRP/" + instanceName + " --seed 1 -f";
            res = system(command.c_str());
            std::cout << "Step #1 - generate data completed." << res << std::endl; 


            command = "cd heatmap/ && python -m problems.vrp.vrp_baseline lkh data/vrp/" + instanceName + "_train_seed1.pkl --no_cuda --disable_cache --lkh_seed 1 -f";
            res = system(command.c_str());
            std::cout << "Step #2.1 - example solution train." << res << std::endl; 

            command = "cd heatmap/ && python -m problems.vrp.vrp_baseline lkh data/vrp/" + instanceName + "_train_seed1.pkl --no_cuda --disable_cache --lkh_seed 2 -f";
            res = system(command.c_str());
            std::cout << "Step #2.2 - example solution test." << res << std::endl; 

            command = "cd heatmap/ && python -m problems.vrp.vrp_baseline lkh data/vrp/" + instanceName + "_train_seed1.pkl --no_cuda --disable_cache --lkh_seed 3 -f";
            res = system(command.c_str());
            std::cout << "Step #2.3 - example solution validation." << res << std::endl; 

            command = "cd heatmap/ && CUDA_VISIBLE_DEVICES=0,0 python main_vrp.py --config configs/vrp_configs.json --placeholder " + instanceName + "";
            res = system(command.c_str());
            std::cout << "Step #3 - train model." << res << std::endl; 


            command = "cd heatmap/ && CUDA_VISIBLE_DEVICES=0,0 python export_heatmap.py --problem vrp --checkpoint logs/vrp_" + instanceName + "/best_val_loss_checkpoint.tar --instances data/vrp/" + instanceName + "_train_seed1.pkl -f --batch_size 1";
            res = system(command.c_str());
            std::cout << "Step #4 - export heatmap." << res << std::endl; 


            command = "cd heatmap/ && CUDA_VISIBLE_DEVICES=0,0 python eval.py data/vrp/" + instanceName + "_train_seed1.pkl --problem cvrp --decode_strategy dpdp --score_function heatmap_potential --beam_size 1000 --heatmap_threshold 1e-5 --heatmap results/vrp/" + instanceName + "_train_seed1/heatmaps/heatmaps_vrp_" + instanceName + ".pkl -f";
            command = "cd heatmap/ && CUDA_VISIBLE_DEVICES=0,0 python eval.py data/vrp/" + instanceName + "_train_seed1.pkl --problem cvrp --decode_strategy dpdp --score_function heatmap_potential --beam_size 10 --heatmap_threshold 0 --heatmap results/vrp/" + instanceName + "_train_seed1/heatmaps/heatmaps_vrp_" + instanceName + ".pkl -f";

            res = system(command.c_str());
            std::cout << command << std:: endl;

            std::cout << "Step #5 - execute DPDP." << res << std::endl; 

            // Retrieve bigtour from file
            std::string filename = "heatmap/" + instanceName + "_bigtour.txt";
            std::ifstream bigTourFile(filename);
            std::string content;
            if (bigTourFile.is_open())
            {
                while(bigTourFile >> content)
                    this->bigTourDPDP.push_back(std::atoi(content.c_str()));

                validHeatmap = (this->bigTourDPDP.size() > 0);

                std::cout << "Big Tour  ";
                for(int i =0 ; i < this->bigTourDPDP.size() ; i++)
                    std::cout << this->bigTourDPDP[i] <<" ";
            }
        }
};
#endif
