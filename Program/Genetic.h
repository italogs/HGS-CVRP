/*MIT License

Copyright(c) 2020 Thibaut Vidal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef GENETIC_H
#define GENETIC_H

#include "Population.h"
#include "Individual.h"
class Genetic
{
private:

	Params * params;				// Problem parameters
	Split * split;					// Split algorithm
	Population * population;		// Population
	LocalSearch * localSearch;		// Local Search structure
	Mining * mining;				// Mining structure
	Individual * offspring;			// First individual to be used as input for the crossover

	short int **GAB_A = NULL;				// EAX: ParentA edges
	short int **GAB_B = NULL;				// EAX: ParentB edges

	std::map<std::pair<int,int>,int> heat_map_stl;

	// OX Crossover
	void crossoverOX(Individual * result, const Individual * parent1, const Individual * parent2);
	void crossoverHeatmap(Individual * result, const Individual * parent1, const Individual * parent2);
	void crossoverEAX(Individual *result, const Individual *parentA, const Individual *parentB);
	void crossover_newOX(Individual * result, const Individual * parent1, const Individual * parent2);
	void crossover_PILS(Individual *result, const Individual *parent1, const Individual *parent2);

	void crossoverOXClosest(Individual *result, const Individual *parent1, const Individual *parent2);

public:

	double *costX = new double[1];
	char *xctypeX = new char[1];
	char **namesX = new char *[1];
	
	double *ubX = new double[1];
	double *lbX = new double[1];

    // Running the genetic algorithm until maxIterNonProd consecutive iterations or a time limit
    void run(int maxIterNonProd, unsigned long timeLimit) ;

	// Constructor
	Genetic(Params * params, Split * split, Population * population, LocalSearch * localSearch, Mining * mining);

	// Destructor
	~Genetic(void);
};

#endif
