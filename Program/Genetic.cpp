#include "Genetic.h"

void Genetic::run(int maxIterNonProd, int timeLimit)
{
	int nbIterNonProd = 1;
	int nbIter;
	for (nbIter = 0; nbIterNonProd <= maxIterNonProd && clock() / CLOCKS_PER_SEC < timeLimit; nbIter++)
	{
		/* SELECTION AND CROSSOVER */
		if (params->crossoverType == 1)
			crossoverOX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		else if (params->crossoverType == 2)
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

	//1st phase
	/* We use the edges from the following resulting set: (E_A \union E_B) \ (E_A \intersection E_B ) */

	/* Initializing matrix (n+1)x(n+1) */
	int **GAB_A = new int *[this->params->nbClients + 1];
	int **GAB_B = new int *[this->params->nbClients + 1];
	int **GAB_union = new int *[this->params->nbClients + 1];
	int **GAB_intersection = new int *[this->params->nbClients + 1];
	int **GAB_difference = new int *[this->params->nbClients + 1];

	for (int i = 0; i <= this->params->nbClients; i++)
	{
		GAB_A[i] = new int[this->params->nbClients + 1];
		GAB_B[i] = new int[this->params->nbClients + 1];
		GAB_union[i] = new int[this->params->nbClients + 1];
		GAB_intersection[i] = new int[this->params->nbClients + 1];
		GAB_difference[i] = new int[this->params->nbClients + 1];
	}

	for (int i = 0; i <= this->params->nbClients; i++)
	{
		for (int j = i + 1; j <= this->params->nbClients; j++)
		{
			GAB_A[i][j] = GAB_A[j][i] = 0;
			GAB_B[i][j] = GAB_B[j][i] = 0;
			GAB_union[i][j] = GAB_union[j][i] = 0;
			GAB_intersection[i][j] = GAB_intersection[j][i] = 0;
			GAB_difference[i][j] = GAB_difference[j][i] = 0;
		}
	}

	/* Accouting the number of edges from each parent */
	for (int i = 1; i <= params->nbClients; i++)
	{
		int A_successor = parentA->successors[i];
		int A_predecessor = parentA->predecessors[i];
		int B_successor = parentB->successors[i];
		int B_predecessor = parentB->predecessors[i];

		std::set<int> edge_A1({A_predecessor, i});
		std::set<int> edge_A2({i, A_successor});

		std::set<int> edge_B1({B_predecessor, i});
		std::set<int> edge_B2({i, B_successor});

		//One-client route
		if (edge_A1 == edge_A2)
		{
			// edge_A1
			GAB_A[A_predecessor][i]++;
			GAB_A[i][A_predecessor]++;

			// edge_A2
			GAB_A[i][A_successor]++;
			GAB_A[A_successor][i]++;
		}
		else
		{
			// edge_A1
			GAB_A[A_predecessor][i] = 1;
			GAB_A[i][A_predecessor] = 1;
			// edge_A2
			GAB_A[i][A_successor] = 1;
			GAB_A[A_successor][i] = 1;
		}

		//One-client route
		if (edge_B1 == edge_B2)
		{
			// edge_B1
			GAB_B[B_predecessor][i]++;
			GAB_B[i][B_predecessor]++;

			// edge_B2
			GAB_B[i][B_successor]++;
			GAB_B[B_successor][i]++;
		}
		else
		{
			// edge_A1
			GAB_B[B_predecessor][i] = 1;
			GAB_B[i][B_predecessor] = 1;
			// edge_A2
			GAB_B[i][B_successor] = 1;
			GAB_B[B_successor][i] = 1;
		}
	}

	/* Accounting (E_A \union E_B) \ (E_A \intersect E_B ) */
	for (int i = 0; i <= params->nbClients; i++)
	{
		for (int j = i + 1; j <= params->nbClients; j++)
		{
			GAB_union[i][j] = (GAB_A[i][j] + GAB_B[i][j]);
			GAB_union[j][i] = GAB_union[i][j];
			GAB_intersection[i][j] = abs(GAB_A[i][j] - GAB_B[i][j]);
			GAB_intersection[j][i] = GAB_intersection[i][j];
			GAB_difference[i][j] = abs(GAB_union[i][j] - GAB_intersection[i][j]);
			GAB_difference[j][i] = GAB_difference[i][j];
		}
	}
	for (int i = 0; i <= params->nbClients; i++)
	{
		for (int j = i + 1; j <= params->nbClients; j++)
		{
			GAB_A[i][j] = 0;
			GAB_A[j][i] = 0;
			GAB_B[i][i] = 0;
			GAB_B[j][j] = 0;
		}
	}

	// /* For each existing arc in GAB_difference */
	for (int i = 0; i <= params->nbClients; i++)
	{
		for (int j = i + 1; j <= params->nbClients; j++)
		{
			if (GAB_difference[i][j] > 0)
			{
				//One client route in parentA
				if (parentA->successors[i] == 0 && parentA->predecessors[i] == 0)
					GAB_A[j][i] = GAB_A[i][j] = GAB_difference[i][j];

				//One client route in parentB
				else if (parentB->successors[i] == 0 && parentB->predecessors[i] == 0)
					GAB_A[j][i] = GAB_A[i][j] = GAB_difference[i][j];

				else
				{
					//ParentA has (i,j) and parentB doesn't
					if ((parentA->successors[i] == j || parentA->predecessors[i] == j) && (parentB->successors[i] != j && parentB->predecessors[i] != j))
						GAB_A[j][i] = GAB_A[i][j] = GAB_difference[i][j];

					//ParentA doesn't have (i,j) and parentB does
					else if ((parentA->successors[i] != j && parentA->predecessors[i] != j) && (parentB->successors[i] == j || parentB->predecessors[i] == j))
						GAB_B[j][i] = GAB_B[i][j] = GAB_difference[i][j];
				}
			}
		}
	}

	// 2nd phase - Create AB-cycles
	//AB_Cycles: each AB_Cycle is a sequence of [(i,j) and parent edge]
	//parent edge is a boolean to identify if this edge came from parentA(true) or parentB(false)
	std::vector<std::vector<std::pair<std::pair<int, int>, bool>>> AB_cycles;
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

		//Store the cycle's arcs and its edge parent
		//Arcs are saved as pair to maintain the its ordering
		std::vector<std::pair<std::pair<int, int>, bool>> giant_cycle;

		//To access all used edges (avoid repetitions or back-and-forth situations)
		std::multiset<std::pair<std::set<int>, bool>> usedEdges;
		std::set<int> usedVertices;
		usedVertices.insert(predecessor);

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
							// break;
						}
					}
				}
			}

			//No successor found in the first attempt
			if (successor < 0)
			{
				int i;
				for (i = 0; i < giant_cycle.size(); i++)
				{
					if (predecessor == giant_cycle[i].first.first)
						break;
				}
				//There's no subcycle. So we select an edge of the previous used edgeParent to continue.
				if (i == giant_cycle.size())
				{
					edgeParent = !edgeParent;
					for (int i = 0; i <= params->nbClients; i++)
					{
						//Checking if there're available edges
						if ((edgeParent && GAB_A[predecessor][i] > 0) || (!edgeParent && GAB_B[predecessor][i] > 0))
						{
							std::set<int> edge({predecessor, i});
							auto key = std::make_pair(edge, edgeParent);
							auto it = usedEdges.find(key);
							//We test two cases: (1) when the edge was not used in the cycle; (2) if we have two edges from same parent (i.e., one-client route)
							if (it == usedEdges.end() || (edgeParent && GAB_A[predecessor][i] > 0) || (!edgeParent && GAB_B[predecessor][i] > 0))
							{
								// if (params->timeCost[predecessor][i] < bestCostInsertion)
								{
									successor = i;
									bestCostInsertion = params->timeCost[predecessor][i];
									break;
								}
							}
						}
					}

					if (successor < 0)
					{
						std::cout << "Error. No successor found." << std::endl;
						exit(0);
					}
				}
				else // This may be removed later. Its purpose was to identify all non-subcycle edges.
				{
					std::cout << "This can be eliminated;" << std::endl;
					exit(0);
					for (int j = 0; j < i; j++)
					{
						if (giant_cycle[j].second)
						{
							GAB_A[giant_cycle[j].first.first][giant_cycle[j].first.second]++;
							GAB_A[giant_cycle[j].first.second][giant_cycle[j].first.first]++;
						}
						else
						{
							GAB_B[giant_cycle[j].first.first][giant_cycle[j].first.second]++;
							GAB_B[giant_cycle[j].first.second][giant_cycle[j].first.first]++;
						}
						giant_cycle.erase(giant_cycle.begin());
					}
					break;
				}
			}

			//Include the successor node into the cycle and save the edge
			std::set<int> usedEdge({predecessor, successor});
			usedEdges.insert(std::make_pair(usedEdge, edgeParent));
			giant_cycle.push_back(std::make_pair(std::make_pair(predecessor, successor), edgeParent));
			usedVertices.insert(successor);
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

	//A step to check if all edges have been used when we are not in the single block strategy
	if (!singleStrategy)
	{
		for (int i = 0; i <= this->params->nbClients; i++)
		{
			for (int j = i + 1; j <= this->params->nbClients; j++)
			{
				if (GAB_A[i][j] > 0 || GAB_B[i][j] > 0)
				{
					std::cout << "Error. There are available edges." << std::endl;
					exit(0);
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
	std::set<int> E_set_vertices;
	std::set<int> E_set_center_vertices;
	for (int i = 0; i < E_set.size(); i++)
	{
		if (!E_set[i].second && (E_set[i].first.first != 0 && E_set[i].first.second != 0))
		{
			E_set_center_vertices.insert(E_set[i].first.first);
			E_set_center_vertices.insert(E_set[i].first.second);
		}
	}

	if (!singleStrategy)
	{

		for (int i = 0; i < AB_cycles.size(); i++)
		{
			if (i == indexAB_center)
				continue;

			for (int j = 0; j < AB_cycles[i].size(); j++)
			{
				//If at least one vertice of an AB_cycle is common to the initial one, we save the index of the E_set
				if (!AB_cycles[i][j].second && E_set_center_vertices.find(AB_cycles[i][j].first.first) != E_set_center_vertices.end())
				{
					indexSelectedE_sets.push_back(i);
					break;
				}
			}
			if (indexSelectedE_sets[indexSelectedE_sets.size() - 1] == i)
			{
				for (int j = 0; j < AB_cycles[i].size(); j++)
				{
					if (!AB_cycles[i][j].second && (AB_cycles[i][j].first.first != 0 && AB_cycles[i][j].first.second != 0))
					{
						E_set_vertices.insert(AB_cycles[i][j].first.first);
						E_set_vertices.insert(AB_cycles[i][j].first.second);
					}
				}
			}
		}
	}

	std::vector<int> remainingNodes;
	remainingNodes.push_back(0);
	for (int i = 0; i < parentA->chromT.size(); i++)
	{
		if (E_set_vertices.find(parentA->chromT[i]) == E_set_vertices.end() && E_set_center_vertices.find(parentA->chromT[i]) == E_set_center_vertices.end())
			remainingNodes.push_back(parentA->chromT[i]);
	}
	remainingNodes.push_back(0);

	//We iterate over all selected E_sets
	for (int k = 0; k < indexSelectedE_sets.size(); k++)
	{
		E_set = AB_cycles[indexSelectedE_sets[k]];

		for (int i = 0; i < E_set.size(); i++)
		{
			//So far, we avoid edges that contain the depot
			if (E_set[i].first.first == 0 || E_set[i].first.second == 0)
				continue;

			if (!E_set[i].second)
			{
				int bestIndexInsertion = 1;
				double bestCostInsertion = 1e32;
				std::vector<int>::iterator it_edge_i = std::find(remainingNodes.begin(), remainingNodes.end(), E_set[i].first.first);
				std::vector<int>::iterator it_edge_j = std::find(remainingNodes.begin(), remainingNodes.end(), E_set[i].first.second);
				if (it_edge_i == remainingNodes.end() && it_edge_j == remainingNodes.end())
				{
					//We search for the best spot to insert the arc
					for (int j = 0; j < remainingNodes.size() - 1; j++)
					{
						if (params->timeCost[remainingNodes[j]][E_set[i].first.first] + params->timeCost[E_set[i].first.second][remainingNodes[j + 1]] < bestCostInsertion)
						{
							bestCostInsertion = params->timeCost[remainingNodes[j]][E_set[i].first.first] + params->timeCost[E_set[i].first.second][remainingNodes[j + 1]];
							bestIndexInsertion = j + 1;
						}
					}
					int myarray[2];
					{
						myarray[0] = E_set[i].first.first;
						myarray[1] = E_set[i].first.second;
					}
					remainingNodes.insert(remainingNodes.begin() + bestIndexInsertion, myarray, myarray + 2);
				}
				else if (it_edge_i != remainingNodes.end() && it_edge_j != remainingNodes.end())
				{
					int pos_i = it_edge_i - remainingNodes.begin();
					int pos_j = it_edge_j - remainingNodes.begin();
					if (pos_i < pos_j)
					{
						remainingNodes.erase(it_edge_j);
						int myarray[1] = {E_set[i].first.second};
						remainingNodes.insert(it_edge_i, myarray, myarray + 1);
					}
					else
					{
						remainingNodes.erase(it_edge_i);
						int myarray[1] = {E_set[i].first.first};
						remainingNodes.insert(it_edge_j, myarray, myarray + 1);
					}
				}
				else
				{
					if (it_edge_i == remainingNodes.end())
					{
						int myarray[] = {E_set[i].first.first};
						remainingNodes.insert(it_edge_j, myarray, myarray + 1);
					}
					else
					{
						int myarray[] = {E_set[i].first.second};
						remainingNodes.insert(it_edge_i, myarray, myarray + 1);
					}
				}
			}
		}
	}
	// Completing the individual with the Split algorithm
	for (int i = 0; i < result->chromT.size(); i++)
		result->chromT[i] = remainingNodes[i + 1];

	split->generalSplit(result, parentA->myCostSol.nbRoutes);

	for (int i = 0; i <= this->params->nbClients; i++)
	{
		delete[] GAB_A[i];
		delete[] GAB_B[i];
		delete[] GAB_union[i];
		delete[] GAB_intersection[i];
		delete[] GAB_difference[i];
	}
	delete[] GAB_A;
	delete[] GAB_B;
	delete[] GAB_union;
	delete[] GAB_intersection;
	delete[] GAB_difference;
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
