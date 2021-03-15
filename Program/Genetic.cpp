#include "Genetic.h"

#define CrossTypeEAX

void Genetic::run(int maxIterNonProd, int timeLimit)
{
	int nbIterNonProd = 1;
	int nbIter;
	for (nbIter = 0; nbIterNonProd <= maxIterNonProd && clock() / CLOCKS_PER_SEC < timeLimit; nbIter++)
	{
		/* SELECTION AND CROSSOVER */
		// crossoverOX(offspring, population->getBinaryTournament(), population->getBinaryTournament());

		crossoverEAX(offspring, population->getBinaryTournament(), population->getBinaryTournament());

		/* LOCAL SEARCH */
		localSearch->run(offspring, params->penaltyCapacity, params->penaltyDuration);
		bool isNewBest = population->addIndividual(offspring, true);
		if (!offspring->isFeasible && std::rand() % 2 == 0) // Repair half of the solutions in case of infeasibility
		{
			localSearch->run(offspring, params->penaltyCapacity * 10., params->penaltyDuration * 10.);
			if (offspring->isFeasible)
				isNewBest = (population->addIndividual(offspring, false) || isNewBest);
		}

		/* TRACKING THE NUMBER OF ITERATIONS SINCE LAST SOLUTION IMPROVEMENT */
		if (isNewBest)
			nbIterNonProd = 1;
		else
			nbIterNonProd++;

		/* DIVERSIFICATION, PENALTY MANAGEMENT AND TRACES */
		if (nbIter % 100 == 0)
			population->managePenalties();

		if (nbIter % 500 == 0)
			population->printState(nbIter, nbIterNonProd);

		/* FOR TESTS INVOLVING SUCCESSIVE RUNS UNTIL A TIME LIMIT: WE RESET THE ALGORITHM/POPULATION EACH TIME maxIterNonProd IS ATTAINED*/
		if (timeLimit != INT_MAX && nbIterNonProd == maxIterNonProd)
		{
			population->restart();
			nbIterNonProd = 1;
		}
	}

	std::cout << "oxCalls: " << oxCalls << std::endl;
	std::cout << "nbIter: " << nbIter << std::endl;
}

void Genetic::crossoverEAX(Individual *result, const Individual *parentA, const Individual *parentB)
{
	bool singleBlock = false;
	/* GAB is a list of adjacencies */
	/* We use (E_A \union E_B) \ (E_A \intersect E_B )*/
	int **GAB_A = new int *[this->params->nbClients + 1];
	int **GAB_B = new int *[this->params->nbClients + 1];

	for (int i = 0; i <= this->params->nbClients; i++)
	{
		GAB_A[i] = new int[this->params->nbClients + 1];
		GAB_B[i] = new int[this->params->nbClients + 1];
	}

	for (int i = 0; i <= this->params->nbClients; i++)
	{
		for (int j = 0; j <= this->params->nbClients; j++)
		{
			GAB_A[i][j] = 0;
			GAB_B[i][j] = 0;
		}
	}

	//1st Phase - Generate auxiliary graph GAB
	for (int i = 1; i <= params->nbClients; i++)
	{
		// We have two edges (forward and backward) for each client.
		// We only need to check the edge endpoints
		int A_endpoint1 = parentA->successors[i];
		int A_endpoint2 = parentA->predecessors[i];

		if (i > A_endpoint1)
		{
			GAB_A[i][A_endpoint1]++;
			GAB_A[A_endpoint1][i]++;
		}

		if (i > A_endpoint2)
		{
			GAB_A[i][A_endpoint2]++;
			GAB_A[A_endpoint2][i]++;
		}

		//Making the checkings for parentB
		int B_endpoint1 = parentB->successors[i];
		int B_endpoint2 = parentB->predecessors[i];
		if (i > B_endpoint1)
		{
			GAB_B[i][B_endpoint1]++;
			GAB_B[B_endpoint1][i]++;
		}

		if (i > B_endpoint2)
		{
			GAB_B[i][B_endpoint2]++;
			GAB_B[B_endpoint2][i]++;
		}
	}
	//AB_Cycles: each AB_Cycle is a sequence of [(i,j) and parent edge]
	std::vector<std::vector<std::pair<std::pair<int, int>, bool>>> AB_cycles;
	// 2nd phase - Create AB-cycles
	bool hasEdges = true;
	while (hasEdges)
	{
		hasEdges = false;
		int predecessor = -1;
		//We find an initial node: predecessor
		bool edgeParent = true;
		for (int i = 0; i <= this->params->nbClients; i++)
		{
			for (int j = 0; j <= this->params->nbClients; j++)
			{
				if (GAB_A[i][j] > 0)
				{
					edgeParent = true;
					predecessor = i;
					i = this->params->nbClients + 1;
					break;
				}
				else if (GAB_B[i][j] > 0)
				{
					edgeParent = false;
					predecessor = i;
					i = this->params->nbClients + 1;
					break;
				}
			}
		}

		//We select its first adjacent node to obtain its origin (parentA or parentB)
		//The successor node will be anyway selected in the first iteration of the next loop

		//To store the cycle's vertices
		std::vector<std::pair<std::pair<int, int>, bool>> giant_cycle;

		//To access all used edges (avoid repetitions or back-and-forth situations)
		std::multiset<std::pair<std::set<int>, bool>> usedEdges;
		while (1)
		{
			//We select the successor node (note: the loop will have at most 4 elements, except depot)
			int successor = -1;
			int indexSuccessor = 0;
			double bestCostInsertion = 1e24;

			for (int i = 0; i <= params->nbClients; i++)
			{
				if ((edgeParent && GAB_A[predecessor][i] > 0) || (!edgeParent && GAB_B[predecessor][i] > 0))
				{
					std::set<int> edge({predecessor, i});
					auto key = std::make_pair(edge, edgeParent);
					auto it = usedEdges.find(key);
					//We test two cases: (1) when the edge was not used in the cycle; (2) if we have two edges from same parent (i.e., one-client route)
					if (it == usedEdges.end() || (edgeParent && GAB_A[predecessor][i] > 1) || (!edgeParent && GAB_B[predecessor][i] > 1))
					{

						if (params->timeCost[predecessor][i] < bestCostInsertion)
						{
							successor = i;
							bestCostInsertion = params->timeCost[predecessor][i];
						}
					}
				}
			}

			if (successor < 0) //No means that this conditional is true
			{
				std::cout << "não pode entrar aqui" << std::endl;
				exit(0);
			}

			//Include the successor node into the cycle and save the edge
			std::set<int> usedEdge({predecessor, successor});
			usedEdges.insert(std::make_pair(usedEdge, edgeParent));
			giant_cycle.push_back(std::make_pair(std::make_pair(predecessor, successor), edgeParent));

			if (edgeParent)
			{
				GAB_A[predecessor][successor]--;
				GAB_A[successor][predecessor]--;
			}
			else
			{
				GAB_B[predecessor][successor]--;
				GAB_B[successor][predecessor]--;
			}

			if (giant_cycle.size() > 1 && (giant_cycle[giant_cycle.size() - 1].first.second == giant_cycle[0].first.first))
				break;

			edgeParent = !edgeParent;
			predecessor = successor;
		}

		//An AB_cycle is then formed
		AB_cycles.push_back(giant_cycle);

		//If singleBlock is true, we only need one cycle
		if (singleBlock)
			break;

		// If all edges have been used, we break the loop
		for (int i = 0; i <= this->params->nbClients; i++)
		{
			for (int j = 0; j <= this->params->nbClients; j++)
			{
				if (GAB_A[i][j] > 0 || GAB_B[i][j] > 0)
				{
					hasEdges = true;
					i = this->params->nbClients;
					break;
				}
			}
		}
	}

	//We construct the offspring based on parentA
	for (int i = 0; i < parentA->chromT.size(); i++)
		result->chromT[i] = parentA->chromT[i];

	//3rd phase - Form E_sets
	//There're two strategies:
	//Single strategy - select one AB_cycle at random
	//Block strategy - select one AB_cycle at random, then include all AB_cycles that contain at least one node of the first AB_cycle
	int indexInitialE_set = std::rand() % AB_cycles.size();
	std::vector<std::pair<std::pair<int, int>, bool>> E_set = AB_cycles[indexInitialE_set];
	std::vector<int> selectedE_sets;
	selectedE_sets.push_back(indexInitialE_set);
	if (!singleBlock)
	{
		//Have all vertices stored
		std::set<int> verticesInitialE_set;
		for (int i = 0; i < E_set.size(); i++)
		{
			if (E_set[i].first.first != 0)
				verticesInitialE_set.insert(E_set[i].first.first);
		}

		for (int i = 0; i < AB_cycles.size(); i++)
		{
			if (i == indexInitialE_set)
				continue;

			for (int j = 0; j < AB_cycles[i].size(); j++)
			{
				//If at least one vertice of an AB_cycle is common to the initial one, we save the index of the E_set
				if (AB_cycles[i][j].first.first != 0 && verticesInitialE_set.find(AB_cycles[i][j].first.first) != verticesInitialE_set.end())
				{
					selectedE_sets.push_back(i);
					break;
				}
			}
		}
	}

	//We iterate over all selected E_sets
	for (int k = 0; k < selectedE_sets.size(); k++)
	{
		E_set = AB_cycles[selectedE_sets[k]];

		//From the E_set, we introduce all edges.
		//When the first occurrence of the edge (one endpoint is found),
		//The another endpoint is placed right after it and the remaining nodes are reversed and shifted to right
		//We don't "merge" subtours. We just place them on the bigtour and the split algorithm will take of making this job.

		for (int i = 0; i < E_set.size(); i++)
		{
			//So far, we avoid edges that contain the depot
			if (E_set[i].first.first == 0 || E_set[i].first.second == 0)
				continue;
			//We only regard edges that are from parentB, since the solution is based on parentA
			else if (!E_set[i].second)
			{
				//Node found
				//The next nodes are stored and shifted right after the node adjacent of it in the cycle
				std::vector<int> remainingNodes;
				remainingNodes.push_back(0);
				for (int j = 0; j < result->chromT.size(); j++)
				{
					if (result->chromT[j] != E_set[i].first.first && result->chromT[j] != E_set[i].first.second)
						remainingNodes.push_back(result->chromT[j]);
				}
				remainingNodes.push_back(0);

				int bestIndexInsertion = 0; // index of chromT
				double bestCostInsertion = params->timeCost[0][E_set[i].first.first] + params->timeCost[E_set[i].first.second][result->chromT[0]];
				bool flip = false;
				//We search for the best insertion of the arc
				for (int j = 0; j < remainingNodes.size() - 1; j++)
				{
					if (params->timeCost[remainingNodes[j]][E_set[i].first.first] + params->timeCost[E_set[i].first.second][remainingNodes[j + 1]] < bestCostInsertion)
					{
						bestCostInsertion = params->timeCost[remainingNodes[j]][E_set[i].first.first] + params->timeCost[E_set[i].first.second][remainingNodes[j + 1]];
						bestIndexInsertion = j;
						flip = false;
					}
					if (params->timeCost[remainingNodes[j]][E_set[i].first.second] + params->timeCost[E_set[i].first.first][remainingNodes[j + 1]] < bestCostInsertion)
					{
						bestCostInsertion = params->timeCost[remainingNodes[j]][E_set[i].first.second] + params->timeCost[E_set[i].first.first][remainingNodes[j + 1]];
						bestIndexInsertion = j;
						flip = true;
					}
				}
				//Select 0, then we preserve index 0 and put on the right
				int remainingNodesIndex = 1;
				for (int j = 0; j < result->chromT.size(); j++)
				{
					if (j == bestIndexInsertion)
					{
						if (flip)
						{
							result->chromT[j] = E_set[i].first.second;
							j++;
							result->chromT[j] = E_set[i].first.first;
						}
						else
						{
							result->chromT[j] = E_set[i].first.first;
							j++;
							result->chromT[j] = E_set[i].first.second;
						}
					}
					else
					{
						result->chromT[j] = remainingNodes[remainingNodesIndex++];
					}
				}
				break;
			}
		}
	}
	// Completing the individual with the Split algorithm
	split->generalSplit(result, parentA->myCostSol.nbRoutes);

	for (int i = 0; i <= this->params->nbClients; i++)
	{
		delete[] GAB_A[i];
		delete[] GAB_B[i];
	}
	delete[] GAB_A;
	delete[] GAB_B;
}

void Genetic::crossoverOX(Individual *result, const Individual *parent1, const Individual *parent2)
{
	// Frequency table to track the customers which have been already inserted
	std::vector<bool> freqClient = std::vector<bool>(params->nbClients + 1, false);

	// Picking the beginning and end of the crossover zone
	int start = std::rand() % params->nbClients;
	int end = std::rand() % params->nbClients;
	while (end == start)
		end = std::rand() % params->nbClients;

	// Copy in place the elements from start to end (possibly "wrapping around" the end of the array)
	int j = start;
	while (j % params->nbClients != (end + 1) % params->nbClients)
	{
		result->chromT[j % params->nbClients] = parent1->chromT[j % params->nbClients];
		freqClient[result->chromT[j % params->nbClients]] = true;
		j++;
	}

	// Fill the remaining elements in the order given by the second parent
	for (int i = 1; i <= params->nbClients; i++)
	{
		int temp = parent2->chromT[(end + i) % params->nbClients];
		if (freqClient[temp] == false)
		{
			result->chromT[j % params->nbClients] = temp;
			j++;
		}
	}

	// Completing the individual with the Split algorithm
	split->generalSplit(result, parent1->myCostSol.nbRoutes);
}

Genetic::Genetic(Params *params, Split *split, Population *population, LocalSearch *localSearch) : params(params), split(split), population(population), localSearch(localSearch)
{
	offspring = new Individual(params);
}

Genetic::~Genetic(void)
{
	delete offspring;
}
