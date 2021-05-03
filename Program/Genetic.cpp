#include "Genetic.h"
#include <queue>

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
		else if (params->crossoverType == 3)
		{
			crossover_newOX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		}
		else if (params->crossoverType == 4)
		{
			crossover_newOX(offspring, population->getBinaryTournament(), population->getBinaryTournament());
		}
		else if (params->crossoverType == 5)
		{
			crossoverOX_fixRoute(offspring, population->getBinaryTournament(), population->getBinaryTournament());
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

		total_time_local_search += (clock() - local_search_start);

		/* TRACKING THE NUMBER OF ITERATIONS SINCE LAST SOLUTION IMPROVEMENT */
		if (isNewBest)
			nbIterNonProd = 1;
		else
			nbIterNonProd++;

		if (params->crossoverType == 3 || params->crossoverType == 4)
		{
			// Erase the memory if 1k iterations were not productive
			if (nbIterNonProd % 1001 == 1000)
				heat_map_stl.clear();

			for (int i = 0; i < offspring->chromR.size(); i++)
			{
				if (offspring->chromR[i].size() == 0)
					continue;
				for (int j = 0; j < offspring->chromR[i].size() - 1; j++)
				{
					//We ignore the edge orientation
					int lowest = std::min(offspring->chromR[i][j], offspring->chromR[i][j + 1]);
					int highest = std::max(offspring->chromR[i][j], offspring->chromR[i][j + 1]);
					heat_map_stl[std::make_pair(lowest, highest)]++;
				}
			}

			if (params->crossoverType == 4 && timeLimit != INT_MAX && nbIterNonProd == maxIterNonProd)
			{
				// params->RoutePool.clear();
				for (auto indi : population->feasibleSubpopulation)
				{
					for (int i = 0; i < indi->chromR.size(); i++)
					{
						if (!indi->chromR[i].empty())
						{
							bool repeatedRoute = false;
							if (indi->chromR[i][0] > indi->chromR[i][indi->chromR[i].size() - 1])
								std::reverse(indi->chromR[i].begin(), indi->chromR[i].end());
							for (int j = 0; j < params->RoutePool.size(); j++)
							{
								if (params->RoutePool[j].second == indi->costChromR[i] &&
									params->RoutePool[j].first.size() == indi->chromR[i].size() &&
									params->RoutePool[j].first[0] == indi->chromR[i][0] &&
									params->RoutePool[j].first[params->RoutePool[j].first.size() - 1] == indi->chromR[i][indi->chromR[i].size() - 1] &&
									params->RoutePool[j].first[params->RoutePool[j].first.size() / 2] == indi->chromR[i][indi->chromR[i].size() / 2])
								{
									repeatedRoute = true;
									break;
								}
							}
							if (!repeatedRoute)
								params->RoutePool.push_back(std::make_pair(indi->chromR[i], indi->costChromR[i]));
						}
					}
				}
				petalAlgorithm(offspring, population);
				bool isNewBest = population->addIndividual(offspring, true);
				std::cout << "Petal Solution: " << offspring->myCostSol.penalizedCost << "; feasible: " << offspring->isFeasible << "; newBest: " << isNewBest << std::endl;
				if (isNewBest)
					nbIterNonProd = 1;
			}
		}
		else if (params->crossoverType == 5)
		{
			// Erase the memory if 1k iterations were not productive
			if (nbIterNonProd % 1001 == 1000)
				routePoolFrequency.clear();

			for (int i = 0; i < offspring->chromR.size(); i++)
			{
				if (!offspring->chromR[i].empty())
				{
					if (offspring->chromR[i].size() > 1 && offspring->chromR[i][0] > offspring->chromR[i][offspring->chromR[i].size() - 1])
						std::reverse(offspring->chromR[i].begin(), offspring->chromR[i].end());

					routePoolFrequency[offspring->chromR[i]]++;
				}
			}
		}

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

void Genetic::crossoverOX_fixRoute(Individual *result, const Individual *parent1, const Individual *parent2)
{
	static int equalParents = 0;

	if (parent1->myCostSol.penalizedCost == parent2->myCostSol.penalizedCost)
	{
		int attempts = 0;
		while (parent1->myCostSol.penalizedCost == parent2->myCostSol.penalizedCost)
			parent2 = population->getBinaryTournament();
	}

	std::vector<std::pair<std::vector<int>, int>> bestHeats;

	for (auto elem : routePoolFrequency)
	{
		std::vector<int> routeFrequency = elem.first;
		int elemFrequency = elem.second;

		if (bestHeats.size() <= params->nbClients * 0.1)
		{
			bestHeats.push_back(std::make_pair(routeFrequency, elemFrequency));
		}
		else
		{
			int lowestPos = 0;
			int lowestFrequency = bestHeats[lowestPos].second;

			for (int k = lowestPos + 1; k < bestHeats.size(); k++)
			{
				if (bestHeats[k].second < lowestFrequency)
				{
					bestHeats[k].second = lowestFrequency;
					lowestPos = k;
				}
			}

			if (elemFrequency > lowestFrequency)
			{
				bestHeats[lowestPos].first = routeFrequency;
				bestHeats[lowestPos].second = elemFrequency;
			}
		}
	}

	if (bestHeats.size() == 0)
	{
		crossoverOX(offspring, parent1, parent2);
		return;
	}

	int randomPos = std::rand() % bestHeats.size();
	std::vector<int> routeFrequency = bestHeats[randomPos].first;
	std::vector<bool> freqClient = std::vector<bool>(params->nbClients + 1, false);

	for (int i = 0; i < routeFrequency.size(); i++)
	{
		result->chromT[i] = routeFrequency[i];
		freqClient[result->chromT[i]] = true;
	}

	// Picking the beginning and end of the crossover zone
	int start = routeFrequency.size();
	int end = (std::rand() % (params->nbClients - start - 1)) + start + 1;
	// while (end == start)
	// 	end = (std::rand() % (params->nbClients - start - 1)) + start;

	// Copy in place the elements from start to end (possibly "wrapping around" the end of the array)
	int j = start;
	while (j < end + 1)
	{
		int elem = parent1->chromT[j];
		if (!freqClient[elem])
		{
			result->chromT[start] = elem;
			freqClient[elem] = true;
			start++;
		}
		j++;
	}

	// Fill the remaining elements in the order given by the second parent
	for (int i = 1; i <= params->nbClients; i++)
	{
		int temp = parent2->chromT[(end + i) % params->nbClients];
		if (freqClient[temp] == false)
		{
			result->chromT[start % params->nbClients] = temp;
			start++;
			freqClient[temp] = true;
		}
	}

	// Completing the individual with the Split algorithm
	split->generalSplit(result, parent1->myCostSol.nbRoutes);
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

	int bestI = -1, bestJ = -1;
	double bestHeatIJ = 0.0;
	std::vector<std::pair<std::pair<int, int>, double>> bestHeats;
	for (auto elem : heat_map_stl)
	{
		bestHeatIJ = elem.second;
		bestI = elem.first.first;
		bestJ = elem.first.second;

		if (bestHeatIJ <= 1)
			continue;

		//We ignore edges that are present in
		if (parent1->predecessors[bestI] == bestJ || parent1->successors[bestI] == bestJ)
			continue;
		else if (parent2->predecessors[bestI] == bestJ || parent2->successors[bestI] == bestJ)
			continue;

		//Once the heap is not full, we keep inserting in it
		if (bestHeats.size() < (int)(params->nbClients * 0.05))
		{
			bestHeats.push_back(std::make_pair(std::make_pair(bestI, bestJ), bestHeatIJ));
			//if the full size is reached, we construct the min-heap
			if (bestHeats.size() == (int)(params->nbClients * 0.05))
				std::make_heap(bestHeats.begin(), bestHeats.end(), minHeapComparisonFn);
		}
		else
		{
			//if the current element is more frequent than the less frequent element of the heap, which is the root
			if (bestHeatIJ > (bestHeats.front()).second)
			{
				//We remove the root (minimum element)
				std::pop_heap(bestHeats.begin(), bestHeats.end(), minHeapComparisonFn);
				bestHeats.pop_back();

				//We insert the current element appropriately into the heap (i.e., heapify) only using push_heap
				bestHeats.push_back(std::make_pair(std::make_pair(bestI, bestJ), bestHeatIJ));
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
	int randomPos = std::rand() % bestHeats.size();
	bestI = bestHeats[randomPos].first.first;
	bestJ = bestHeats[randomPos].first.second;

	std::vector<int> segment = std::vector<int>(3);
	bool completeSegment = false;
	for (int i = 0; i < bestHeats.size(); i++)
	{
		if (i != randomPos)
		{
			if (bestHeats[i].first.first == bestI)
			{
				completeSegment = true;
				segment[0] = bestHeats[i].first.second;
				segment[1] = bestI;
				segment[2] = bestJ;
				break;
			}
			else if (bestHeats[i].first.second == bestI)
			{
				completeSegment = true;
				segment[0] = bestHeats[i].first.first;
				segment[1] = bestI;
				segment[2] = bestJ;
				break;
			}
			else if (bestHeats[i].first.first == bestJ)
			{
				completeSegment = true;
				segment[0] = bestHeats[i].first.second;
				segment[1] = bestJ;
				segment[2] = bestI;
				break;
			}
			else if (bestHeats[i].first.second == bestJ)
			{
				completeSegment = true;
				segment[0] = bestHeats[i].first.first;
				segment[1] = bestJ;
				segment[2] = bestI;
				break;
			}
		}
	}

	int start;
	int j;
	int end;
	// Copy the first part of parent1 into offspring, including bestI (or bestJ)
	std::vector<bool> freqClient = std::vector<bool>(params->nbClients + 1, false);
	if (completeSegment)
	{
		for (start = 0; start < parent1->chromT.size(); start++)
		{
			result->chromT[start] = parent1->chromT[start];
			freqClient[parent1->chromT[start]] = true;

			if (result->chromT[start] == segment[0])
			{
				result->chromT[++start] = segment[1];
				freqClient[result->chromT[start]] = true;
				result->chromT[++start] = segment[2];
				freqClient[result->chromT[start]] = true;
				break;
			}
			else if (result->chromT[start] == segment[1])
			{
				freqClient[result->chromT[start]] = false;
				result->chromT[start] = segment[0];
				freqClient[result->chromT[start]] = true;
				result->chromT[++start] = segment[1];
				freqClient[result->chromT[start]] = true;
				result->chromT[++start] = segment[2];
				freqClient[result->chromT[start]] = true;
				break;
			}
			else if (result->chromT[start] == segment[2])
			{
				result->chromT[++start] = segment[1];
				freqClient[result->chromT[start]] = true;
				result->chromT[++start] = segment[0];
				freqClient[result->chromT[start]] = true;
				std::swap(segment[0], segment[2]);
				break;
			}
		}
		j = start + 1;
		for (end = 0; end < parent1->chromT.size(); end++)
		{
			if (parent2->chromT[end] == segment[2])
				break;
		}
	}
	else
	{
		for (start = 0; start < parent1->chromT.size(); start++)
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
		for (end = 0; end < parent1->chromT.size(); end++)
		{
			if (parent2->chromT[end] == bestJ)
				break;
		}
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

void Genetic::petalAlgorithm(Individual *result, Population *pop)
{
	CPXENVptr env = NULL;
	CPXLPptr lp = NULL;
	int status = 0;
	env = CPXopenCPLEX(&status);						 // Open CPLEX environment
	CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_OFF); // Switching OFF the display
	// CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_ON);				  // Switching OFF the display
	CPXsetintparam(env, CPXPARAM_Read_DataCheck, CPX_DATACHECK_WARN); // Print warnings
	CPXsetintparam(env, CPX_PARAM_MIPINTERVAL, 10000);				  // shows the log after every 10000 nodes
	CPXsetintparam(env, CPX_PARAM_THREADS, 1);						  // number of threads
	CPXsetdblparam(env, CPX_PARAM_TILIM, 60);
	CPXsetintparam(env, CPX_PARAM_PRELINEAR, 0);
	std::string str_problem_name = "setpartitioning_petal";
	lp = CPXcreateprob(env, &status, str_problem_name.c_str()); // Create LP problem as a container
	bool isAbort = false;

	std::vector<std::vector<int>> incidenceVertex = std::vector<std::vector<int>>(params->nbClients + 1);

	for (int i = 0; i < params->RoutePool.size(); i++)
	{
		for (int j = 0; j < params->RoutePool[i].first.size(); j++)
		{
			if ((params->RoutePool[i].first)[j] > params->nbClients)
			{
				std::cout << "eppaaa";
				exit(0);
			}
			incidenceVertex[(params->RoutePool[i].first)[j]].push_back(i);
		}
	}

	int longestIncidence = 0;
	for (int i = 0; i < params->RoutePool.size(); i++)
	{
		if (params->RoutePool[i].first.empty())
		{
			std::cout << "epaa" << std::endl;
			exit(0);
		}
		xctypeX[0] = 'B';
		sprintf(namesX[0], "_X(%d)", i);
		costX[0] = params->RoutePool[i].second;
		ubX[0] = 1.0;
		lbX[0] = 0.0;
		status = CPXnewcols(env, lp, 1, costX, lbX, ubX, xctypeX, namesX);

		if (params->RoutePool[i].first.size() > longestIncidence)
			longestIncidence = params->RoutePool[i].first.size();
	}
	for (int i = 0; i < params->tempRoutePool.size(); i++)
	{
		if (params->tempRoutePool[i].first.empty())
		{
			std::cout << "epaa" << std::endl;
			exit(0);
		}
		xctypeX[0] = 'B';
		sprintf(namesX[0], "_X(%d)", i);
		costX[0] = params->tempRoutePool[i].second;
		ubX[0] = 1.0;
		lbX[0] = 0.0;
		status = CPXnewcols(env, lp, 1, costX, lbX, ubX, xctypeX, namesX);

		if (params->tempRoutePool[i].first.size() > longestIncidence)
			longestIncidence = params->tempRoutePool[i].first.size();
	}

	double rhs[] = {1.0};
	char sense[] = {'E'};
	int rmatbeg[] = {0};

	for (int i = 1; i < incidenceVertex.size(); i++)
	{
		if (incidenceVertex[i].empty())
		{
			std::cout << i << std::endl;
			isAbort = true;
			std::cout << "Abortedd" << std::endl;
			break;
		}
		int *rmatind = new int[incidenceVertex[i].size()];
		double *rmatval = new double[incidenceVertex[i].size()];

		for (int j = 0; j < incidenceVertex[i].size(); j++)
		{
			rmatind[j] = incidenceVertex[i][j];
			rmatval[j] = 1.0;
		}
		status = CPXaddrows(env, lp, 0, 1, incidenceVertex[i].size(), rhs, sense, rmatbeg, rmatind, rmatval, NULL, NULL);
		delete[] rmatind;
		delete[] rmatval;
	}
	int nbRoutes = 0;
	if (!isAbort)
	{
		status = CPXmipopt(env, lp); // Solving the model
		if (status)
			std::cout << "Primal-SVM. Failed to solve problem. Status code: " << status << std::endl;

		double *solution_set = new double[(int)(params->RoutePool.size() + params->tempRoutePool.size())];
		double solValue = 0.0;
		int solStatus = 0;
		int returnStatus = CPXsolution(env, lp, &solStatus, &solValue, solution_set, NULL, NULL, NULL);
		if (returnStatus)
		{
			isAbort = true;
		}
		else if (solStatus == 103)
		{
			std::cout << "infeasible 103" << std::endl;
			isAbort = true;
		}
		else if (solStatus == 107)
		{
			std::cout << "infeasible 107" << std::endl;
			isAbort = true;
		}
		else if (solStatus == 108)
		{
			std::cout << "infeasible 108" << std::endl;
			isAbort = true;
		}
		else
		{
			int posChromT = 0;
			std::vector<bool> freqClient = std::vector<bool>(params->nbClients + 1, false);
			int solutionPos = 0;
			for (int i = 0; i < params->RoutePool.size(); i++, solutionPos++)
			{
				if (solution_set[solutionPos] < 0.0001)
				{
					continue;
				}
				for (int j = 0; j < params->RoutePool[i].first.size(); j++)
				{
					if (!freqClient[(params->RoutePool[i].first)[j]])
					{
						result->chromT[posChromT++] = (params->RoutePool[i].first)[j];
						freqClient[(params->RoutePool[i].first)[j]] = true;
					}
				}
				nbRoutes++;
			}
			for (int i = 0; i < params->tempRoutePool.size(); i++, solutionPos++)
			{
				if (solution_set[solutionPos] < 0.0001)
				{
					continue;
				}
				for (int j = 0; j < params->tempRoutePool[i].first.size(); j++)
				{
					if (!freqClient[(params->tempRoutePool[i].first)[j]])
					{
						result->chromT[posChromT++] = (params->tempRoutePool[i].first)[j];
						freqClient[(params->tempRoutePool[i].first)[j]] = true;
					}
				}
				nbRoutes++;
			}

			for (int i = 1; i < freqClient.size(); i++)
			{
				if (!freqClient[i])
				{
					result->chromT[posChromT++] = i;
				}
			}
		}

		delete[] solution_set;
	}
	if (!isAbort)
		split->generalSplit(result, nbRoutes);
	else
		crossoverOX(result, pop->getBinaryTournament(), pop->getBinaryTournament());

	status = CPXfreeprob(env, &lp);
	status = CPXcloseCPLEX(&env);
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

	heat_map = new double *[this->params->nbClients + 1];
	for (int i = 0; i <= this->params->nbClients; i++)
	{
		heat_map[i] = new double[this->params->nbClients + 1];
	}

	for (int i = 0; i <= this->params->nbClients; i++)
	{
		for (int j = i + 1; j <= this->params->nbClients; j++)
		{
			heat_map[i][j] = heat_map[j][i] = 0.0;
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

	for (int i = 0; i <= this->params->nbClients; i++)
		delete[] heat_map[i];
	delete[] heat_map;

	delete[] costX;
	delete[] xctypeX;

	delete[] ubX;
	delete[] lbX;

	delete[] namesX[0];
	delete[] namesX;
}
