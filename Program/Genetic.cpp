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
	/* each pair corresponds to the destiny vertex and which parent (true for parentA / false for parentB) */
	std::vector<std::vector<std::pair<int, bool>>> copy_GAB, GAB = std::vector<std::vector<std::pair<int, bool>>>(this->params->nbClients + 1);
	std::vector<std::vector<int>> AB_cycles;
	std::multiset<std::pair<std::set<int>, bool>> GAB_edges;

	//1st Phase - Generate auxiliary graph GAB
	for (int i = 1; i <= params->nbClients; i++)
	{
		// We have two edges (forward and backward) for each client.
		// We only need to check the edge endpoints
		int A_endpoint1 = parentA->successors[i];
		int A_endpoint2 = parentA->predecessors[i];

		if (i > A_endpoint1)
		{
			bool parentValue = true;
			std::set<int> edge;

			edge.insert(A_endpoint1);
			edge.insert(i);

			std::pair<std::set<int>, bool> key;
			key.first = edge;
			key.second = parentValue;
			GAB[i].push_back(std::make_pair(A_endpoint1, parentValue));
			GAB[A_endpoint1].push_back(std::make_pair(i, parentValue));
			GAB_edges.insert(std::make_pair(edge, parentValue));
		}

		if (i > A_endpoint2)
		{
			bool parentValue = true;
			std::set<int> edge;

			edge.insert(A_endpoint2);
			edge.insert(i);

			std::pair<std::set<int>, bool> key;
			key.first = edge;
			key.second = parentValue;
			GAB[i].push_back(std::make_pair(A_endpoint2, parentValue));
			GAB[A_endpoint2].push_back(std::make_pair(i, parentValue));
			GAB_edges.insert(std::make_pair(edge, parentValue));
		}

		//Making the checkings for parentB
		int B_endpoint1 = parentB->successors[i];
		int B_endpoint2 = parentB->predecessors[i];
		if (i > B_endpoint1)
		{
			bool parentValue = false;
			std::set<int> edge;

			edge.insert(B_endpoint1);
			edge.insert(i);

			std::pair<std::set<int>, bool> key;
			key.first = edge;
			key.second = parentValue;
			GAB[i].push_back(std::make_pair(B_endpoint1, parentValue));
			GAB[B_endpoint1].push_back(std::make_pair(i, parentValue));
			GAB_edges.insert(std::make_pair(edge, parentValue));
		}

		if (i > B_endpoint2)
		{
			bool parentValue = false;
			std::set<int> edge;

			edge.insert(B_endpoint2);
			edge.insert(i);

			std::pair<std::set<int>, bool> key;
			key.first = edge;
			key.second = parentValue;
			GAB[i].push_back(std::make_pair(B_endpoint2, parentValue));
			GAB[B_endpoint2].push_back(std::make_pair(i, parentValue));
			GAB_edges.insert(std::make_pair(edge, parentValue));
		}
	}

	// Copy of GAB
	copy_GAB = GAB;

	// 2nd phase - Create AB-cycles
	bool hasEdges = true;
	while (hasEdges)
	{
		hasEdges = false;
		int predecessor = -1;
		//We find an initial node: predecessor
		for (predecessor = 0; predecessor <= params->nbClients; predecessor++)
		{
			if (GAB[predecessor].size() > 0)
				break;
		}
		//We select its first adjacent node to obtain its origin (parentA or parentB)
		//The successor node will be anyway selected in the first iteration of the next loop
		bool edgeParent = GAB[predecessor][0].second;

		// We use this variable below
		bool edgeParentFirst = edgeParent;

		//To store the cycle's vertices
		std::vector<int> giant_cycle;
		giant_cycle.push_back(predecessor);

		//To access all used edges (avoid repetitions or back-and-forth situations)
		std::multiset<std::pair<std::set<int>, bool>> usedEdges;
		while (1)
		{
			//We select the successor node (note: the loop will have at most 4 elements, except depot)
			int successor = -1;
			int indexSuccessor = 0;
			for (int i = 0; i < GAB[predecessor].size(); i++)
			{
				if (GAB[predecessor][i].second == edgeParent)
				{
					std::set<int> edge;
					edge.insert(predecessor);
					edge.insert(GAB[predecessor][i].first);
					auto key = std::make_pair(edge, edgeParent);
					auto it = usedEdges.find(key);
					//We test two cases: (1) when the edge was not used in the cycle; (2) if we have two edges from same parent (i.e., one-client route)
					if (it == usedEdges.end() || GAB_edges.count(std::make_pair(edge, edgeParent)) > 1)
					{
						successor = GAB[predecessor][i].first;
						indexSuccessor = i;
						//We avoid the overlap of equal edges from different parents: 0(A)1(B)2(A)1(B)....
						//Commenting the conditional below allows this overlapping
						if (giant_cycle.size() > 1 && successor != giant_cycle[giant_cycle.size() - 2])
							break;
					}
				}
			}

			if (successor < 0) //No means that this conditional is true
			{
				std::cout << "não pode entrar aqui" << std::endl;
				exit(0);
			}

			//Include the successor node into the cycle and save the edge
			giant_cycle.push_back(successor);
			std::set<int> usedEdge;
			usedEdge.insert(predecessor);
			usedEdge.insert(successor);
			usedEdges.insert(std::make_pair(usedEdge, edgeParent));

			//All used edges are deleted from the list of adjacencies
			GAB[predecessor].erase(GAB[predecessor].begin() + indexSuccessor);
			for (int i = 0; i < GAB[successor].size(); i++)
			{
				if (GAB[successor][i].first == predecessor && GAB[successor][i].second == edgeParent)
					GAB[successor].erase(GAB[successor].begin() + i);
			}

			if (giant_cycle.size() > 1 && (giant_cycle[giant_cycle.size() - 1] == giant_cycle[0]))
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
		for (int i = 0; i <= params->nbClients; i++)
		{
			if (GAB[i].size() > 0)
			{
				hasEdges = true;
				break;
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
	bool blockStrategy = !singleBlock;
	int indexInitialE_set = std::rand() % AB_cycles.size();
	std::vector<int> E_set = AB_cycles[indexInitialE_set];
	std::vector<int> selectedE_sets;
	selectedE_sets.push_back(indexInitialE_set);
	if (blockStrategy)
	{
		//Have all vertices stored
		std::set<int> verticesInitialE_set;
		for (int i = 0; i < E_set.size(); i++)
		{
			if (E_set[i] != 0)
				verticesInitialE_set.insert(E_set[i]);
		}

		for (int i = 0; i < AB_cycles.size(); i++)
		{
			if (i == indexInitialE_set)
				continue;

			for (int j = 0; j < AB_cycles[i].size(); j++)
			{
				//If at least one vertice of an AB_cycle is common to the initial one, we save the index of the E_set
				if (verticesInitialE_set.find(AB_cycles[i][j]) != verticesInitialE_set.end())
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
		//We collect the first edgeParent of the E_set
		bool edgeParent = true;
		for (int i = 0; i < copy_GAB[E_set[0]].size(); i++)
		{
			if (copy_GAB[E_set[0]][i].first == E_set[1])
			{
				edgeParent = copy_GAB[E_set[0]][i].second;
				break;
			}
		}

		//From the E_set, we introduce all edges.
		//When the first occurrence of the edge (one endpoint is found),
		//The another endpoint is placed right after it and the remaining nodes are reversed and shifted to right
		//We don't "merge" subtours. We just place them on the bigtour and the split algorithm will take of making this job.
		for (int i = 0; i < E_set.size() - 1; i++)
		{
			//So far, we avoid edges that contain the depot
			if (E_set[i] == 0 || E_set[i + 1] == 0)
				continue;
			//We only regard edges that are from parentB, since the solution is based on parentA
			else if (!edgeParent)
			{
				for (int j = 0; j < result->chromT.size(); j++)
				{
					if (result->chromT[j] == E_set[i] || result->chromT[j] == E_set[i + 1])
					{
						//Node found
						//The next nodes are stored and shifted right after the node adjacent of it in the cycle
						std::vector<int> remainingNodes;
						if (result->chromT[j] == E_set[i])
						{
							for (int k = j + 1; k < result->chromT.size(); k++)
							{
								if (result->chromT[k] != E_set[i + 1])
									remainingNodes.push_back(result->chromT[k]);
							}
							result->chromT[j + 1] = E_set[i + 1];
						}
						else if (result->chromT[j] == E_set[i + 1])
						{
							for (int k = j + 1; k < result->chromT.size(); k++)
							{
								if (result->chromT[k] != E_set[i])
									remainingNodes.push_back(result->chromT[k]);
							}
							result->chromT[j + 1] = E_set[i];
						}
						std::reverse(remainingNodes.begin(), remainingNodes.end());
						for (int k = 0; k < remainingNodes.size(); k++)
							result->chromT[j + 2 + k] = remainingNodes[k];
						break;
					}
				}
			}
			edgeParent = !edgeParent;
		}
	}
	// Completing the individual with the Split algorithm
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
}

Genetic::~Genetic(void)
{
	delete offspring;
}
