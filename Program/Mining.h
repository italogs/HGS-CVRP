#ifndef MINING_H
#define MINING_H

#include "Individual.h"
#include <map>
#include <queue>

/**
 * Author: Thibaut Vidal & Italo Santana
 * 
 * PILS data structures and basic getters and setters are defined in this file.
 * Each MapAndHeap object stands for a Binary Heap that stores the k most frequent patterns.
 * Mining class has a vector of MapAndHeap, where each of its position regards a pattern size.
 * 
 */

struct MapElement
{
	int frequency = 1;
	int indexMinHeap = -1;
};
typedef std::map<std::vector<int>, MapElement>::iterator MyIterator;

class MapAndHeap
{

public:
	// Map to store all sequences found in the solutions (typically based on a red-black tree, complexity to insert = log(X) * sequenceSize in the worse case), where X represents the number of elements
	// An Unordered_Map could be a slightly better alternative to remove the log(X) factor, but subject to caution when defining a good hashing function
	// An efficient Trie (with a Map at each level) would be even better but require more coding effort... this is only useful if insertions become a bottleneck
	std::map<std::vector<int>, MapElement> myMap;

	int nbElementsHeap;					  // current number of elements in the heap
	int heapMaxCapacity;				  // capacity of the heap
	std::vector<MyIterator> elementsHeap; // elements of the heap (iterators towards the elements of the map)

	void heapify(int idx)
	{
		int left = 2 * idx + 1;
		int right = 2 * idx + 2;
		int smallest = idx;
		if (left < heapMaxCapacity && elementsHeap[left]->second.frequency < elementsHeap[smallest]->second.frequency)
			smallest = left;
		if (right < heapMaxCapacity && elementsHeap[right]->second.frequency < elementsHeap[smallest]->second.frequency)
			smallest = right;

		if (smallest != idx)
		{
			// Always update the associated index in the map data structure
			elementsHeap[smallest]->second.indexMinHeap = idx;
			elementsHeap[idx]->second.indexMinHeap = smallest;

			// Swap elements in the min heap
			MyIterator temp = elementsHeap[smallest];
			elementsHeap[smallest] = elementsHeap[idx];
			elementsHeap[idx] = temp;
			heapify(smallest);
		}
	}

	void insert(const std::vector<int> &mySequence)
	{
		/* MANAGEMENT OF THE MAP */
		auto result = myMap.insert(std::pair<std::vector<int>, MapElement>(mySequence, MapElement()));
		MyIterator it = result.first;			 // map.insert returns a pair whose first member is an iterator towards the data which has been inserted
		bool insertionPerformed = result.second; // map.insert returns a pair whose second member is a boolean valued to TRUE iff the insertion has been done
		if (!insertionPerformed)
			it->second.frequency++; // If the element already existed in the map, we increase its frequency

		/* MANAGEMENT OF THE HEAP */
		if (it->second.indexMinHeap != -1)
		{
			// Element is already in the heap but frequency has changed, restore min heap property (only if the structure is already organized as a heap)
			if (nbElementsHeap == heapMaxCapacity)
				heapify(it->second.indexMinHeap);
		}
		else if (nbElementsHeap < heapMaxCapacity)
		{
			// Element is not in the heap and there is space, add it
			elementsHeap[nbElementsHeap] = it;
			it->second.indexMinHeap = nbElementsHeap;
			nbElementsHeap++;

			// Once the full size is attained, we enforce heap property in the entire vector (only need to do that once)
			if (nbElementsHeap == heapMaxCapacity)
				for (int i = (heapMaxCapacity - 1) / 2; i >= 0; i--)
					heapify(i);
		}
		else if (it->second.frequency > elementsHeap[0]->second.frequency)
		{
			// Element is not in the heap but more frequent than the root, therefore it replaces the root element
			elementsHeap[0]->second.indexMinHeap = -1;
			elementsHeap[0] = it;
			it->second.indexMinHeap = 0;
			heapify(0);
		}
	}

	MapAndHeap(int heapMaxCapacity) : heapMaxCapacity(heapMaxCapacity)
	{
		elementsHeap = std::vector<MyIterator>(heapMaxCapacity);
		nbElementsHeap = 0;
	}
};

// This stores statistics on frequent sequences of visits found within the solutions
// Can return a limited number of such sequences, to be exploited in a local search.
class Mining
{

private:
	// Access to the problem parameters
	Params *params;

	// Each element of this structure is designed to store all "most-frequent" sequences of size k for k=2...params->maxSeqMining
	std::vector<MapAndHeap> myMapAndHeap;

public:
	// Collects all patterns of all desired size values from this solution
	void collect(Individual *indiv)
	{
		params->totalExtractions++;
		double tcpu1 = (double)clock() / (double)CLOCKS_PER_SEC;
		for (int sequenceSize = params->minSeqMining; sequenceSize <= params->maxSeqMining; sequenceSize++)
		{
			for (int r = 0; r < params->nbVehicles; r++) // Iterating on the routes of the current solution
			{
				std::vector<int> &myRoute = indiv->chromR[r];
				for (int indexEnd = sequenceSize; indexEnd <= (int)myRoute.size(); indexEnd++)
				{
					std::vector<int> mySequence(myRoute.begin() + indexEnd - sequenceSize, myRoute.begin() + indexEnd);
					if (mySequence[0] > mySequence[sequenceSize - 1]) //Avoids that two symmetric patterns are inserted
						std::reverse(mySequence.begin(), mySequence.end());

					myMapAndHeap[sequenceSize].insert(mySequence);
				}
			}
		}
		double tcpu2 = (double)clock() / (double)CLOCKS_PER_SEC;
		params->totalExtractionTimePILS += tcpu2 - tcpu1;
	}

	// Accessor to the i^th frequent sequence in the heap for this sequence size.
	// sequenceSize must be greater than 1
	const std::vector<int> &frequentSequence(int sequenceSize, int i)
	{
		// As a safety check, verify that the map and heap elements are indeed always pointing to each other
		MyIterator myIt = myMapAndHeap[sequenceSize].elementsHeap[i];
		if (myIt->second.indexMinHeap != i)
			throw std::string("ERROR linking in Pattern Mining structure");
		return myIt->first;
	}

	// Accessor to the number of sequences currently selected for a given sequence size
	int getNbSequences(int sequenceSize)
	{
		return myMapAndHeap[sequenceSize].nbElementsHeap;
	}

	// Constructor
	Mining(Params *params) : params(params)
	{
		myMapAndHeap = std::vector<MapAndHeap>(params->maxSeqMining + 1, MapAndHeap((int)(params->factorPatternMining * (double)params->nbClients)));
	}
};

#endif
