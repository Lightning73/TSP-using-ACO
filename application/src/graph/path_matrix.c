#include "shortest_path.h"
#include "binary_heap.h"

Path** path_matrix(int* farm, Graph* graph, int nmbfarm)
{
    Path** matrix = (Path**)calloc(nmbfarm * nmbfarm, sizeof(Path*)); // crée la matrice

    for (int i = 0; i < nmbfarm; i++)
    {
        for (int j = 0; j < nmbfarm; j++)
        {
            int current = i * nmbfarm + j;
            matrix[current] = Graph_shortestPath(graph, farm[i], farm[j]); // pour chaque points a visiter, trouve le chemin le plus petit entre eux
        }

    }
    return matrix;  // retourne la matrice des chemins entre chaque points a visiter

}

void path_matrix_destroy(Path** matrix, int nmbfarm)
{
    if (matrix == NULL)
    {
        return;
    }
    for (int i = 0; i < nmbfarm * nmbfarm; i++)
    {
        Path_destroy(matrix[i]);
    }
    free(matrix);
}
