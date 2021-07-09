
#ifndef HEATMAP_WRAPPER_H
#define HEATMAP_WRAPPER_H

#include <iostream>
#include <string>
#include <climits>
#include <vector>
#include <fstream>
#include "Params.h"
#include "Population.h"

class Population; 
class HeatmapWrapper
{    
    private:
        Params *params;
    public:
        std::vector<int> bigTourDPDP;
        std::string instanceName;
 
        HeatmapWrapper(Params *params) : params(params)
        {
            instanceName = params->pathToInstance.substr(params->pathToInstance.find_last_of("/\\") + 1);
        }

        void processBigTourDPDP(Population *&pop);

};
#endif
