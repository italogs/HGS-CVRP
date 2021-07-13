#include "HeatmapWrapper.h"

void HeatmapWrapper::processBigTourDPDP(Population *&pop)
{

    std::string command;
    int res;
    std::vector<int> randpos = std::vector<int>(pop->feasibleSubpopulation.size());
    for(int i = 0 ; i < pop->feasibleSubpopulation.size(); i++)
        randpos[i] = i;

    std::random_shuffle(randpos.begin(), randpos.end());
    
    // Generate problem-solution pairs for testing (problem read instance, solution is from the pool)
    {
        int nbSolutions = pop->feasibleSubpopulation.size()-2;
        command = "cd heatmap/ && python generate_data.py --problem vrp --name train --instance ../../Instances/CVRP/" + instanceName + " --seed 1 -f --dataset_size "+std::to_string(nbSolutions);
        res = system(command.c_str());
        std::cout << "Step #1.1 - generate training data completed." << res << std::endl; 

        std::string line;
        std::ofstream trainInstance("heatmap/example-solutions/" + this->instanceName +  "_train.txt",std::ios::out);
        for(int k = 0; k < nbSolutions && k < pop->feasibleSubpopulation.size(); k++)
        {
            line = std::to_string(pop->feasibleSubpopulation[randpos[k]]->myCostSol.penalizedCost) + " ";
            for(int i = 0; i < pop->feasibleSubpopulation[randpos[k]]->chromR.size(); i++)
            {
                for(int j = 0; j < pop->feasibleSubpopulation[randpos[k]]->chromR[i].size(); j++)
                {
                    line += std::to_string(pop->feasibleSubpopulation[randpos[k]]->chromR[i][j]) + " ";
                }
                if(i+1 < pop->feasibleSubpopulation[randpos[k]]->chromR.size() && pop->feasibleSubpopulation[randpos[k]]->chromR[i+1].size() > 0)
                    line += "0 ";    
            }
            trainInstance << line << std::endl;
        }
        trainInstance.close();

        randpos.erase(randpos.begin(), randpos.begin() + nbSolutions);

        command = "cd heatmap/ && python -m problems.vrp.vrp_baseline hgs example-solutions/" + instanceName + "_train.txt --no_cuda --disable_cache --lkh_seed 1 -f";
        res = system(command.c_str());
        std::cout << "Step #1.1 - example solution training." << res << std::endl; 
    }

    {
        int nbSolutions = 1;
        command = "cd heatmap/ && python generate_data.py --problem vrp --name validation --instance ../../Instances/CVRP/" + instanceName + " --seed 2 -f --dataset_size "+std::to_string(nbSolutions);
        res = system(command.c_str());
        std::cout << "Step #1.2 - generate validation data completed." << res << std::endl; 

        std::string line;
        std::ofstream trainInstance("heatmap/example-solutions/" + this->instanceName +  "_validation.txt",std::ios::out);
        for(int k = 0; k < nbSolutions && k < pop->feasibleSubpopulation.size(); k++)
        {
            line = std::to_string(pop->feasibleSubpopulation[randpos[k]]->myCostSol.penalizedCost) + " ";
            for(int i = 0; i < pop->feasibleSubpopulation[randpos[k]]->chromR.size(); i++)
            {
                for(int j = 0; j < pop->feasibleSubpopulation[randpos[k]]->chromR[i].size(); j++)
                {
                    line += std::to_string(pop->feasibleSubpopulation[randpos[k]]->chromR[i][j]) + " ";
                }
                if(i+1 < pop->feasibleSubpopulation[randpos[k]]->chromR.size() && pop->feasibleSubpopulation[randpos[k]]->chromR[i+1].size() > 0)
                    line += "0 ";    
            }
            trainInstance << line << std::endl;
        }
        trainInstance.close();
        
        randpos.erase(randpos.begin(), randpos.begin() + nbSolutions);
        
        command = "cd heatmap/ && python -m problems.vrp.vrp_baseline hgs example-solutions/" + instanceName + "_validation.txt --no_cuda --disable_cache --lkh_seed 2 -f";
        res = system(command.c_str());
        std::cout << "Step #1.2 - example solution validation." << res << std::endl; 
    }

    // Generate problem-solution pairs for training
    // {
    //     command = "cd heatmap/ && python generate_data.py --problem vrp --name train --seed 1 -f --dataset_size 50";
    //     res = system(command.c_str());
    //     std::cout << "Step #1.1 - generate training data completed." << res << std::endl; 

    //     command = "cd heatmap/ && python -m problems.vrp.vrp_baseline lkh data/vrp/vrp100_train_seed1.pkl --no_cuda --disable_cache --lkh_seed 1 -f";
    //     res = system(command.c_str());
    //     std::cout << "Step #1.1 - example solution train." << res << std::endl; 
    // }

    // // Generate problem-solution pairs for validation
    // {
    //     command = "cd heatmap/ && python generate_data.py --problem vrp --name validation --seed 2 -f --dataset_size 5";
    //     res = system(command.c_str());
    //     std::cout << "Step #1.2 - generate validation data completed." << res << std::endl; 

    //     command = "cd heatmap/ && python -m problems.vrp.vrp_baseline lkh data/vrp/vrp100_validation_seed2.pkl --no_cuda --disable_cache --lkh_seed 2 -f";
    //     res = system(command.c_str());
    //     std::cout << "Step #1.2 - example solution validation." << res << std::endl; 
    // }
    
    // Generate problem-solution pairs for testing (problem read instance, solution is from the pool)
    {
        int nbSolutions = 1;
        command = "cd heatmap/ && python generate_data.py --problem vrp --name test --instance ../../Instances/CVRP/" + instanceName + " --seed 3 -f --dataset_size "+std::to_string(nbSolutions);
        res = system(command.c_str());
        std::cout << "Step #1.3 - generate test data completed." << res << std::endl; 

        std::string line;
        std::ofstream trainInstance("heatmap/example-solutions/" + this->instanceName +  "_test.txt",std::ios::out);
        for(int k = 0; k < nbSolutions && k < pop->feasibleSubpopulation.size(); k++)
        {
            line = std::to_string(pop->feasibleSubpopulation[randpos[k]]->myCostSol.penalizedCost) + " ";
            for(int i = 0; i < pop->feasibleSubpopulation[randpos[k]]->chromR.size(); i++)
            {
                for(int j = 0; j < pop->feasibleSubpopulation[randpos[k]]->chromR[i].size(); j++)
                {
                    line += std::to_string(pop->feasibleSubpopulation[randpos[k]]->chromR[i][j]) + " ";
                }
                if(i+1 < pop->feasibleSubpopulation[randpos[k]]->chromR.size() && pop->feasibleSubpopulation[randpos[k]]->chromR[i+1].size() > 0)
                    line += "0 ";    
            }
            trainInstance << line << std::endl;
        }
        trainInstance.close();


        command = "cd heatmap/ && python -m problems.vrp.vrp_baseline hgs example-solutions/" + instanceName + "_test.txt --no_cuda --disable_cache --lkh_seed 3 -f";
        res = system(command.c_str());
        std::cout << "Step #1.3 - example solution test." << res << std::endl; 
    }

    command = "cd heatmap/ && CUDA_VISIBLE_DEVICES=0,0 python main_vrp.py --config configs/vrp_configs.json --placeholder " + instanceName + " > output_training.txt";
    res = system(command.c_str());
    std::cout << "Step #2 - train model." << res << std::endl; 


    command = "cd heatmap/ && CUDA_VISIBLE_DEVICES=0,0 python export_heatmap.py --problem vrp --checkpoint logs/vrp_" + instanceName + "/best_val_loss_checkpoint.tar --instances data/vrp/" + instanceName + "_test_seed3.pkl -f --batch_size 1 > output_heatmap.txt";
    res = system(command.c_str());
    std::cout << "Step #3 - export heatmap." << res << std::endl; 


    command = "cd heatmap/ && CUDA_VISIBLE_DEVICES=0,0 python eval.py data/vrp/" + instanceName + "_test_seed3.pkl --problem cvrp --decode_strategy dpdp --score_function heatmap_potential --beam_size 10 --heatmap_threshold 1e-5 --heatmap results/vrp/" + instanceName + "_test_seed3/heatmaps/heatmaps_vrp_" + instanceName + ".pkl -f > output_eval.txt";
    res = system(command.c_str());
    std::cout << "Step #4 - execute DPDP." << res << std::endl; 


    std::ifstream bigTour("heatmap/" + this->instanceName +  "_bigtour.txt",std::ios::in);

    Individual *randomIndiv = new Individual(params);

    int customer; 
    int posBigTour = 0;
    while (bigTour >> customer)
    {
        randomIndiv->chromT[posBigTour++] = customer;
    }
    std::cout << "posBigTour: " << posBigTour << std::endl;

    split->generalSplit(randomIndiv, params->nbVehicles);
    std::cout << "Add individual: " << pop->addIndividual(randomIndiv,true) << std::endl;



    std::cout << "randomIndiv->cost 1: " << randomIndiv->myCostSol.penalizedCost << std::endl;
    std::cout << "isFeasible " << randomIndiv->isFeasible << std::endl;
    localSearch->run(randomIndiv, params->penaltyCapacity, params->penaltyDuration);
    
    std::cout << "randomIndiv->cost 2: " << randomIndiv->myCostSol.penalizedCost << std::endl;
    std::cout << "isFeasible " << randomIndiv->isFeasible << std::endl;

    std::cout << "Add individual: " << pop->addIndividual(randomIndiv,true) << std::endl;
    
}
