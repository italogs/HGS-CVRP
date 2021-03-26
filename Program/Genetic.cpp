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

	//1st phase define edges from ParentA and B
	/* We have an incidence matrix to store the edges from the following resulting set: (E_A \union E_B) \ (E_A \intersection E_B ) */
	/* In other words, the edges that are not common in both parents.*/

	/* Initializing matrices */
	for (int i = 0; i <= this->params->nbClients; i++)
	{
		for (int j = i + 1; j <= this->params->nbClients; j++)
		{
			GAB_A[i][j] = GAB_A[j][i] = 0;
			GAB_B[i][j] = GAB_B[j][i] = 0;
		}
	}

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
	for (int i = 0; i < parentA->chromR.size(); i++)
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
	for (int i = 0; i < parentB->chromR.size(); i++)
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
	// std::vector<std::vector<std::pair<std::pair<int, int>, bool>>> AB_cycles;
	std::vector<std::vector<std::pair<int, int>>> AB_cycles;
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
		// old version (storing parent value)
		// std::vector<std::pair<std::pair<int, int>, bool>> current_AB_cycle;
		std::vector<std::pair<int, int>> current_AB_cycle;

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
			// current_AB_cycle.push_back(std::make_pair(std::make_pair(predecessor, successor), edgeParent));
			current_AB_cycle.push_back(std::make_pair(predecessor, successor));

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
			if (current_AB_cycle.size() > 1 && (successor == current_AB_cycle[0].first))
				break;

			// Otherwise, update predecessor and flip edgeParent
			edgeParent = !edgeParent;
			predecessor = successor;
		}

		//An AB_cycle is then formed
		AB_cycles.push_back(current_AB_cycle);
	}

	//Two strategies in genering the E_sets and merging them
	//Single strategy - select one AB_cycle at random
	//Block strategy - select one AB_cycle at random, then include all AB_cycles that contain at least one node of the first AB_cycle
	bool singleStrategy = false;
	bool enableWorkingVersion = true;

	if (!enableWorkingVersion)
	{
	}
	else
	{
		// We just include the largest AB_cycle as an independent route
		std::vector<bool> E_set_center_vertices = std::vector<bool>(params->nbClients + 1, false);
		int indexLargestCenter = 0;
		int largestCenterSize = 0;
		for (int i = 0; i < AB_cycles.size(); i++)
		{
			if (AB_cycles[i][0].first == 0 && AB_cycles[i].size() > largestCenterSize)
			{
				indexLargestCenter = i;
				largestCenterSize = AB_cycles[i].size();
			}
		}
		std::vector<std::pair<int, int>> E_set = AB_cycles[indexLargestCenter];
		for (int i = 0; i < E_set.size(); i++)
		{
			if (!E_set[i].second)
			{
				if (E_set[i].first != 0)
					E_set_center_vertices[E_set[i].first] = true;
				if (E_set[i].second != 0)
					E_set_center_vertices[E_set[i].second] = true;
			}
		}
		int lastPosChromT = 0;
		for (int i = 0; i < parentA->chromR.size(); i++)
		{
			if (parentA->chromR[i].size() == 0)
				continue;
			for (int j = 0; j < parentA->chromR[i].size(); j++)
			{
				if (!E_set_center_vertices[parentA->chromR[i][j]])
				{
					result->chromT[lastPosChromT++] = parentA->chromR[i][j];
				}
			}
		}

		//To avoid including a vertex twice (when there are subcycles)
		std::vector<bool> usedVertices = std::vector<bool>(params->nbClients + 1, false);
		for (int i = 0; i < E_set.size(); i++)
		{
			if (E_set[i].second != 0 && E_set_center_vertices[E_set[i].second] && !usedVertices[E_set[i].second])
			{
				usedVertices[E_set[i].second] = true;
				result->chromT[lastPosChromT++] = E_set[i].second;
			}
		}
	}

	split->generalSplit(result, parentA->myCostSol.nbRoutes);
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

	GAB_A = new int *[this->params->nbClients + 1];
	GAB_B = new int *[this->params->nbClients + 1];
	for (int i = 0; i <= this->params->nbClients; i++)
	{
		GAB_A[i] = new int[this->params->nbClients + 1];
		GAB_B[i] = new int[this->params->nbClients + 1];
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
