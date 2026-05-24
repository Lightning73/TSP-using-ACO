#pragma once

#include "../settings.h"
#include "graph.h"
#include "../utils/heap_int.h"

/// @brief Crée un tas binaire.
/// @param size la taille du tas binaire.
/// @return Le tas binaire créé.
HeapInt* heap_create(int size);

/// @brief Descend un noeud
/// @param heap le tas binaire, i le noeud à descendre.
void heap_down(HeapInt* heap, int i);
/// @brief Remonte un noeud
/// @param heap le tas binaire, i le noeud à remonter
void heap_up(HeapInt* heap, int i);

/// @brief échange 2 noeuds
/// @param a et b les noeuds
void swap(HeapIntPair* a, HeapIntPair* b);

/// @brief ajout d'un élément dans le tas binaire
/// @param heap le tas binaire
/// @param element l'element à ajouter
/// @param priority la priorité du nouveau noeud
void heap_insertnode(HeapInt* heap, int element, float priority);

/// @brief Récupère la valeur max du tas binaire
/// @param heap le tas binaire
/// @return La racine(maximum) du tas binaire
HeapIntPair heap_extract_max(HeapInt* heap);

/// @brief Modifie la priorité d'un noeud
/// @param heap le tas binaire
/// @param nodeId le noeud à modifier
/// @param newPriority la nouvelle priorité
void heap_increase_key(HeapInt* heap, int nodeId, float newPriority);

/// @brief détruit un tas binaire
/// @param heap le tas binaire a détruire
void heap_destroy(HeapInt* heap);
