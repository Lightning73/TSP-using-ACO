#pragma once

#include "../settings.h"
#include "graph.h"
#include "utils/heap_int.h"
#include "shortest_path.h"

/// @brief crée une matrice du nombre de points multipliée par le nombre de points
/// @param farm le tableau des points a visiter
/// @param graph le graphe
/// @param nmbfarm le nombre de points à visiter 
/// @return retourne la matrice des chemins entre chaque points a visiter
Path** path_matrix(int* farm, Graph* graph, int nmbfarm);

void path_matrix_destroy(Path** matrix, int nmbfarm);
