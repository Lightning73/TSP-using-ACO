/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "settings.h"
#include "graph/graph.h"
#include "graph/tsp.h"
#include "graph/geojson.h"
#include "graph/tsp.h"
//*/
int main()
{
    FILE* conf = fopen("../../../data/conf.txt", "r");
    if (conf == NULL)
    {
        printf("can't open file");
        return -1;
    }
    char graphPath[256];
    char gpsPath[256];
    int nmbpoints;
    char jsonpath[256];
    fscanf(conf, "%*[^:]:");
    fscanf(conf, "%*[ \t]");
    fscanf(conf, "%s", graphPath);
    fscanf(conf, "%*[^:]:");
    fscanf(conf, "%*[ \t]:");
    fscanf(conf, "%s", gpsPath);
    fscanf(conf, "%*[^:]:");
    fscanf(conf, "%*[ \t]:");
    fscanf(conf, "%d", &nmbpoints);
    int* farm = (int*)calloc(nmbpoints, sizeof(int));
    fscanf(conf, "%*[^:]:");
    fscanf(conf, "%*[ \t]:");
    for (int i = 0; i < nmbpoints; i++)
    {
        fscanf(conf, "%d", &farm[i]);
    }
    fscanf(conf, "%*[^:]:");
    fscanf(conf, "%*[ \t]:");
    fscanf(conf, "%s", jsonpath);
    fclose(conf);
    Graph* graph = Graph_load(graphPath);
    if (graph == NULL)
    {
        free(farm);
        return EXIT_FAILURE;
    }
    Graph* subgraph = Graph_create(nmbpoints);
    for (int u = 0; u < nmbpoints; u++)
    {
        for (int v = 0; v < nmbpoints; v++)
        {
            if (u != v)
            {
                float* arc = Graph_getArc(graph, farm[u], farm[v]);
                if (arc != NULL)
                {
                    Graph_setArc(subgraph, u, v, *arc);
                }
                else
                {
                    Path* shortestPath = Graph_shortestPath(graph, farm[u], farm[v]);
                    if (shortestPath != NULL)
                    {
                        Graph_setArc(subgraph, u, v, shortestPath->distance);
                        Path_destroy(shortestPath);
                    }
                }
            }
        }
    }
    Path* path = Graph_tspFromACO(subgraph, 0, 200, 60, 2.0f, 3.0f, 0.1f, 2.0f);
    
    


    printf("%.1f %d\n", path->distance, path->list->nodeCount);
    ListIntNode* curr = path->list->sentinel.next;
    while (curr != &(path->list->sentinel))
    {
        printf("%d", curr->value);
        if (curr->next != &(path->list->sentinel))
        {
            printf(" ");
        }
        curr = curr->next;
    }
    printf("\n");
    ConvertToGeojson(graph, path, jsonpath, gpsPath, farm, nmbpoints);
    Graph_destroy(graph);
    Graph_destroy(subgraph);
    Path_destroy(path);
    free(farm);

    return EXIT_SUCCESS;

}

