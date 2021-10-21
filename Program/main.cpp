#include "Genetic.h"
#include "commandline.h"
#include "LocalSearch.h"
#include "Split.h"
#include "Mining.h"

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
		Params params(commandline.pathInstance, commandline.seed, commandline.crossoverType, commandline.useDPDP, commandline.nbVeh, commandline.heatmapThreshold);

		// Creating the Split and local search structures
		Split split(&params);

		// Creating the structure to mine frequent sequences
		Mining mining(&params);

		LocalSearch localSearch(&params);

		if (params.useDPDP || params.crossoverType == 9)
		{
			std::cout << "Using DPDP: " << params.useDPDP << std::endl;
			std::cout << "params.heatmapThreshold: " << params.heatmapThreshold << std::endl;
			std::string instanceBaseName = params.pathToInstance.substr(params.pathToInstance.find_last_of("/\\") + 1);
			std::string heatmapName = instanceBaseName.substr(0, instanceBaseName.find_last_of("."));
			bool isUchoaInstance = (heatmapName.substr(0,3) == "X-n");
			if (isUchoaInstance)
			{
				std::string heatmapFullPath = "DPDP/Heatmaps_for_HGS/" + heatmapName + "/" + heatmapName;
				params.bestCustomerHeat.push_back(0);
				for (int client_i = 1; client_i <= params.nbClients; client_i++)
				{
					std::ifstream heatmapFile(heatmapFullPath + "_" + std::to_string(client_i) + ".hm");
					if (heatmapFile.is_open())
					{
						double edge_heat = 0.0;
						std::vector<std::pair<int, double>> heatList;

						double bestHeatValue = -1.0;
						int bestHeatCustomer = 1;
						for (int customer_id = 0; customer_id <= 100; customer_id++)
						{
							heatmapFile >> edge_heat;
						}
						for (int customer_id = 0; customer_id <= 100; customer_id++)
						{
							if (customer_id > 0)
							{
								heatmapFile >> edge_heat;

								// we entirely ignore 'heats' involving the depot
								if (edge_heat > bestHeatValue)
								{
									bestHeatValue = edge_heat;
									bestHeatCustomer = customer_id;
								}
								// we also ignore 'heats' that points customer i to itself
								//  if (edge_heat >= params.heatmapThreshold)
								//  {
								heatList.push_back(std::make_pair(customer_id, -edge_heat));
								// }
							}
						}
						// std::cout << "heatList.size(): " << heatList.size() << std::endl;
						// The save the best heat of each customer for crossover (if enabled)
						if (params.crossoverType == 9)
						{
							params.bestCustomerHeat.push_back(params.closestVertices[client_i][bestHeatCustomer]);
						}
						if (params.useDPDP)
						{
							// Reset the original list of correlatedVertices (closest criteria)
							params.correlatedVertices[client_i].clear();

							// Sort list of heats and including them into correlatedVertices
							std::sort(heatList.begin(), heatList.end(), orderPairSecond);

							for (int j = 0; j < heatList.size() && j < (params.nbGranular + 2); j++)
							{
								params.correlatedVertices[client_i].push_back(params.closestVertices[client_i][heatList[j].first]);
							}
						}
					}
				}
			}
			else
			{

				std::string heatmapFullPath = "../Heatmaps/" + heatmapName + ".hm";
				std::ifstream heatmapFile(heatmapFullPath);
				std::string content;

				// Avoiding baseline to enter
				if (heatmapFile.is_open())
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
							// we entirely ignore 'heats' involving the depot
							if (customer_id > 0)
							{
								if (edge_heat > bestHeatValue)
								{
									bestHeatValue = edge_heat;
									bestHeatCustomer = customer_id;
								}
								// we also ignore 'heats' that points customer i to itself
								// edge_heat >= params.heatmapThreshold && 
								if (customer_id != i)
								{
									heatList.push_back(std::make_pair(customer_id, -edge_heat));
								}
							}
						}

						// The save the best heat of each customer for crossover (if enabled)
						if (params.crossoverType == 9)
							params.bestCustomerHeat.push_back(bestHeatCustomer);

						if (params.useDPDP)
						{
							// Reset the original list of correlatedVertices (closest criteria)
							params.correlatedVertices[i].clear();

							// Sort list of heats and including them into correlatedVertices
							std::sort(heatList.begin(), heatList.end(), orderPairSecond);

							for (int j = 0; j < heatList.size() && j < params.nbGranular; j++)
								params.correlatedVertices[i].push_back(heatList[j].first);
						}
					}
				}
			}
		}

		// Initial population
		std::cout << "----- INSTANCE LOADED WITH " << params.nbClients << " CLIENTS AND " << params.nbVehicles << " VEHICLES" << std::endl;
		std::cout << "----- BUILDING INITIAL POPULATION" << std::endl;
		Population population(&params, &split, &localSearch, &mining);

		// Genetic algorithm
		std::cout << "----- STARTING GENETIC ALGORITHM - CROSSOVER TYPE: " << params.crossoverType << std::endl;
		Genetic solver(&params, &split, &population, &localSearch, &mining);
		solver.run(commandline.nbIter, commandline.timeLimit);
		std::cout << "----- GENETIC ALGORITHM FINISHED, TIME SPENT: " << (double)clock() / (double)CLOCKS_PER_SEC << std::endl;

		// Exporting the best solution

		std::string instanceBaseName = params.pathToInstance.substr(params.pathToInstance.find_last_of("/\\") + 1);
		std::ofstream allSolutionsFile("Solutions-DPDP/solutions_useDPDP" + std::to_string(params.useDPDP) + "_" + std::to_string(commandline.timeLimit) + "sec_crossover" + std::to_string(commandline.crossoverType) + "_" + instanceBaseName + ".txt", std::ios::out | std::ios::app);
		if (population.getBestFound() != NULL)
		{
			allSolutionsFile << std::to_string(population.getBestFound()->myCostSol.penalizedCost) << std::endl;
			population.getBestFound()->exportCVRPLibFormat(commandline.pathSolution);
			population.exportSearchProgress(commandline.pathSolution + "_crossover" + std::to_string(params.crossoverType) + ".PG.csv", commandline.pathInstance, commandline.seed);
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
