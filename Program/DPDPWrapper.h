
#ifndef DPDP_WRAPPER_H
#define DPDP_WRAPPER_H

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
class DPDPWrapper
{
private:
    Params * params;
public:
    std::vector<int> bigTourDPDP;
    std::string instanceName;
    bool trainedModel;

    DPDPWrapper(Params *params) : params(params)
    {
        instanceName = params->pathToInstance.substr(params->pathToInstance.find_last_of("/\\") + 1);
        this->trainedModel = false;
    }
};
#endif
