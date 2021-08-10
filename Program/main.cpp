#include "Genetic.h"
#include "commandline.h"
#include "LocalSearch.h"
#include "Split.h"
#include "Mining.h"
#include "DPDPWrapper.h"

using namespace std;

bool orderPairSecond(const std::pair<int, double> &a, const std::pair<int, double> &b)
{
	return a.second < b.second;
}

int main(int argc, char *argv[])
{
	try
	{
		// Reading the arguments of the program
		CommandLine commandline(argc, argv);

		// Reading the data file and initializing some data structures
		std::cout << "----- READING DATA SET: " << commandline.pathInstance << std::endl;
		Params params(commandline.pathInstance, commandline.seed, commandline.crossoverType, commandline.useDPDP, commandline.nbVeh, commandline.heatThresholdType);

		// Creating the Split and local search structures
		Split split(&params);

		// Creating the structure to mine frequent sequences
		Mining mining(&params);

		LocalSearch localSearch(&params);

		if (params.useDPDP > 0)
		{
			std::cout << "useDPDP: " << params.useDPDP << std::endl;
			std::string instanceBaseName = params.pathToInstance.substr(params.pathToInstance.find_last_of("/\\") + 1);
			std::string heatmapName = instanceBaseName.substr(0, instanceBaseName.find_last_of("."));
			std::string heatmapFullPath = "../Heatmaps/" + heatmapName + ".hm";
			std::ifstream heatmapFile(heatmapFullPath);
			std::string content;

			//Avoiding baseline to enter
			if (heatmapFile.is_open() && (params.useDPDP != 1 || (params.useDPDP == 1 && params.crossoverType == 9)) && (params.useDPDP != 2 || (params.useDPDP == 2 && params.crossoverType == 9)))
			{
				for (int i = 0; i <= (int)params.nbClients; i++)
				{

					double edge_heat = 0.0;
					std::vector<std::pair<int, double>> heatList;

					double bestHeatValue = -1.0;
					int bestHeatCustomer = 1;
					for (int customer_id = 0; customer_id <= params.nbClients; customer_id++)
					{
						heatmapFile >> edge_heat;

						if (edge_heat > bestHeatValue && customer_id != 0)
						{
							bestHeatValue = edge_heat;
							bestHeatCustomer = customer_id;
						}

						//we ignore 'heats' between customer - depot;
						//we also ignore 'heats' that points customer i to itself
						if (customer_id > 0 && edge_heat >= params.heat_threshold && customer_id != i)
						{
							heatList.push_back(std::make_pair(customer_id, -edge_heat));
						}
					}
					params.bestCustomerHeat.push_back(bestHeatCustomer);

					std::sort(heatList.begin(), heatList.end(), orderPairSecond);

					//Options 3 and 4 are ordered with removal
					if (params.useDPDP == 3 || params.useDPDP == 4)
						params.correlatedVertices[i].clear();

					for (int j = 0; j < heatList.size() && j < params.nbGranular; j++)
					{
						if (std::find(params.correlatedVertices[i].begin(), params.correlatedVertices[i].end(), heatList[j].first) == params.correlatedVertices[i].end())
						{
							params.correlatedVertices[i].push_back(heatList[j].first);
						}
					}
				}
			}
		}

		DPDPWrapper dpdp(&params);

		// Initial population
		std::cout << "----- INSTANCE LOADED WITH " << params.nbClients << " CLIENTS AND " << params.nbVehicles << " VEHICLES" << std::endl;
		std::cout << "----- BUILDING INITIAL POPULATION" << std::endl;
		Population population(&params, &split, &localSearch, &mining);

		// Genetic algorithm
		std::cout << "----- STARTING GENETIC ALGORITHM - CROSSOVER TYPE: " << params.crossoverType << std::endl;
		Genetic solver(&params, &split, &population, &localSearch, &mining, &dpdp);
		solver.run(commandline.nbIter, commandline.timeLimit);
		std::cout << "----- GENETIC ALGORITHM FINISHED, TIME SPENT: " << (double)clock() / (double)CLOCKS_PER_SEC << std::endl;

		// Exporting the best solution

		std::string instanceBaseName = params.pathToInstance.substr(params.pathToInstance.find_last_of("/\\") + 1);
		std::ofstream allSolutionsFile("Solutions-DPDP/solutions_useDPDP" + std::to_string(params.useDPDP) + "_" + std::to_string(commandline.timeLimit) + "sec_crossover" + std::to_string(commandline.crossoverType) + "_" + instanceBaseName + ".txt", std::ios::out | std::ios::app);
		if (population.getBestFound() != NULL)
		{
			allSolutionsFile << std::to_string(population.getBestFound()->myCostSol.penalizedCost) << std::endl;
			population.getBestFound()->exportCVRPLibFormat(commandline.pathSolution);
			population.exportSearchProgress(commandline.pathSolution + ".PG.csv", commandline.pathInstance, commandline.seed);
			if (commandline.pathBKS != "")
				population.exportBKS(commandline.pathBKS);
		}
		else
		{
			allSolutionsFile << "Exporting best solution is not valid" << std::endl;
		}
		allSolutionsFile.close();
	}
	catch (const string &e)
	{
		std::cout << "EXCEPTION | " << e << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "EXCEPTION | " << e.what() << std::endl;
	}
	return 0;
}
