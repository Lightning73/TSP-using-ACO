/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "tsp.h"
#include "binary_heap.h"

int Argmin(Graph* self, int prev, bool* explored) //
{
    float distMin = INFINITY;
    int sommet = -1;
    ArcList* current = self->vertices[prev].arcList;
    while (current != NULL)
    {
        if (explored[current->target] == 0) // si pas déjà exploré
        {
            if (current->weight <= distMin)
            {
                distMin = current->weight;
                sommet = current->target; // sommet à la dist min
            }
        }
        current = current->next;
    }
    return sommet;
}

Path* Graph_tspFromHeuristic(Graph* self, int station)
{
    int prev = station;
    Path* path = Path_create(station);
    int size = self->vertexCount;
    bool* explored = calloc(size, sizeof(bool));
    explored[prev] = true;

    for (int i = 0; i < size - 1; i++)
    {
        int next = Argmin(self, prev, explored);// On recupère le sommet avec la distance minimale
        if (next == -1)
        {
            break; // si on a pas trouvé de noeud
        }
        ListInt_insertLast(path->list, next);
        path->distance += *(Graph_getArc(self, prev, next)); //mise à jour de la distance

        explored[next] = 1;//on marque le noeud comme exploré
        prev = next;
    }
    ListInt_insertLast(path->list, station); //on remet le premier point pour faire une boucle
    path->distance += *(Graph_getArc(self, prev, station));// on rajoute la distance du dernier au premier noeud
    free(explored);
    return path;
}


float* Graph_acoGetProbabilities(
    Graph* distances, Graph* pheromones, int start, bool* explored,
    float alpha, float beta)
{
    int count = distances->vertexCount;
    float total = 0.0f;
    float* tab = (float*)calloc(count, sizeof(float));
    AssertNew(tab);

    for (int i = 0; i < count; i++)
    {
        if (explored[i] == true)
        {
            tab[i] = 0;
        }
        else
        {
            float* pheromone = Graph_getArc(pheromones, start, i);
            float* distance = Graph_getArc(distances, start, i);

            if (pheromone && distance && *distance > 0.0f)
            {
                float countpheromones = *pheromone;
                float countdistances = (1.0f / *distance);
                float countdistphero = powf(countpheromones, alpha) * powf(countdistances, beta);
                tab[i] = countdistphero;
                total += countdistphero;
            }
            else
            {
                tab[i] = 0.0f;
            }

        }
    }
    if (total <= 0.0f)
    {
        return tab;
    }
    if (total > 0.0f)
    {
        for (int i = 0; i < count; i++)
        {
            tab[i] /= total;
        }
    }

    return tab;
}


Path* Graph_acoConstructPath(
    Graph* distances, Graph* pheromones, int station,
    float alpha, float beta)// 4.2.b 
{
    int prev = station;
    Path* T = Path_create(station);
    int size = distances->vertexCount;
    bool* explored = calloc(size, sizeof(bool));
    explored[prev] = 1;

    for (int i = 0; i < size - 1; i++)
    {
        float* probabilites = Graph_acoGetProbabilities(distances, pheromones, prev, explored, alpha, beta);

        float random = (float)rand() / (float)RAND_MAX;
        float buffer = 0;
        int next = -1;
        // on additionne les probas jusqu'a ce qu'on trouve l'endroit qui est le plus proche du rand
        for (int n = 0; n < size; n++)
        {
            buffer += probabilites[n];
            if (buffer > random)
            {
                next = n;
                break;
            }
        }
        if (next == -1)
        {
            free(probabilites);
            break;
        }

        ListInt_insertLast(T->list, next);
        T->distance += *(Graph_getArc(distances, prev, next)); //mise à jour de la distance
        explored[next] = 1;//on marque le noeud comme exploré
        prev = next;
        free(probabilites);
    }

    ListInt_insertLast(T->list, station);
    T->distance += *(Graph_getArc(distances, prev, station));// on rajoute la distance du dernier au premier noeud
    free(explored);
    return T;
}

void Graph_acoPheromoneUpdatePath(Graph* pheromones, Path* path, float q)
{
    ListInt* last = path->list;
    ListIntNode* node = last->sentinel.next;

    int count = path->list->nodeCount;
    for (int i = 0; i < count - 1; i++)
    {
        float* arcdecon = Graph_getArc(pheromones, node->value, node->next->value);
        if (arcdecon == NULL)
        {
            return;
        }
        float total = *arcdecon;
        total += q / path->distance;
        Graph_setArc(pheromones, node->value, node->next->value, total);
        node = node->next;
    }
}

void Graph_acoPheromoneGlobalUpdate(Graph* pheromones, float rho)
{
    int count = pheromones->vertexCount;
    for (int i = 0; i < count; i++)
    {
        ArcList* count2 = pheromones->vertices[i].arcList;// plus simple et moins brouillon comme ça je pense
        while (count2 != NULL)
        {
            count2->weight = (1 - rho) * count2->weight;
            count2 = count2->next;
        }
    }
    return;
}

Path* Graph_tspFromACO(
    Graph* distances, int station, int iterationCount, int antCount,
    float alpha, float beta, float rho, float q) // 4.2) Mise en place d'un ACO
{

    Path* T = NULL;
    Graph* P = Graph_create(distances->vertexCount);//Graphe de p
    for (int u = 0; u < P->vertexCount; u++)
    {
        for (int v = 0; v < P->vertexCount; v++)
        {
            if (Graph_getArc(distances, u, v))
            {
                Graph_setArc(P, u, v, 1.0f);
            }
        }
    }

    for (int i = 0; i < iterationCount; i++)
    {
        for (int j = 0; j < antCount; j++)
        {
            Path* Tj = Graph_acoConstructPath(distances, P, station, alpha, beta);
            Graph_acoPheromoneUpdatePath(P, Tj, q);
            if (Tj->list->nodeCount == distances->vertexCount + 1 && (T == NULL || Tj->distance < T->distance))
            {
                if (T != NULL)
                {
                    Path_destroy(T);
                }
                T = Tj;
            }
            else
            {
                Path_destroy(Tj);
            }
        }
        //Evap P
        Graph_acoPheromoneGlobalUpdate(P, rho);
    }
    Graph_destroy(P);
    return T;
}
