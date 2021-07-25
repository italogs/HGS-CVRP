
#ifndef HEATMAP_WRAPPER_H
#define HEATMAP_WRAPPER_H

#include <iostream>
#include <string>
#include <climits>
#include <vector>
#include <fstream>
#include "Params.h"
#include "Population.h"
#include "Individual.h"
class Population;
class Individual;
class HeatmapWrapper
{
private:
    Params *params;
    Split *split;             // Split algorithm
    LocalSearch *localSearch; // Local search structure
    Mining *mining;           // Mining structure
public:
    std::vector<int> bigTourDPDP;
    std::string instanceName;
    bool trainedModel;

    HeatmapWrapper(Params *params, Split *split, LocalSearch *localSearch, Mining *mining) : params(params), split(split), localSearch(localSearch), mining(mining)
    {
        instanceName = params->pathToInstance.substr(params->pathToInstance.find_last_of("/\\") + 1);
        this->trainedModel = false;
    }

    void processBigTourDPDP(Population *&pop);

    void inputSolutionFromDPDP(Population *&pop);

    void processDPDP_Val_Test(Population *&pop);

    void trainModel();
};
#endif
