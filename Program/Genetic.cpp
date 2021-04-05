#include "Genetic.h"

void Genetic::run(int maxIterNonProd, unsigned long timeLimit)
{
	int nbIterNonProd = 1;
	int nbIter;
	clock_t total_time_crossover = 0;
	clock_t total_time_local_search = 0;
	for (nbIter = 0; nbIterNonProd <= maxIterNonProd && clock() / CLOCKS_PER_SEC < timeLimit; nbIter++)
	{
		/* SELECTION AND CROSSOVER */
		clock_t crossover_start = clock();
		if (params->crossoverType == 1)
			crossoverOX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		else if (params->crossoverType == 2)
			crossoverEAX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		total_time_crossover += (clock() - crossover_start);

		/* LOCAL SEARCH */
		clock_t local_search_start = clock();
		localSearch->run(offspring, params->penaltyCapacity, params->penaltyDuration);
		bool isNewBest = population->addIndividual(offspring, true);
		if (!offspring->isFeasible && std::rand() % 2 == 0) // Repair half of the solutions in case of infeasibility
		{
			localSearch->run(offspring, params->penaltyCapacity * 10., params->penaltyDuration * 10.);
			if (offspring->isFeasible)
				isNewBest = (population->addIndividual(offspring, false) || isNewBest);
		}

		total_time_local_search += (clock() - local_search_start);

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

	std::cout << "nbIter: " << nbIter << std::endl;
	std::cout << "total_time_crossover: " << (((double)total_time_crossover) / CLOCKS_PER_SEC) << std::endl;
	std::cout << "avg per iteration total_time_crossover: " << ((((double)total_time_crossover) / CLOCKS_PER_SEC) / nbIter) << std::endl;

	std::cout << "total_time_local_search: " << (((double)total_time_local_search) / CLOCKS_PER_SEC) << std::endl;
	std::cout << "avg per iteration total_time_local_search: " << ((((double)total_time_local_search) / CLOCKS_PER_SEC) / nbIter) << std::endl;
}

void Genetic::crossoverEAX(Individual *result, const Individual *parentA, const Individual *parentB)
{
	//We must have two distinct solutions. To avoid many computations, we compare by the solution cost value
	while (parentA->myCostSol.penalizedCost == parentB->myCostSol.penalizedCost)
		parentB = this->population->getBinaryTournament();

	//1st phase define edges from ParentA and B
	/* We have an incidence matrix to store the edges from the following resulting set: (E_A \union E_B) \ (E_A \intersection E_B ) */
	/* In other words, the edges that are not common in both parents.*/
	/* Accouting the number of edges from each parent */
	for (int i = 1; i <= params->nbClients; i++)
	{
		int A_successor = parentA->successors[i];
		int A_predecessor = parentA->predecessors[i];
		int B_successor = parentB->successors[i];
		int B_predecessor = parentB->predecessors[i];

		//Using std::set for simplification to compare edges.
		std::set<int> edge_Apred({A_predecessor, i});
		std::set<int> edge_Asucc({i, A_successor});

		std::set<int> edge_Bpred({B_predecessor, i});
		std::set<int> edge_Bsucc({i, B_successor});

		// One-client routes are not allowed
		// Each if consists in verifying if the surrogate edge is present in the other parent.
		if (edge_Apred != edge_Asucc)
		{
			if (edge_Apred != edge_Bpred && edge_Apred != edge_Bsucc)
			{
				// edge_Apred
				GAB_A[A_predecessor][i] = 1;
				GAB_A[i][A_predecessor] = 1;
			}
			if (edge_Asucc != edge_Bpred && edge_Asucc != edge_Bsucc)
			{
				// edge_Asucc
				GAB_A[i][A_successor] = 1;
				GAB_A[A_successor][i] = 1;
			}
		}
		if (edge_Bpred != edge_Bsucc)
		{
			if (edge_Bpred != edge_Apred && edge_Bpred != edge_Asucc)
			{
				// edge_Bpred
				GAB_B[B_predecessor][i] = 1;
				GAB_B[i][B_predecessor] = 1;
			}
			if (edge_Bsucc != edge_Apred && edge_Bsucc != edge_Asucc)
			{
				// edge_Bsucc
				GAB_B[i][B_successor] = 1;
				GAB_B[B_successor][i] = 1;
			}
		}
	}
	//We deal with one-client routes in the two loops below
	for (size_t i = 0; i < parentA->chromR.size(); i++)
	{
		if (parentA->chromR[i].size() == 1)
		{
			//If the current route also exists in the another parent we discard both, since all edges will not be present in the resulting graph anyway.
			if (parentB->successors[parentA->chromR[i][0]] == 0 && parentB->predecessors[parentA->chromR[i][0]] == 0)
			{
				continue;
			}

			//Special case: parentA has 0-i-0 and parentB has u-i-v such that u and v are not the depot.
			//Therefore, we have two edges 0-i in A.
			if (parentB->successors[parentA->chromR[i][0]] != parentB->predecessors[parentA->chromR[i][0]] && parentB->successors[parentA->chromR[i][0]] != 0 && parentB->predecessors[parentA->chromR[i][0]] != 0)
			{
				GAB_A[0][parentA->chromR[i][0]] = 2;
				GAB_A[parentA->chromR[i][0]][0] = 2;
			}
			else
			{
				GAB_A[0][parentA->chromR[i][0]] = 1;
				GAB_A[parentA->chromR[i][0]][0] = 1;
			}
		}
	}
	for (size_t i = 0; i < parentB->chromR.size(); i++)
	{
		if (parentB->chromR[i].size() == 1)
		{
			if (parentA->successors[parentB->chromR[i][0]] == 0 && parentA->predecessors[parentB->chromR[i][0]] == 0)
			{
				continue;
			}
			if (parentA->successors[parentB->chromR[i][0]] != parentA->predecessors[parentB->chromR[i][0]] && parentA->successors[parentB->chromR[i][0]] != 0 && parentA->predecessors[parentB->chromR[i][0]] != 0)
			{
				GAB_B[0][parentB->chromR[i][0]] = 2;
				GAB_B[parentB->chromR[i][0]][0] = 2;
			}
			else
			{
				GAB_B[0][parentB->chromR[i][0]] = 1;
				GAB_B[parentB->chromR[i][0]][0] = 1;
			}
		}
	}

	// 2nd phase - Create AB-cycles:each AB_Cycle is a sequence of [(i,j) and parent edge]
	// parent edge is a boolean to identify if this edge came from parentA(true) or parentB(false)
	// old version (storing parent value)
	std::vector<std::vector<std::pair<std::pair<int, int>, bool>>> AB_cycles;
	// std::vector<std::vector<std::pair<int, int>>> AB_cycles;
	bool hasEdges = true;
	while (hasEdges)
	{
		hasEdges = false;
		int predecessor = -1;
		//We find an initial node (predecessor) for an AB_cycle.
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
					hasEdges = true;
					break;
				}
				else if (GAB_B[i][j] > 0)
				{
					edgeParent = false;
					predecessor = i;
					i = this->params->nbClients + 1;
					hasEdges = true;
					break;
				}
			}
		}
		if (!hasEdges)
			break;

		//Store the cycle's arcs and its edge parent
		//Arcs are saved as pair to maintain the its ordering
		// old version (storing parent value)
		std::vector<std::pair<std::pair<int, int>, bool>> current_AB_cycle;
		// std::vector<std::pair<int, int>> current_AB_cycle;

		while (1)
		{
			//We select the successor node having the predecessor already set
			//This selection is based on the best arc to be selected (full greedy here)
			int successor = -1;
			double bestCostInsertion = 1e24;
			for (int i = 0; i <= params->nbClients; i++)
			{
				if (predecessor == i)
					continue;
				//Checking if there're available edges, and if exists, it must be the best one
				if (((edgeParent && GAB_A[predecessor][i] > 0) || (!edgeParent && GAB_B[predecessor][i] > 0)) && (params->timeCost[predecessor][i] < bestCostInsertion))
				{
					successor = i;
					bestCostInsertion = params->timeCost[predecessor][i];
				}
			}

			//No successor found means all edges have been used. We break the main loop
			if (successor < 0)
			{
				break;
			}

			//Include the successor node into the cycle and save the edge
			// old version (storing parent value)
			current_AB_cycle.push_back(std::make_pair(std::make_pair(predecessor, successor), edgeParent));
			// current_AB_cycle.push_back(std::make_pair(predecessor, successor));

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
			if (current_AB_cycle.size() > 1 && successor == current_AB_cycle[0].first.first)
				break;

			// Otherwise, update predecessor and flip edgeParent
			edgeParent = !edgeParent;
			predecessor = successor;
		}

		//An AB_cycle is then formed
		AB_cycles.push_back(current_AB_cycle);
	}

	// 3rd phase - Select E_set
	// Two strategies in genering E_sets (or selectedAB_cycles[0])
	// Single strategy - select one AB_cycle
	// Block strategy - select one AB_cycle, then include all AB_cycles that contain at least one node in common with the first AB_cycle
	bool singleStrategy = false;

	//selectedAB_cycles[0] will be the center
	std::vector<short> selectedAB_cycles;
	selectedAB_cycles.push_back(std::rand() % AB_cycles.size());

	// Mark all vertices of the center as true (except depot)
	std::vector<bool> vertices_E_set_center = std::vector<bool>(params->nbClients + 1, false);
	for (size_t i = 0; i < AB_cycles[selectedAB_cycles[0]].size(); i++)
	{
		if (!AB_cycles[selectedAB_cycles[0]][i].second)
		{
			if (AB_cycles[selectedAB_cycles[0]][i].first.first != 0)
				vertices_E_set_center[AB_cycles[selectedAB_cycles[0]][i].first.first] = true;
			if (AB_cycles[selectedAB_cycles[0]][i].first.second != 0)
				vertices_E_set_center[AB_cycles[selectedAB_cycles[0]][i].first.second] = true;
		}
	}

	// If the condition is true, we'll pick all AB_Cycles that has, at least, one vertex in common with the center
	if (!singleStrategy)
	{
		for (size_t i = 0; i < AB_cycles.size(); i++)
		{
			if (i == selectedAB_cycles[0])
				continue;

			for (size_t j = 0; j < AB_cycles[i].size(); j++)
			{
				if (!AB_cycles[i][j].second && (vertices_E_set_center[AB_cycles[i][j].first.first] || vertices_E_set_center[AB_cycles[i][j].first.second]))
				{
					selectedAB_cycles.push_back(i);
					break;
				}
			}
		}
	}

	// 4th phase - Create offspring
	// Copy all routes from parentA to result (offspring)
	for (size_t i = 0; i < parentA->chromR.size(); i++)
	{
		if (parentA->chromR.size() > 0)
			result->chromR[i].assign(parentA->chromR[i].begin(), parentA->chromR[i].end());
	}

	//To quickly obtain a route id from a given vertex
	std::vector<short> vertexRoute = std::vector<short>(params->nbClients + 1);
	for (size_t i = 0; i < result->chromR.size(); i++)
	{
		for (size_t j = 0; j < result->chromR[i].size(); j++)
			vertexRoute[result->chromR[i][j]] = i;
	}

	//We iterate over all edges of the selected AB_Cycles
	for (size_t i = 0; i < selectedAB_cycles.size(); i++)
	{
		for (size_t j = 0; j < AB_cycles[selectedAB_cycles[i]].size(); j++)
		{
			// It's a parent B edge and its endpoints are not connected to depot
			// if (!AB_cycles[selectedAB_cycles[i]][j].second)
			if (!AB_cycles[selectedAB_cycles[i]][j].second && AB_cycles[selectedAB_cycles[i]][j].first.first != 0 && AB_cycles[selectedAB_cycles[i]][j].first.second != 0)
			{
				//Both edge's endpoints are from the same route in parentA
				if (vertexRoute[AB_cycles[selectedAB_cycles[i]][j].first.first] == vertexRoute[AB_cycles[selectedAB_cycles[i]][j].first.second] || (AB_cycles[selectedAB_cycles[i]][j].first.first == 0 || AB_cycles[selectedAB_cycles[i]][j].first.second == 0))
				{

					int route_id;
					if (AB_cycles[selectedAB_cycles[i]][j].first.first == 0 || AB_cycles[selectedAB_cycles[i]][j].first.second == 0)
					{
						if (AB_cycles[selectedAB_cycles[i]][j].first.first == 0)
							route_id = vertexRoute[AB_cycles[selectedAB_cycles[i]][j].first.second];
						else
						{
							route_id = vertexRoute[AB_cycles[selectedAB_cycles[i]][j].first.first];
							std::swap(AB_cycles[selectedAB_cycles[i]][j].first.second, AB_cycles[selectedAB_cycles[i]][j].first.first);
						}
					}
					else
						route_id = vertexRoute[AB_cycles[selectedAB_cycles[i]][j].first.first];

					int posChromR = 0;
					// We then update the route
					// Let predecessor (p) and successor (s)
					// Example: (1) - (p) - (2) - (s) - (3)
					// Resulting: (1) - (p) - (s) - reverse(2) - (3)
					for (size_t k = 0; k < result->chromR[route_id].size(); k++)
					{
						if (AB_cycles[selectedAB_cycles[i]][j].first.first == result->chromR[route_id][k] || AB_cycles[selectedAB_cycles[i]][j].first.first == 0)
						{
							// So far (1);
							std::vector<int> verticesInBetween;
							if (AB_cycles[selectedAB_cycles[i]][j].first.first == 0)
								k--;
							for (size_t p = k + 1; p < result->chromR[route_id].size(); p++)
							{
								if (AB_cycles[selectedAB_cycles[i]][j].first.second == result->chromR[route_id][p])
									break;
								verticesInBetween.push_back(result->chromR[route_id][p]);
							}
							// Then (p) - (s)
							if (AB_cycles[selectedAB_cycles[i]][j].first.first != 0)
								result->chromR[route_id][posChromR++] = AB_cycles[selectedAB_cycles[i]][j].first.first;
							result->chromR[route_id][posChromR++] = AB_cycles[selectedAB_cycles[i]][j].first.second;

							std::reverse(verticesInBetween.begin(), verticesInBetween.end());
							// Then reverse(2)
							for (size_t i = 0; i < verticesInBetween.size(); i++)
								result->chromR[route_id][posChromR++] = verticesInBetween[i];

							//(3) is already there
							break;
						}
						else if (AB_cycles[selectedAB_cycles[i]][j].first.second == result->chromR[route_id][k])
						{
							// we flip the edge ordering and repeat the iteration
							std::swap(AB_cycles[selectedAB_cycles[i]][j].first.second, AB_cycles[selectedAB_cycles[i]][j].first.first);
							k--;
						}
						else
						{
							posChromR++;
						}
					}
				}
				else
				{
					// Dealing with endpoints from different routes

					// Example: Consider v (route 1) and w (route 2)
					// route_first:  (1) - v - (2); route_second:  (3) - w - (4)
					// Resulting: route_first: (1) - v - w - (4); route_second: (3) - (2)

					int route_id_first = vertexRoute[AB_cycles[selectedAB_cycles[i]][j].first.first];
					int pos_id_first = 0;
					for (pos_id_first = 0; pos_id_first < result->chromR[route_id_first].size(); pos_id_first++)
					{
						if (result->chromR[route_id_first][pos_id_first] == AB_cycles[selectedAB_cycles[i]][j].first.first)
							break;
					}

					// We store (2) for later usage
					std::vector<int> chunk2_route_first = std::vector<int>(result->chromR[route_id_first].size() - pos_id_first - 1);
					for (size_t k = pos_id_first + 1, pos = 0; k < result->chromR[route_id_first].size(); k++, pos++)
						chunk2_route_first[pos] = result->chromR[route_id_first][k];

					int route_id_second = vertexRoute[AB_cycles[selectedAB_cycles[i]][j].first.second];
					int pos_id_second = 0;
					for (pos_id_second = 0; pos_id_second < result->chromR[route_id_second].size(); pos_id_second++)
					{
						if (result->chromR[route_id_second][pos_id_second] == AB_cycles[selectedAB_cycles[i]][j].first.second)
							break;
					}
					size_t initial_size_route_first = result->chromR[route_id_first].size();
					size_t initial_size_route_second = result->chromR[route_id_second].size();

					// Applying the new size for route_first
					int newSize_route_first = pos_id_first + 1 + 1 + result->chromR[route_id_second].size() - pos_id_second - 1;
					result->chromR[route_id_first].resize(newSize_route_first);

					//Appending w and (4)
					for (size_t k = pos_id_second, iterator = 1; k < result->chromR[route_id_second].size(); k++, iterator++)
						result->chromR[route_id_first][iterator + pos_id_first] = result->chromR[route_id_second][k];

					// Applying the new size for route_second
					result->chromR[route_id_second].resize(initial_size_route_first + initial_size_route_second - result->chromR[route_id_first].size());

					// Including chunk (2) at (3)
					for (size_t k = 0; k < chunk2_route_first.size(); k++)
						result->chromR[route_id_second][pos_id_second + k] = chunk2_route_first[k];

					// Update route id for all involved vertices
					for (size_t k = 0; k < result->chromR[route_id_first].size(); k++)
						vertexRoute[result->chromR[route_id_first][k]] = route_id_first;
					for (size_t k = 0; k < result->chromR[route_id_second].size(); k++)
						vertexRoute[result->chromR[route_id_second][k]] = route_id_second;
				}
			}
		}
	}

	bool useSplit = true;
	if (useSplit)
	{
		// Copy all routes to big tour
		int posChromT = 0;
		for (size_t i = 0; i < result->chromR.size(); i++)
		{
			for (size_t j = 0; j < result->chromR[i].size(); j++)
				result->chromT[posChromT++] = result->chromR[i][j];
		}

		split->generalSplit(result, parentA->myCostSol.nbRoutes);
	}
	else
		result->evaluateCompleteCost();
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

	GAB_A = new short int *[this->params->nbClients + 1];
	GAB_B = new short int *[this->params->nbClients + 1];
	for (int i = 0; i <= this->params->nbClients; i++)
	{
		GAB_A[i] = new short int[this->params->nbClients + 1];
		GAB_B[i] = new short int[this->params->nbClients + 1];
	}

	/* Initializing matrices */
	for (int i = 0; i <= this->params->nbClients; i++)
	{
		for (int j = i + 1; j <= this->params->nbClients; j++)
		{
			GAB_A[i][j] = GAB_A[j][i] = 0;
			GAB_B[i][j] = GAB_B[j][i] = 0;
		}
	}
}

Genetic::~Genetic(void)
{
	delete offspring;

	for (int i = 0; i <= this->params->nbClients; i++)
	{
		delete[] GAB_A[i];
		delete[] GAB_B[i];
	}
	delete[] GAB_A;
	delete[] GAB_B;
}
