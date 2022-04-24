/*MIT License

Copyright(c) 2022 Thibaut Vidal

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

#ifndef HEATMAP_H
#define HEATMAP_H

#include <iostream>
#include <string>
#include <climits>
#include "Params.h"

class Heatmap
{
private:
    // This parameter refers to the output size of the network, which can be translated as the size of the instances used for training and testing.
    const int GNN_OUTPUT_SIZE = 100;
    Params *params;

public:
    bool static orderPairSecond(const std::pair<int, double> &a, const std::pair<int, double> &b)
    {
        return a.second < b.second;
    }

    // Reads the line of command and extracts possible options
    Heatmap(Params *params, unsigned long timeLimit) : params(params)
    {
        if (params->useHeatmapLS == 1 || params->crossoverType == 1)
        {
            // List of lists to temporary store all correlated vertices
            std::vector<std::set<int>> setCorrelatedVertices = std::vector<std::set<int>>(params->nbClients + 1);
            std::string instanceBaseName = params->pathToInstance.substr(params->pathToInstance.find_last_of("/\\") + 1);
            std::string heatmapName = instanceBaseName.substr(0, instanceBaseName.find_last_of("."));
            std::string heatmapFullPath = "DPDP/Heatmaps_for_HGS/" + heatmapName + "/" + heatmapName;

            // If the nb of customers is greater than GNN_OUTPUT_SIZE, we apply the scale-up technique
            if (params->nbClients > GNN_OUTPUT_SIZE)
            {
                params->bestCustomerHeat.push_back(0);
                // Each client has its own file (depot + GNN_OUTPUT_SIZE closest elements)
                for (int client_i = 1; client_i <= params->nbClients; client_i++)
                {
                    std::ifstream heatmapFile(heatmapFullPath + "_" + std::to_string(client_i) + ".hm");
                    if (heatmapFile.is_open())
                    {
                        double edge_heat = 0.0;
                        std::vector<std::pair<int, double>> heatList;
                        double bestHeatValue = -1.0;
                        int bestHeatCustomer = 1;

                        // Reading the first line of the heatmap (depot line)
                        for (int client_j = 0; client_j <= GNN_OUTPUT_SIZE; client_j++)
                            heatmapFile >> edge_heat;

                        // Reading the second line of the file, which is related to client_i
                        for (int client_j = 0; client_j <= GNN_OUTPUT_SIZE; client_j++)
                        {
                            heatmapFile >> edge_heat;
                            if (client_j > 0 && client_i != client_j)
                            {
                                // we entirely ignore 'heats' involving the depot
                                if (edge_heat > bestHeatValue)
                                {
                                    bestHeatValue = edge_heat;
                                    bestHeatCustomer = client_j;
                                }
                                // we also ignore 'heats' that points customer i to itself
                                heatList.push_back(std::make_pair(client_j, -edge_heat));
                            }
                        }

                        // Sort list of heats and including them into correlatedVertices
                        std::sort(heatList.begin(), heatList.end(), orderPairSecond);

                        for (int j = 0; j < heatList.size() && j < params->correlatedVertices[client_i].size() / 2; j++)
                        {
                            // If i is correlated with j, then j should be correlated with i
                            setCorrelatedVertices[client_i].insert(params->closestVertices[client_i][heatList[j].first]);
                            setCorrelatedVertices[params->closestVertices[client_i][heatList[j].first]].insert(client_i);
                        }
                        int posClosest = 0;
                        for (int j = 0; j < params->correlatedVertices[client_i].size() && setCorrelatedVertices[client_i].size() < params->correlatedVertices[client_i].size() && posClosest < params->nbClients; j++)
                        {
                            // If i is correlated with j, then j should be correlated with i
                            setCorrelatedVertices[client_i].insert(params->closestVertices[client_i][posClosest]);
                            setCorrelatedVertices[params->closestVertices[client_i][posClosest]].insert(client_i);
                            posClosest++;
                        }
                    }
                    else
                    {
                        std::cout << "Heatmap could not be found. Terminating program." << std::endl;
                        exit(0);
                    }
                }
            }
            else
            {
                std::ifstream heatmapFile(heatmapFullPath + ".hm");
                std::string content;
                if (heatmapFile.is_open())
                {
                    double edge_heat = 0.0;
                    // Reading the first line of the heatmap (depot line)
                    params->bestCustomerHeat.push_back(0);
                    for (int client_i = 0; client_i <= GNN_OUTPUT_SIZE; client_i++)
                        heatmapFile >> edge_heat;

                    for (int client_i = 1; client_i <= (int)GNN_OUTPUT_SIZE; client_i++)
                    {
                        std::vector<std::pair<int, double>> heatList;
                        double bestHeatValue = -1.0;
                        int bestHeatCustomer = 1;
                        // Reading the line of client_i
                        for (int client_j = 0; client_j <= GNN_OUTPUT_SIZE; client_j++)
                        {
                            heatmapFile >> edge_heat;
                            // we entirely ignore 'heats' involving the depot
                            if (client_j > 0 && client_i != client_j)
                            {
                                if (edge_heat > bestHeatValue)
                                {
                                    bestHeatValue = edge_heat;
                                    bestHeatCustomer = client_j;
                                }
                                // we also ignore 'heats' that points customer i to itself
                                heatList.push_back(std::make_pair(client_j, -edge_heat));
                            }
                        }

                        // Sort list of heats and including them into correlatedVertices
                        std::sort(heatList.begin(), heatList.end(), orderPairSecond);
                        for (int j = 0; j < heatList.size() && j < params->correlatedVertices[client_i].size() / 2; j++)
                        {
                            // If i is correlated with j, then j should be correlated with i
                            setCorrelatedVertices[client_i].insert(heatList[j].first);
                            setCorrelatedVertices[heatList[j].first].insert(client_i);
                        }
                        int posClosest = 0;
                        for (int j = 0; j < params->correlatedVertices[client_i].size() && setCorrelatedVertices[client_i].size() < (params->correlatedVertices[client_i].size()) && posClosest < params->nbClients; j++)
                        {
                            // If i is correlated with j, then j should be correlated with i
                            setCorrelatedVertices[client_i].insert(params->closestVertices[client_i][posClosest]);
                            setCorrelatedVertices[params->closestVertices[client_i][posClosest]].insert(client_i);
                            posClosest++;
                        }
                    }
                }
                else
                {
                    std::cout << "Heatmap could not be found. Terminating program." << std::endl;
                    exit(0);
                }
            }

            // Filling the vector of correlated vertices
            for (int i = 1; i < setCorrelatedVertices.size(); i++)
            {

                // Reset the original list of correlatedVertices (closest criteria)
                if (params->useHeatmapLS == 1)
                    params->correlatedVertices[i].clear();
                if (params->crossoverType == 1)
                    params->correlatedVerticesCrossover[i].clear();
                for (int x : setCorrelatedVertices[i])
                {
                    if (x == 0)
                    {
                        std::cout << "Safety check. This cannot happen" << std::endl;
                        exit(0);
                    }
                    if (params->useHeatmapLS == 1)
                        params->correlatedVertices[i].push_back(x);
                    if (params->crossoverType == 1)
                        params->correlatedVerticesCrossover[i].push_back(x);
                }
            }

            // Including the generation of heatmaps into the time
            std::ifstream timeSpentFile("DPDP/dpdp/results/vrp/" + heatmapName + "/heatmaps/time.txt");
            if (timeSpentFile.is_open())
            {
                timeSpentFile >> params->time_shift_export_heatmap;
            }
            else
            {
                std::cout << "Heatmap time file does not exist. Terminating code." << std::endl;
                exit(0);
            }

            if (params->time_shift_export_heatmap > timeLimit)
            {
                std::cout << "params->time_shift_export_heatmap > timeLimit: " << params->time_shift_export_heatmap << ":" << timeLimit << std::endl;
                exit(0);
            }
            printTimeShift();
        }
    }

    void printTimeShift()
    {
        if (params->useHeatmapLS == 1 || params->crossoverType == 1)
        {
            std::cout << " - CrossoverOX with heatmaps: " << params->crossoverType << std::endl;
            std::cout << " - Granular Search with heatmaps: " << params->useHeatmapLS << std::endl;
            std::cout << " - Time Shift from HeatmapGeneration: " << params->time_shift_export_heatmap << std::endl;
        }
    }
};
#endif
