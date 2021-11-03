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
		{
			crossoverOX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		}
		else if (params->crossoverType == 2)
		{
			crossoverEAX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		}
		// type 3: two most-occurring adjacent edges are used
		// type 4: it's a type 3 of three edges
		else if (params->crossoverType == 3 || params->crossoverType == 4 || params->crossoverType == 7 || params->crossoverType == 8)
		{
			crossover_newOX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		}
		// type 5 and 6: PILS-like crossover
		else if (params->crossoverType == 5 || params->crossoverType == 6)
		{
			if (params->crossoverType == 5 || (std::rand() % 2 == 0))
				crossover_PILS(offspring, population->getBinaryTournament(), population->getBinaryTournament());
			else
				crossoverOX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		}
		else if (params->crossoverType == 9)
		{
			crossoverHeatmap(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		}
		else if (params->crossoverType == 10)
		{
			crossoverOXClosest(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		}

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

		// MINING SEQUENCE INFORMATION ON A PERCENTAGE OF THE LOCAL MINIMA
		total_time_local_search += (clock() - local_search_start);

		/* TRACKING THE NUMBER OF ITERATIONS SINCE LAST SOLUTION IMPROVEMENT */
		if (isNewBest)
			nbIterNonProd = 1;
		else
			nbIterNonProd++;

		if (params->crossoverType == 3 || params->crossoverType == 4 || params->crossoverType == 7 || params->crossoverType == 8)
		{
			// Erase the memory each 1k iterations that were not productive
			if (nbIterNonProd % 1001 == 1000)
				heat_map_stl.clear();

			for (int i = 0; i < (int)offspring->chromR.size(); i++)
			{
				for (int j = 0; j < (int)offspring->chromR[i].size() - 1; j++)
				{
					heat_map_stl[std::make_pair(std::min(offspring->chromR[i][j], offspring->chromR[i][j + 1]), std::max(offspring->chromR[i][j], offspring->chromR[i][j + 1]))]++;
					if (params->crossoverType == 7 || params->crossoverType == 8)
					{
						params->edgeFrequencyForCorrelatedVertices[offspring->chromR[i][j]][offspring->chromR[i][j + 1]]++;
						params->edgeFrequencyForCorrelatedVertices[offspring->chromR[i][j + 1]][offspring->chromR[i][j]]++;
					}
				}
			}
		}
		else if ((params->crossoverType == 5 || params->crossoverType == 6) && std::rand() % params->samplingRatioMining == 0)
		{
			mining->collect(offspring);
		}

		/* DIVERSIFICATION, PENALTY MANAGEMENT AND TRACES */
		if (nbIter % 100 == 0)
			population->managePenalties();

		if (nbIter % 501 == 500 && (params->crossoverType == 7 || params->crossoverType == 8))
		{
			std::vector<std::pair<double, int>> mostFrequentVertices;
			std::vector<bool> insertedVertices = std::vector<bool>(params->nbClients + 1, false);
			for (int i = 1; i <= params->nbClients; i++)
			{
				// For client i, we sort in descreasing order all clients on their appearing frequency in relation to i
				mostFrequentVertices.clear();
				for (int j = 1; j <= params->nbClients; j++)
				{
					if (params->edgeFrequencyForCorrelatedVertices[i][j] > 0 && i != j)
						mostFrequentVertices.push_back(std::pair<int, int>(-params->edgeFrequencyForCorrelatedVertices[i][j], j));
					insertedVertices[j] = false;
				}
				insertedVertices[0] = false;
				std::sort(mostFrequentVertices.begin(), mostFrequentVertices.end());

				// We reset the list of correlated vertices to insert the nbGranular most frequent vertices.
				params->correlatedVertices[i].clear();
				for (int j = 0; j < (int)mostFrequentVertices.size() && j < params->nbGranular; j++)
				{
					params->correlatedVertices[i].push_back(mostFrequentVertices[j].second);
					insertedVertices[mostFrequentVertices[j].second] = true;
				}

				// In case where the size of this list is lower than nbGranular, we complete it by increasing the most closest vertices
				// We use vector to quickly check if its incidence in O(1)
				if (params->crossoverType == 7 && (int)params->correlatedVertices[i].size() < params->nbGranular)
				{
					for (int j = 0; j < (int)params->closestVertices[i].size() && (int)params->correlatedVertices[i].size() < params->nbGranular; j++)
					{
						if (!insertedVertices[params->closestVertices[i][j]])
							params->correlatedVertices[i].push_back(params->closestVertices[i][j]);
					}
				}

				for (int j = 0; j <= params->nbClients; j++)
					params->edgeFrequencyForCorrelatedVertices[i][j] = 0;
			}
		}

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

bool minHeapComparisonFn(const std::pair<std::pair<int, int>, double> &elem1, const std::pair<std::pair<int, int>, double> &elem2)
{
	return (elem1.second > elem2.second);
}

void Genetic::crossover_newOX(Individual *result, const Individual *parent1, const Individual *parent2)
{
	//We avoid equal parents. At least we verify their cost
	if (parent1->myCostSol.penalizedCost == parent2->myCostSol.penalizedCost)
	{
		while (parent1->myCostSol.penalizedCost == parent2->myCostSol.penalizedCost)
			parent2 = population->getBinaryTournament();
	}

	int bestI = -1, bestJ = -1, bestHeatIJ = 0;
	std::vector<std::pair<std::pair<int, int>, int>> bestHeats;
	const int maximumHeapSize = params->nbClients * 0.05;
	for (const auto &elem : heat_map_stl)
	{
		if (elem.second < 2)
			continue;

		bestHeatIJ = elem.second;
		bestI = elem.first.first;
		bestJ = elem.first.second;

		//We ignore edges that are present in the parent
		if (parent1->predecessors[bestI] == bestJ || parent1->successors[bestI] == bestJ || parent2->predecessors[bestI] == bestJ || parent2->successors[bestI] == bestJ)
			continue;

		//Once the heap is not full, we keep inserting in it
		if ((int)bestHeats.size() < maximumHeapSize)
		{
			bestHeats.push_back(elem);
			//if the full size is reached, we construct the min-heap
			if ((int)bestHeats.size() == maximumHeapSize)
				std::make_heap(bestHeats.begin(), bestHeats.end(), minHeapComparisonFn);
		}
		else
		{
			//if the current element is more frequent than the less frequent element of the heap, which is the root
			if (bestHeatIJ > (bestHeats.front()).second)
			{
				// We pop the root (move root to the end of the vector),
				// then we insert a new element (replace last element by the new element),
				// and heapify (push_heap take care of it)
				std::pop_heap(bestHeats.begin(), bestHeats.end(), minHeapComparisonFn);
				bestHeats.back().first.first = elem.first.first;
				bestHeats.back().first.second = elem.first.second;
				bestHeats.back().second = elem.second;
				std::push_heap(bestHeats.begin(), bestHeats.end(), minHeapComparisonFn);
			}
		}
	}

	if (bestHeats.size() == 0)
	{
		crossoverOX(offspring, parent1, parent2);
		return;
	}

	// Get a random frequent edge
	int centerEdge = std::rand() % bestHeats.size();
	bestI = bestHeats[centerEdge].first.first;
	bestJ = bestHeats[centerEdge].first.second;

	// We search for another edge that connects to (bestI, bestJ).
	// If this is the case, completeSegment is true and we deal with some exceptional cases
	std::vector<int> segment = std::vector<int>(3);
	bool completeSegment = false;
	int adjacentEdge = 0;
	for (adjacentEdge = 0; adjacentEdge < (int)bestHeats.size(); adjacentEdge++)
	{
		if (adjacentEdge != centerEdge)
		{
			if (bestHeats[adjacentEdge].first.first == bestI)
			{
				completeSegment = true;
				segment[0] = bestHeats[adjacentEdge].first.second;
				segment[1] = bestI;
				segment[2] = bestJ;
				break;
			}
			else if (bestHeats[adjacentEdge].first.second == bestI)
			{
				completeSegment = true;
				segment[0] = bestHeats[adjacentEdge].first.first;
				segment[1] = bestI;
				segment[2] = bestJ;
				break;
			}
			else if (bestHeats[adjacentEdge].first.first == bestJ)
			{
				completeSegment = true;
				segment[0] = bestHeats[adjacentEdge].first.second;
				segment[1] = bestJ;
				segment[2] = bestI;
				break;
			}
			else if (bestHeats[adjacentEdge].first.second == bestJ)
			{
				completeSegment = true;
				segment[0] = bestHeats[adjacentEdge].first.first;
				segment[1] = bestJ;
				segment[2] = bestI;
				break;
			}
		}
	}

	// If type 4, 7, and 8, we add another adjacent edge to segment
	if (params->crossoverType == 4 || params->crossoverType == 7 || params->crossoverType == 8)
	{
		for (int adjacentEdge2 = 0; adjacentEdge2 < (int)bestHeats.size(); adjacentEdge2++)
		{
			if (adjacentEdge2 != centerEdge && adjacentEdge2 != adjacentEdge)
			{
				if (bestHeats[adjacentEdge2].first.first == segment[0])
				{
					if (bestHeats[adjacentEdge2].first.second != segment[1] && bestHeats[adjacentEdge2].first.second != segment[2])
					{
						segment.insert(segment.begin(), bestHeats[adjacentEdge2].first.second);
						break;
					}
				}
				else if (bestHeats[adjacentEdge2].first.second == segment[0])
				{
					if (bestHeats[adjacentEdge2].first.first != segment[1] && bestHeats[adjacentEdge2].first.first != segment[2])
					{
						segment.insert(segment.begin(), bestHeats[adjacentEdge2].first.first);
						break;
					}
				}
				else if (bestHeats[adjacentEdge2].first.first == segment[2])
				{
					if (bestHeats[adjacentEdge2].first.second != segment[0] && bestHeats[adjacentEdge2].first.second != segment[1])
					{
						segment.push_back(bestHeats[adjacentEdge2].first.second);
						break;
					}
				}
				else if (bestHeats[adjacentEdge2].first.second == segment[2])
				{
					if (bestHeats[adjacentEdge2].first.first != segment[0] && bestHeats[adjacentEdge2].first.first != segment[1])
					{
						segment.push_back(bestHeats[adjacentEdge2].first.first);
						break;
					}
				}
			}
		}
	}

	int start, end, j;
	// Copy the first part of parent1 into offspring, including bestI (or bestJ)
	std::vector<bool> freqClient = std::vector<bool>(params->nbClients + 1, false);
	if (completeSegment)
	{
		for (start = 0; start < params->nbClients; start++)
		{
			result->chromT[start] = parent1->chromT[start];
			freqClient[parent1->chromT[start]] = true;

			if (result->chromT[start] == segment[0] || result->chromT[start] == segment[1])
			{
				result->chromT[start] = segment[0];
				freqClient[result->chromT[start]] = true;
				result->chromT[++start] = segment[1];
				freqClient[result->chromT[start]] = true;
				result->chromT[++start] = segment[2];
				freqClient[result->chromT[start]] = true;
				if (segment.size() == 4)
				{
					result->chromT[++start] = segment[3];
					freqClient[result->chromT[start]] = true;
				}
				break;
			}
			else if (result->chromT[start] == segment[2] || (segment.size() == 4 && result->chromT[start] == segment[3]))
			{
				if (segment.size() == 4)
				{
					result->chromT[start] = segment[3];
					freqClient[result->chromT[start]] = true;

					result->chromT[++start] = segment[2];
					freqClient[result->chromT[start]] = true;
				}
				result->chromT[++start] = segment[1];
				freqClient[result->chromT[start]] = true;
				result->chromT[++start] = segment[0];
				freqClient[result->chromT[start]] = true;
				if (segment.size() == 4)
					std::swap(segment[0], segment[3]);
				else
					std::swap(segment[0], segment[2]);
				break;
			}
		}
		j = start + 1;
		for (end = 0; end < params->nbClients; end++)
		{
			if (parent2->chromT[end] == segment.back())
				break;
		}
	}
	else
	{
		for (start = 0; start < params->nbClients; start++)
		{
			result->chromT[start] = parent1->chromT[start];
			freqClient[parent1->chromT[start]] = true;
			if (parent1->chromT[start] == bestI || parent1->chromT[start] == bestJ)
				break;
		}

		if (parent1->chromT[start] == bestJ)
			std::swap(bestI, bestJ);

		//Then include bestJ (or bestI). For simplification, bestJ will always be the second element of the selected edge
		result->chromT[start + 1] = bestJ;
		freqClient[bestJ] = true;
		j = start + 2;
		for (end = 0; end < params->nbClients; end++)
		{
			if (parent2->chromT[end] == bestJ)
				break;
		}
	}

	// Fill the remaining elements in the order given by the second parent
	for (int i = 1; i <= params->nbClients; i++)
	{
		int temp = parent2->chromT[(end + i) % params->nbClients];
		if (!freqClient[temp])
		{
			result->chromT[j % params->nbClients] = temp;
			j++;
		}
	}

	// Completing the individual with the Split algorithm
	split->generalSplit(result, parent1->myCostSol.nbRoutes);
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
		int A_successor = std::max(parentA->successors[i], parentA->predecessors[i]);
		int A_predecessor = std::min(parentA->successors[i], parentA->predecessors[i]);
		int B_successor = std::max(parentB->successors[i], parentB->predecessors[i]);
		int B_predecessor = std::min(parentB->successors[i], parentB->predecessors[i]);

		// One-client routes are not allowed
		// Each if consists in verifying if the surrogate edge is present in the other parent.
		if (A_predecessor != A_successor)
		{
			if (A_predecessor != B_predecessor && A_predecessor != B_successor)
			{
				// edge_Apred
				GAB_A[A_predecessor][i] = 1;
				GAB_A[i][A_predecessor] = 1;
			}
			if (A_successor != B_predecessor && A_successor != B_successor)
			{
				// edge_Asucc
				GAB_A[i][A_successor] = 1;
				GAB_A[A_successor][i] = 1;
			}
		}
		if (B_predecessor != B_successor)
		{
			if (B_predecessor != A_predecessor && B_predecessor != A_successor)
			{
				// edge_Bpred
				GAB_B[B_predecessor][i] = 1;
				GAB_B[i][B_predecessor] = 1;
			}
			if (B_successor != A_predecessor && B_successor != A_successor)
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
	std::vector<std::pair<std::pair<int, int>, bool>> current_AB_cycle;
	bool hasEdges = true, edgeParent;
	int i, j, predecessor, successor;

	double bestCostInsertion;
	while (hasEdges)
	{
		hasEdges = false;
		predecessor = -1;
		//We find an initial node (predecessor) for an AB_cycle.
		edgeParent = true;
		for (i = 0; i <= this->params->nbClients; i++)
		{
			for (j = i + 1; j <= this->params->nbClients; j++)
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
		current_AB_cycle.clear();
		while (1)
		{
			//We select the successor node having the predecessor already set
			//This selection is based on the best arc to be selected (full greedy here)
			successor = -1;
			bestCostInsertion = 1e24;
			for (i = 0; i <= params->nbClients; i++)
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
			if ((short)i == selectedAB_cycles[0])
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
					for (pos_id_first = 0; pos_id_first < (int)result->chromR[route_id_first].size(); pos_id_first++)
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
					for (pos_id_second = 0; pos_id_second < (int)result->chromR[route_id_second].size(); pos_id_second++)
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

void Genetic::crossover_PILS(Individual *result, const Individual *parent1, const Individual *parent2)
{
	// Frequency table to track the customers which have been already inserted
	std::vector<bool> freqClient = std::vector<bool>(params->nbClients + 1, false);

	int patternSize = std::rand() % (params->maxSeqMining - params->minSeqMining + 1) + params->minSeqMining;
	int patternIndex = std::rand() % (mining->getNbSequences(patternSize));

	const std::vector<int> &sequence = mining->frequentSequence(patternSize, patternIndex); // Pattern which we want to inject

	int start;
	for (start = 0; start < params->nbClients; start++)
	{
		result->chromT[start] = parent1->chromT[start];
		freqClient[parent1->chromT[start]] = true;
		if (std::find(sequence.begin(), sequence.end(), parent1->chromT[start]) != sequence.end())
			break;
	}

	// std::cout << "sequence.size(): " << sequence.size() << std::endl;
	for (int i = 0; i < (int)sequence.size(); i++)
	{
		result->chromT[start] = sequence[i];
		freqClient[sequence[i]] = true;
		start++;
	}
	int j = start;
	int end = start;
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

void Genetic::crossoverHeatmap(Individual *result, const Individual *parent1, const Individual *parent2)
{
	// Frequency table to track the customers which have been already inserted
	std::vector<bool> freqClient = std::vector<bool>(params->nbClients + 1, false);

	// Picking the beginning and end of the crossover zone
	int start = (std::rand() % params->nbClients - 1) + 1;

	int best_I = parent1->chromT[start];
	int best_J = params->bestCustomerHeat[best_I];

	if (best_I == 0 || best_J == 0)
	{
		std::cout << "best_I " << best_I << "; best_J: " << best_J << std::endl;
	}

	int posChromT = 0;
	for (; posChromT <= params->nbClients; posChromT++)
	{
		result->chromT[posChromT] = parent1->chromT[posChromT];
		freqClient[result->chromT[posChromT]] = true;
		if (result->chromT[posChromT] == best_I)
		{
			posChromT++;
			result->chromT[posChromT] = best_J;
			freqClient[best_J] = true;
			break;
		}
		if (result->chromT[posChromT] == best_J)
		{
			posChromT++;
			result->chromT[posChromT] = best_I;
			freqClient[best_I] = true;

			std::swap(best_I, best_J);
			break;
		}
	}

	int posParent2 = 0;
	for (; posParent2 < parent2->chromT.size(); posParent2++)
	{
		if (parent2->chromT[posParent2] == best_J || parent2->chromT[posParent2] == best_I)
		{
			break;
		}
	}
	int j = posChromT + 1;
	// Fill the remaining elements in the order given by the second parent
	for (int i = 1; i <= params->nbClients; i++)
	{
		int temp = parent2->chromT[(posParent2 + i) % params->nbClients];
		if (freqClient[temp] == false)
		{
			result->chromT[j % params->nbClients] = temp;
			j++;
		}
	}

	// Completing the individual with the Split algorithm
	split->generalSplit(result, parent1->myCostSol.nbRoutes);
}


void Genetic::crossoverOXClosest(Individual *result, const Individual *parent1, const Individual *parent2)
{
	// Frequency table to track the customers which have been already inserted
	std::vector<bool> freqClient = std::vector<bool>(params->nbClients + 1, false);

	// Picking the beginning and end of the crossover zone
	int start = (std::rand() % params->nbClients - 1) + 1;

	int best_I = parent1->chromT[start];
	int best_J = params->bestCustomerClosest[best_I];

	if (best_I == 0 || best_J == 0)
	{
		std::cout << "best_I " << best_I << "; best_J: " << best_J << std::endl;
	}

	int posChromT = 0;
	for (; posChromT <= params->nbClients; posChromT++)
	{
		result->chromT[posChromT] = parent1->chromT[posChromT];
		freqClient[result->chromT[posChromT]] = true;
		if (result->chromT[posChromT] == best_I)
		{
			posChromT++;
			result->chromT[posChromT] = best_J;
			freqClient[best_J] = true;
			break;
		}
		if (result->chromT[posChromT] == best_J)
		{
			posChromT++;
			result->chromT[posChromT] = best_I;
			freqClient[best_I] = true;

			std::swap(best_I, best_J);
			break;
		}
	}

	int posParent2 = 0;
	for (; posParent2 < parent2->chromT.size(); posParent2++)
	{
		if (parent2->chromT[posParent2] == best_J || parent2->chromT[posParent2] == best_I)
		{
			break;
		}
	}
	int j = posChromT + 1;
	// Fill the remaining elements in the order given by the second parent
	for (int i = 1; i <= params->nbClients; i++)
	{
		int temp = parent2->chromT[(posParent2 + i) % params->nbClients];
		if (freqClient[temp] == false)
		{
			result->chromT[j % params->nbClients] = temp;
			j++;
		}
	}

	// Completing the individual with the Split algorithm
	split->generalSplit(result, parent1->myCostSol.nbRoutes+1);
}

Genetic::Genetic(Params *params, Split *split, Population *population, LocalSearch *localSearch, Mining *mining) : params(params), split(split), population(population), localSearch(localSearch), mining(mining)
{
	namesX[0] = new char[100];

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

	delete[] costX;
	delete[] xctypeX;

	delete[] ubX;
	delete[] lbX;

	delete[] namesX[0];
	delete[] namesX;
}
