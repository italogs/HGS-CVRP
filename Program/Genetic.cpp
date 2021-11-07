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

bool minHeapComparisonFn(const std::pair<std::pair<int, int>, double> &elem1, const std::pair<std::pair<int, int>, double> &elem2)
{
	return (elem1.second > elem2.second);
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
