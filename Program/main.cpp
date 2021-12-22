#include "Genetic.h"
#include "commandline.h"
#include "LocalSearch.h"
#include "Split.h"

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
		Params params(commandline.pathInstance, commandline.seed, commandline.crossoverType, commandline.useHeatmap, commandline.nbVeh, commandline.nbGranular);

		// Creating the Split and local search structures
		Split split(&params);

		LocalSearch localSearch(&params);

		if (params.useHeatmap || params.crossoverType == 9)
		{
			std::cout << "Using Heatmap: " << params.useHeatmap << std::endl;
			std::string instanceBaseName = params.pathToInstance.substr(params.pathToInstance.find_last_of("/\\") + 1);
			std::string heatmapName = instanceBaseName.substr(0, instanceBaseName.find_last_of("."));
			std::string heatmapFullPath = "DPDP/Heatmaps_for_HGS/" + heatmapName + "/" + heatmapName;

			bool isUchoaInstance = (heatmapName.substr(0, 3) == "X-n") && !(heatmapName.substr(0, 10) == "X-n101-k25");
			if (isUchoaInstance)
			{
				params.bestCustomerHeat.push_back(0);
				// Each client has its own file (depot + 100 closest elements)
				for (int client_i = 1; client_i <= params.nbClients; client_i++)
				{
					std::ifstream heatmapFile(heatmapFullPath + "_" + std::to_string(client_i) + ".hm");
					if (heatmapFile.is_open())
					{
						double edge_heat = 0.0;
						std::vector<std::pair<int, double>> heatList;
						double bestHeatValue = -1.0;
						int bestHeatCustomer = 1;

						// Reading the first line of the heatmap (depot line)
						for (int client_j = 0; client_j <= 100; client_j++)
							heatmapFile >> edge_heat;

						// Reading the second line of the file, which is related to client_i
						for (int client_j = 0; client_j <= 100; client_j++)
						{
							heatmapFile >> edge_heat;
							if (client_j > 0 && client_i != client_j)
							{
								// we entirely ignore 'heats' involving the depot
								if (edge_heat > bestHeatValue)
								{
									bestHeatValue = edge_heat;
									bestHeatCustomer = client_j;
								}
								// we also ignore 'heats' that points customer i to itself
								heatList.push_back(std::make_pair(client_j, -edge_heat));
							}
						}

						// The save the best heat of each customer for crossover (if enabled)
						// Careful with wrong interpretations. Each client_j refers to the jth closest customer to client_i
						// Therefore, we use closestVertices below
						if (params.crossoverType == 9)
							params.bestCustomerHeat.push_back(params.closestVertices[client_i][bestHeatCustomer]);

						if (params.useHeatmap)
						{
							// Reset the original list of correlatedVertices (closest criteria)
							params.correlatedVertices[client_i].clear();

							// Sort list of heats and including them into correlatedVertices
							std::sort(heatList.begin(), heatList.end(), orderPairSecond);

							for (int j = 0; j < heatList.size() && j < params.nbGranular / 2; j++)
							{
								params.correlatedVertices[client_i].push_back(params.closestVertices[client_i][heatList[j].first]);
							}
							int posClosest = 0;
							for (int j = 0; params.correlatedVertices[client_i].size() < params.nbGranular; j++)
							{
								if (std::find(params.correlatedVertices[client_i].begin(), params.correlatedVertices[client_i].end(), params.closestVertices[client_i][posClosest]) == params.correlatedVertices[client_i].end())
								{
									params.correlatedVertices[client_i].push_back(params.closestVertices[client_i][posClosest]);
								}
								posClosest++;
							}
						}
					}
				}
			}
			else
			{
				std::ifstream heatmapFile(heatmapFullPath + ".hm");
				std::string content;
				if (heatmapFile.is_open())
				{
					double edge_heat = 0.0;
					// Reading the first line of the heatmap (depot line)
					params.bestCustomerHeat.push_back(0);
					for (int client_i = 0; client_i <= 100; client_i++)
						heatmapFile >> edge_heat;

					for (int client_i = 1; client_i <= (int)100; client_i++)
					{
						std::vector<std::pair<int, double>> heatList;
						double bestHeatValue = -1.0;
						int bestHeatCustomer = 1;
						// Reading the line of client_i
						for (int client_j = 0; client_j <= 100; client_j++)
						{
							heatmapFile >> edge_heat;
							// we entirely ignore 'heats' involving the depot
							if (client_j > 0 && client_i != client_j)
							{
								if (edge_heat > bestHeatValue)
								{
									bestHeatValue = edge_heat;
									bestHeatCustomer = client_j;
								}
								// we also ignore 'heats' that points customer i to itself
								heatList.push_back(std::make_pair(client_j, -edge_heat));
							}
						}

						// The save the best heat of each customer for crossover (if enabled)
						if (params.crossoverType == 9)
							params.bestCustomerHeat.push_back(bestHeatCustomer);

						if (params.useHeatmap)
						{
							// Reset the original list of correlatedVertices (closest criteria)
							params.correlatedVertices[client_i].clear();

							// Sort list of heats and including them into correlatedVertices
							std::sort(heatList.begin(), heatList.end(), orderPairSecond);

							for (int j = 0; j < heatList.size() && j < params.nbGranular / 2; j++)
								params.correlatedVertices[client_i].push_back(heatList[j].first);

							int posClosest = 0;
							for (int j = 0; params.correlatedVertices[client_i].size() < params.nbGranular && posClosest < params.correlatedVertices[client_i].size(); j++)
							{
								if (std::find(params.correlatedVertices[client_i].begin(), params.correlatedVertices[client_i].end(), params.closestVertices[client_i][posClosest]) == params.correlatedVertices[client_i].end())
								{
									params.correlatedVertices[client_i].push_back(params.closestVertices[client_i][posClosest]);
								}
								posClosest++;
							}
						}
					}
				}
			}

			// Including the generation of heatmaps into the time
			std::ifstream timeSpentFile("DPDP/dpdp/results/vrp/" + heatmapName + "/heatmaps/time.txt");
			if (timeSpentFile.is_open())
				timeSpentFile >> params.time_shift_export_heatmap;
			std::cout << "Time Shift from HeatmapGeneration: " << params.time_shift_export_heatmap << std::endl;

			if (params.time_shift_export_heatmap > commandline.timeLimit)
			{
				std::cout << "params.time_shift_export_heatmap > commandline.timeLimit: " << params.time_shift_export_heatmap << ":" << commandline.timeLimit << std::endl;
				exit(0);
			}
		}

		if (params.crossoverType == 10)
		{
			params.bestCustomerClosest.push_back(0);
			for (int client_i = 1; client_i <= params.nbClients; client_i++)
			{
				int best_client_j = client_i == 1 ? 2 : 1;
				double minDistance = params.timeCost[client_i][best_client_j];

				for (int client_j = 1; client_j <= params.nbClients; client_j++)
				{
					if (client_i == client_j)
						continue;
					if (params.timeCost[client_i][client_j] < minDistance)
					{
						best_client_j = client_j;
						minDistance = params.timeCost[client_i][client_j];
					}
				}

				params.bestCustomerClosest.push_back(best_client_j);
			}
		}
		// Initial population
		std::cout << "----- INSTANCE LOADED WITH " << params.nbClients << " CLIENTS AND " << params.nbVehicles << " VEHICLES" << std::endl;
		std::cout << "----- BUILDING INITIAL POPULATION" << std::endl;
		Population population(&params, &split, &localSearch);

		// Genetic algorithm
		std::cout << "----- STARTING GENETIC ALGORITHM - CROSSOVER TYPE: " << params.crossoverType << std::endl;
		Genetic solver(&params, &split, &population, &localSearch);
		solver.run(commandline.nbIter, commandline.timeLimit);
		std::cout << "----- GENETIC ALGORITHM FINISHED, TIME SPENT: " << (double)clock() / (double)CLOCKS_PER_SEC << std::endl;

		// Exporting the best solution
		if (population.getBestFound() != NULL)
		{
			population.getBestFound()->exportCVRPLibFormat(commandline.pathSolution);
			population.exportSearchProgress(commandline.pathSolution + ".PG.csv", commandline.pathInstance, commandline.seed);
			if (commandline.pathBKS != "")
				population.exportBKS(commandline.pathBKS);
		}
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
