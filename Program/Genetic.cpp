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
	//We must have two distinct solutions. To avoid many computations, we compare by the solution cost value
	while (parentA->myCostSol.penalizedCost == parentB->myCostSol.penalizedCost)
		parentB = this->population->getBinaryTournament();

	//Two strategies in genering the E_sets and merging them
	//Single strategy - select one AB_cycle at random
	//Block strategy - select one AB_cycle at random, then include all AB_cycles that contain at least one node of the first AB_cycle
	bool singleStrategy = false;

	/* GAB is an adjacency matrix */
	/* We use the edges from the following resulting set: (E_A \union E_B) \ (E_A \intersect E_B )*/
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
		// We only need to check both edge endpoints
		int A_successor = parentA->successors[i];
		int A_predecessor = parentA->predecessors[i];
		int B_successor = parentB->successors[i];
		int B_predecessor = parentB->predecessors[i];
		if (A_successor != B_successor && A_successor != B_predecessor && B_successor != B_predecessor)
		{
			GAB_A[i][A_successor] = 1;
			GAB_A[A_successor][i] = 1;
		}

		if (A_predecessor != B_successor && A_predecessor != B_predecessor && B_successor != B_predecessor)
		{
			GAB_A[i][A_predecessor] = 1;
			GAB_A[A_predecessor][i] = 1;
		}

		if (A_successor == A_predecessor)
		{
			GAB_A[i][A_predecessor]++;
			GAB_A[A_predecessor][i]++;
		}

		//Making the checkings for parentB
		if (B_successor != A_successor && B_successor != A_predecessor && A_successor != A_predecessor)
		{
			GAB_B[i][B_successor] = 1;
			GAB_B[B_successor][i] = 1;
		}

		if (B_predecessor != A_successor && B_predecessor != A_predecessor && A_successor != A_predecessor)
		{
			GAB_B[i][B_predecessor] = 1;
			GAB_B[B_predecessor][i] = 1;
		}

		if (B_successor == B_predecessor)
		{
			GAB_B[i][B_predecessor]++;
			GAB_B[B_predecessor][i]++;
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
		//We select an adjacent node to obtain its origin (parentA or parentB) as edgeParent
		//It guarantees that an initial edge exists at the first iteration.
		bool edgeParent = true;
		for (int i = 0; i <= this->params->nbClients; i++)
		{
			for (int j = i + 1; j <= this->params->nbClients; j++)
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

		//To store the cycle's vertices
		std::vector<std::pair<std::pair<int, int>, bool>> giant_cycle;

		//To access all used edges (avoid repetitions or back-and-forth situations)
		std::multiset<std::pair<std::set<int>, bool>> usedEdges;
		while (1)
		{
			//We select the successor node having the predecessor already set
			//This selection is based on the best arc to be selected (full greedy here)
			int successor = -1;
			double bestCostInsertion = 1e24;

			for (int i = 0; i <= params->nbClients; i++)
			{
				//Checking if there're available edges
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
				std::cout << "There's a bug! Please, check!" << std::endl;
				exit(0);
			}

			//Include the successor node into the cycle and save the edge
			std::set<int> usedEdge({predecessor, successor});
			usedEdges.insert(std::make_pair(usedEdge, edgeParent));
			giant_cycle.push_back(std::make_pair(std::make_pair(predecessor, successor), edgeParent));

			//Update edge usage
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

			// If a cycle was formed, we break the loop
			if (giant_cycle.size() > 1 && (giant_cycle[giant_cycle.size() - 1].first.second == giant_cycle[0].first.first))
				break;

			// Otherwise, update predecessor and flip edgeParent
			edgeParent = !edgeParent;
			predecessor = successor;
		}

		//An AB_cycle is then formed
		AB_cycles.push_back(giant_cycle);

		//If singleStrategy is true, we only need one cycle, break the main loop
		if (singleStrategy)
			break;

		// If all edges have been used, we break the main loop
		for (int i = 0; i <= this->params->nbClients; i++)
		{
			for (int j = i + 1; j <= this->params->nbClients; j++)
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
	int indexAB_center = std::rand() % AB_cycles.size();
	std::vector<std::pair<std::pair<int, int>, bool>> E_set = AB_cycles[indexAB_center];
	std::vector<int> indexSelectedE_sets;
	indexSelectedE_sets.push_back(indexAB_center);
	if (!singleStrategy)
	{
		//Have all vertices stored
		std::set<int> verticesInitialE_set;
		for (int i = 0; i < E_set.size(); i++)
			verticesInitialE_set.insert(E_set[i].first.first);

		for (int i = 0; i < AB_cycles.size(); i++)
		{
			if (i == indexAB_center)
				continue;

			for (int j = 0; j < AB_cycles[i].size(); j++)
			{
				//If at least one vertice of an AB_cycle is common to the initial one, we save the index of the E_set
				if (verticesInitialE_set.find(AB_cycles[i][j].first.first) != verticesInitialE_set.end())
				{
					indexSelectedE_sets.push_back(i);
					break;
				}
			}
		}
	}

	//We iterate over all selected E_sets
	for (int k = 0; k < indexSelectedE_sets.size(); k++)
	{
		E_set = AB_cycles[indexSelectedE_sets[k]];

		//From the E_set, we introduce all edges.
		//For each arc (and its reverse ordering), we select the best place for its insertion within chromT
		//So far, We don't consider arcs involving the depot
		for (int i = 0; i < E_set.size(); i++)
		{
			//So far, we avoid edges that contain the depot
			if (E_set[i].first.first == 0 || E_set[i].first.second == 0)
				continue;
			//We only regard edges that are from parentB, since the solution is based on parentA
			else if (!E_set[i].second)
			{
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
						result->chromT[j] = remainingNodes[remainingNodesIndex++];
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
